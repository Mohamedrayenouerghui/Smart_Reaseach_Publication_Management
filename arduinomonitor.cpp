#include "arduinomonitor.h"
#include "ui_arduinomonitor.h"
#include "connection.h"

#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QScrollBar>

ArduinoMonitor::ArduinoMonitor(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ArduinoMonitor),
      m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    // Populate ports
    onRefreshPortsClicked();

    // Connections
    connect(ui->connectBtn,    &QPushButton::clicked, this, &ArduinoMonitor::onConnecterClicked);
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &ArduinoMonitor::onDeconnecterClicked);
    connect(ui->clearBtn,      &QPushButton::clicked, this, &ArduinoMonitor::onEffacerClicked);
    connect(ui->exportBtn,     &QPushButton::clicked, this, &ArduinoMonitor::onExporterPDFClicked);
    connect(ui->refreshBtn,    &QPushButton::clicked, this, &ArduinoMonitor::onRefreshPortsClicked);
    connect(m_serial, &QSerialPort::readyRead, this, &ArduinoMonitor::onDonneesRecues);

    ui->disconnectBtn->setEnabled(false);

    // Create Oracle table if needed and load history
    if (Connection::createInstance().getDb().isOpen()) {
        createTableIfNeeded();
        loadHistory();
    }
}

ArduinoMonitor::~ArduinoMonitor()
{
    if (m_serial->isOpen()) m_serial->close();
    delete ui;
}

// ─────────────────────────────────────────────
//  Refresh available serial ports
// ─────────────────────────────────────────────
void ArduinoMonitor::onRefreshPortsClicked()
{
    ui->portComboBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        ui->portComboBox->addItem(info.portName());
    }
    if (ui->portComboBox->count() == 0) {
        ui->portComboBox->addItem("Aucun port");
    }
}

// ─────────────────────────────────────────────
//  Connect to Arduino
// ─────────────────────────────────────────────
void ArduinoMonitor::onConnecterClicked()
{
    if (m_serial->isOpen()) m_serial->close();

    m_serial->setPortName(ui->portComboBox->currentText());
    m_serial->setBaudRate(ui->baudComboBox->currentText().toInt());
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial->open(QIODevice::ReadOnly)) {
        ui->statusLabel->setText("● Connecté");
        ui->statusLabel->setStyleSheet("background:#1E293B; color:#10B981;");
        ui->connectBtn->setEnabled(false);
        ui->disconnectBtn->setEnabled(true);
        ui->consoleTextEdit->append(
            QString("<span style='color:#10B981;'>[%1] Connecté sur %2 @ %3 baud</span>")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                .arg(m_serial->portName())
                .arg(ui->baudComboBox->currentText()));
    } else {
        QMessageBox::critical(this, "Erreur",
            QString("Impossible d'ouvrir le port %1 :\n%2")
                .arg(m_serial->portName())
                .arg(m_serial->errorString()));
    }
}

// ─────────────────────────────────────────────
//  Disconnect
// ─────────────────────────────────────────────
void ArduinoMonitor::onDeconnecterClicked()
{
    if (m_serial->isOpen()) m_serial->close();
    ui->statusLabel->setText("● Déconnecté");
    ui->statusLabel->setStyleSheet("background:#1E293B; color:#EF4444;");
    ui->connectBtn->setEnabled(true);
    ui->disconnectBtn->setEnabled(false);
    ui->consoleTextEdit->append(
        QString("<span style='color:#EF4444;'>[%1] Déconnecté</span>")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
}

// ─────────────────────────────────────────────
//  Read serial data
// ─────────────────────────────────────────────
void ArduinoMonitor::onDonneesRecues()
{
    m_buffer += QString::fromLatin1(m_serial->readAll());

    // Process complete lines
    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QString line = m_buffer.left(idx).trimmed();
        m_buffer = m_buffer.mid(idx + 1);

        if (!line.isEmpty()) {
            // Show in console
            ui->consoleTextEdit->append(
                QString("<span style='color:#94A3B8;'>[%1] %2</span>")
                    .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                    .arg(line.toHtmlEscaped()));
            // Auto-scroll
            ui->consoleTextEdit->verticalScrollBar()->setValue(
                ui->consoleTextEdit->verticalScrollBar()->maximum());

            parseLine(line);
        }
    }
}

// ─────────────────────────────────────────────
//  Parse a line from Arduino
//  Expected lines:
//    "Temperature : 25.50 C"
//    "Variation   : 0.50 C"
//    ">>> FLAMME DETECTEE ! <<<"
//    "Normal"
// ─────────────────────────────────────────────
void ArduinoMonitor::parseLine(const QString &line)
{
    if (line.startsWith("Temperature")) {
        // "Temperature : 25.50 C"
        QString val = line.section(':', 1).trimmed().remove(" C").trimmed();
        bool ok;
        double t = val.toDouble(&ok);
        if (ok) m_lastTemp = t;
    }
    else if (line.startsWith("Variation")) {
        // "Variation   : 0.50 C"
        QString val = line.section(':', 1).trimmed().remove(" C").trimmed();
        bool ok;
        double v = val.toDouble(&ok);
        if (ok) m_lastVariation = v;
    }
    else if (line.contains("FLAMME")) {
        m_lastEtat = "FLAMME DETECTEE";
        updateDisplay();

        // Save to DB
        CapteurData data;
        data.dateHeure  = QDateTime::currentDateTime();
        data.temperature= m_lastTemp;
        data.humidite   = m_lastHumidite;
        data.variation  = m_lastVariation;
        data.etat       = m_lastEtat;
        saveToDatabase(data);

        // Alerte une seule fois jusqu'au retour à Normal
        if (!m_alerteActive) {
            m_alerteActive = true;
            updateConferenceEtat("Fermee");
            updateLaboratoireEtat("Alerte Flamme");
            alerteFlamme();
        }
    }
    else if (line == "Normal") {
        m_lastEtat     = "Normal";
        m_alerteActive = false;  // reset pour la prochaine alerte
        updateDisplay();

        // Save to DB every "Normal" reading
        CapteurData data;
        data.dateHeure  = QDateTime::currentDateTime();
        data.temperature= m_lastTemp;
        data.humidite   = m_lastHumidite;
        data.variation  = m_lastVariation;
        data.etat       = m_lastEtat;
        saveToDatabase(data);
    }
    else if (line.startsWith("Humidite") || line.startsWith("Humidity")) {
        QString val = line.section(':', 1).trimmed().remove(" %").trimmed();
        bool ok;
        double h = val.toDouble(&ok);
        if (ok) m_lastHumidite = h;
    }
}

// ─────────────────────────────────────────────
//  Update live display cards
// ─────────────────────────────────────────────
void ArduinoMonitor::updateDisplay()
{
    ui->tempValueLabel->setText(QString("%1 °C").arg(m_lastTemp, 0, 'f', 1));
    ui->humValueLabel->setText(QString("%1 %").arg(m_lastHumidite, 0, 'f', 1));
    ui->varValueLabel->setText(QString("%1 °C").arg(m_lastVariation, 0, 'f', 1));

    if (m_lastEtat == "FLAMME DETECTEE") {
        ui->etatValueLabel->setText("🔥 FLAMME !");
        ui->etatValueLabel->setStyleSheet("color:#EF4444; font-size:26px; font-weight:bold;");
        ui->etatGroup->setStyleSheet("QGroupBox { border: 2px solid #EF4444; border-radius:8px; }");
        ui->tempValueLabel->setStyleSheet("color:#EF4444; font-size:36px; font-weight:bold;");
    } else {
        ui->etatValueLabel->setText("✅ Normal");
        ui->etatValueLabel->setStyleSheet("color:#34D399; font-size:26px; font-weight:bold;");
        ui->etatGroup->setStyleSheet("QGroupBox { border: 1px solid #334155; border-radius:8px; }");
        ui->tempValueLabel->setStyleSheet("color:#38BDF8; font-size:36px; font-weight:bold;");
    }
}

// ─────────────────────────────────────────────
//  Save reading to Oracle
// ─────────────────────────────────────────────
void ArduinoMonitor::saveToDatabase(const CapteurData &data)
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) return;

    QString dateStr = data.dateHeure.toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO CAPTEUR_DATA (ID_CAPTEUR, DATE_LECTURE, TEMPERATURE, HUMIDITE, VARIATION, ETAT) "
        "VALUES (SEQ_CAPTEUR.NEXTVAL, TO_DATE(:date, 'YYYY-MM-DD HH24:MI:SS'), :temp, :hum, :var, :etat)");
    query.bindValue(":date", dateStr);
    query.bindValue(":temp", data.temperature);
    query.bindValue(":hum",  data.humidite);
    query.bindValue(":var",  data.variation);
    query.bindValue(":etat", data.etat);

    if (!query.exec()) {
        qDebug() << "[Arduino] Erreur INSERT:" << query.lastError().text();
        return;
    }

    // Add row to history table
    int row = ui->historyTable->rowCount();
    ui->historyTable->insertRow(row);
    ui->historyTable->setItem(row, 0, new QTableWidgetItem(
        data.dateHeure.toString("dd/MM/yyyy hh:mm:ss")));
    ui->historyTable->setItem(row, 1, new QTableWidgetItem(
        QString::number(data.temperature, 'f', 1)));
    ui->historyTable->setItem(row, 2, new QTableWidgetItem(
        QString::number(data.humidite, 'f', 1)));
    ui->historyTable->setItem(row, 3, new QTableWidgetItem(
        QString::number(data.variation, 'f', 1)));

    auto *etatItem = new QTableWidgetItem(data.etat);
    if (data.etat == "FLAMME DETECTEE") {
        etatItem->setForeground(QColor("#EF4444"));
        etatItem->setFont(QFont("", -1, QFont::Bold));
    } else {
        etatItem->setForeground(QColor("#34D399"));
    }
    ui->historyTable->setItem(row, 4, etatItem);

    ui->historyTable->scrollToBottom();
    ui->historyTable->resizeColumnsToContents();
}

// ─────────────────────────────────────────────
//  Flame alert popup
// ─────────────────────────────────────────────
void ArduinoMonitor::alerteFlamme()
{
    QMessageBox alert(this);
    alert.setWindowTitle("🔥 ALERTE FLAMME DETECTEE !");
    alert.setText(
        QString("<b style='color:#EF4444; font-size:16px;'>🔥 FLAMME DETECTEE !</b><br><br>"
                "Température : <b>%1 °C</b><br>"
                "Variation   : <b>%2 °C</b><br><br>"
                "Les conférences et laboratoires ont été mis en alerte.<br>"
                "Veuillez évacuer immédiatement !")
            .arg(m_lastTemp, 0, 'f', 1)
            .arg(m_lastVariation, 0, 'f', 1));
    alert.setIcon(QMessageBox::Critical);
    alert.setStyleSheet(
        "QMessageBox { background-color: #1E293B; }"
        "QLabel { color: #E2E8F0; font-size: 14px; }"
        "QPushButton { background-color:#EF4444; color:white; border:none; "
        "border-radius:6px; padding:8px 20px; font-size:13px; }"
        "QPushButton:hover { background-color:#DC2626; }");
    alert.exec();
}

// ─────────────────────────────────────────────
//  Update Conference state in Oracle
// ─────────────────────────────────────────────
void ArduinoMonitor::updateConferenceEtat(const QString &etat)
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) return;

    // Met à jour toutes les conférences "Ouverte" en "Fermée"
    QSqlQuery query(db);
    query.prepare("UPDATE CONFERENCE SET etat = :etat WHERE etat = 'Ouverte'");
    query.bindValue(":etat", etat);

    if (query.exec()) {
        int nb = query.numRowsAffected();
        qDebug() << "[Arduino] Conferences mises a jour:" << nb;
        ui->consoleTextEdit->append(
            QString("<span style='color:#F97316;'>[ALERTE] %1 conference(s) passee(s) en etat: %2</span>")
                .arg(nb).arg(etat));
    } else {
        qDebug() << "[Arduino] Erreur UPDATE CONFERENCE:" << query.lastError().text();
    }
}

// ─────────────────────────────────────────────
//  Update Laboratoire state in Oracle (historique)
// ─────────────────────────────────────────────
void ArduinoMonitor::updateLaboratoireEtat(const QString &etat)
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) return;

    // Ajouter une entrée dans l'historique de tous les labos
    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    QString entree  = QString("%1|Tous|Alerte Arduino|systeme|%2\n")
                          .arg(dateStr).arg(etat);

    QSqlQuery query(db);
    // Concaténer l'entrée au début de l'historique existant
    query.prepare(
        "UPDATE LABORATOIRE SET historique = :entree || NVL(historique, '') "
        "WHERE 1=1");
    query.bindValue(":entree", entree);

    if (query.exec()) {
        qDebug() << "[Arduino] Historique laboratoires mis a jour";
        ui->consoleTextEdit->append(
            QString("<span style='color:#F97316;'>[ALERTE] Historique laboratoires mis a jour: %1</span>")
                .arg(etat));
    } else {
        qDebug() << "[Arduino] Erreur UPDATE LABORATOIRE:" << query.lastError().text();
    }
}

// ─────────────────────────────────────────────
//  Create CAPTEUR_DATA table in Oracle if not exists
// ─────────────────────────────────────────────
void ArduinoMonitor::createTableIfNeeded()
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) return;

    QSqlQuery check(db);
    // Check if table exists in Oracle
    check.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'CAPTEUR_DATA'");
    if (!check.exec() || !check.next()) return;

    if (check.value(0).toInt() == 0) {
        // Create sequence first
        QSqlQuery seqQuery(db);
        seqQuery.exec("CREATE SEQUENCE SEQ_CAPTEUR START WITH 1 INCREMENT BY 1");

        QSqlQuery create(db);
        bool ok = create.exec(
            "CREATE TABLE CAPTEUR_DATA ("
            "  ID_CAPTEUR   NUMBER PRIMARY KEY,"
            "  DATE_LECTURE DATE NOT NULL,"
            "  TEMPERATURE  NUMBER(6,2),"
            "  HUMIDITE     NUMBER(6,2),"
            "  VARIATION    NUMBER(6,2),"
            "  ETAT         VARCHAR2(50)"
            ")");
        if (ok) {
            qDebug() << "[Arduino] Table CAPTEUR_DATA creee avec succes";
        } else {
            qDebug() << "[Arduino] Erreur creation table:" << create.lastError().text();
        }
    }
}

// ─────────────────────────────────────────────
//  Load history from Oracle on startup
// ─────────────────────────────────────────────
void ArduinoMonitor::loadHistory()
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) return;

    QSqlQuery query(db);
    if (!query.exec(
            "SELECT TO_CHAR(DATE_LECTURE,'DD/MM/YYYY HH24:MI:SS'), "
            "TEMPERATURE, HUMIDITE, VARIATION, ETAT "
            "FROM CAPTEUR_DATA ORDER BY DATE_LECTURE DESC")) {
        qDebug() << "[Arduino] Erreur chargement historique:" << query.lastError().text();
        return;
    }

    ui->historyTable->setRowCount(0);
    while (query.next()) {
        int row = ui->historyTable->rowCount();
        ui->historyTable->insertRow(row);
        ui->historyTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->historyTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->historyTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->historyTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        auto *etatItem = new QTableWidgetItem(query.value(4).toString());
        if (query.value(4).toString() == "FLAMME DETECTEE") {
            etatItem->setForeground(QColor("#EF4444"));
            etatItem->setFont(QFont("", -1, QFont::Bold));
        } else {
            etatItem->setForeground(QColor("#34D399"));
        }
        ui->historyTable->setItem(row, 4, etatItem);
    }
    ui->historyTable->resizeColumnsToContents();
}

// ─────────────────────────────────────────────
//  Clear console
// ─────────────────────────────────────────────
void ArduinoMonitor::onEffacerClicked()
{
    ui->consoleTextEdit->clear();
}

// ─────────────────────────────────────────────
//  Export history to PDF
// ─────────────────────────────────────────────
void ArduinoMonitor::onExporterPDFClicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Exporter PDF", "capteur_data.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QString html;
    html += "<html><head><meta charset='UTF-8'></head><body>";
    html += "<h2 style='color:#8B5CF6;'>Historique Capteur DHT11 - Arduino</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%' "
            "style='border-collapse:collapse; font-size:11px;'>";
    html += "<tr style='background:#8B5CF6; color:white;'>"
            "<th>Date/Heure</th><th>Temp (°C)</th><th>Humidité (%)</th>"
            "<th>Variation (°C)</th><th>État</th></tr>";

    int rows = ui->historyTable->rowCount();
    for (int i = 0; i < rows; ++i) {
        bool flamme = ui->historyTable->item(i, 4) &&
                      ui->historyTable->item(i, 4)->text() == "FLAMME DETECTEE";
        QString bg = flamme ? "#FEE2E2" : (i % 2 == 0 ? "#FFFFFF" : "#F3F4F6");
        html += QString("<tr style='background:%1;'>").arg(bg);
        for (int c = 0; c < 5; ++c) {
            QString txt = ui->historyTable->item(i, c)
                          ? ui->historyTable->item(i, c)->text().toHtmlEscaped()
                          : "";
            html += QString("<td>%1</td>").arg(txt);
        }
        html += "</tr>";
    }
    html += "</table>";
    html += QString("<p style='color:#6B7280; font-size:10px;'>%1 lecture(s) - "
                    "Exporté le %2</p>")
                .arg(rows)
                .arg(QDate::currentDate().toString("dd/MM/yyyy"));
    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10));

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);

    QMessageBox::information(this, "Export PDF",
        QString("PDF exporté avec succès !\n%1").arg(filePath));
}
