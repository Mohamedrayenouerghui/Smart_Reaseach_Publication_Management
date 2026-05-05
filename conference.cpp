#include "conference.h"
#include "ui_conference.h"
#include "connection.h"
#include "conferencemodel.h"
#include "smsservice.h"
#include "locationservice.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPrinter>
#include <QTextDocument>
#include <QCursor>
#include <QMenu>
#include <QSqlDatabase>
#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPainter>
#include <QDate>
#include <QStackedWidget>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

// ── Helper: styled confirm dialog ────────────────────────────────
static int confirmDialog(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(
        "QPushButton { background-color:#6B7280; color:white; border:none;"
        "border-radius:6px; padding:8px 20px; min-width:80px; font-size:13px; }"
        "QPushButton:hover { background-color:#4B5563; }");
    return msgBox.exec();
}

// ── Constructor ───────────────────────────────────────────────────
Conference::Conference(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Conference)
{
    ui->setupUi(this);
    setupConnections();

    if (Connection::createInstance().getDb().isOpen()) {
        manager.setDatabase(Connection::createInstance().getDb());
        QList<ConferenceModel> list = ConferenceModel::afficherTous();
        QList<ConferenceData> result;
        result.reserve(list.size());
        for (const ConferenceModel &c : list) {
            ConferenceData d;
            d.id          = c.getIdConference();
            d.nom         = c.getNom();
            d.lieu        = c.getLieu();
            d.dateDebut   = c.getDateDebut();
            d.dateFin     = c.getDateFin();
            d.fraisInscri = c.getFraisInscri();
            d.etat        = c.getEtat();
            d.theme       = c.getTheme();
            d.idArticle   = c.getIdArticle();
            result.append(d);
        }
        refreshTable(result);
    }
}

Conference::~Conference()
{
    delete ui;
}

// ── setupConnections ──────────────────────────────────────────────
void Conference::setupConnections()
{
    connect(ui->ajouterButton,      &QPushButton::clicked, this, &Conference::onAjouterClicked);
    connect(ui->modifierButton,     &QPushButton::clicked, this, &Conference::onModifierClicked);
    connect(ui->supprimerButton,    &QPushButton::clicked, this, &Conference::onSupprimerClicked);
    connect(ui->rechercherButton,   &QPushButton::clicked, this, &Conference::onRechercherClicked);
    connect(ui->trierButton,        &QPushButton::clicked, this, &Conference::onTrierClicked);
    connect(ui->exporterPDFButton,  &QPushButton::clicked, this, &Conference::onExporterPDFClicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &Conference::onStatistiquesClicked);
    connect(ui->clearSearchButton,  &QPushButton::clicked, this, &Conference::onClearSearchClicked);
    connect(ui->ajouterFormButton,  &QPushButton::clicked, this, &Conference::onAjouterFormClicked);
    connect(ui->envoyerSMSButton,   &QPushButton::clicked, this, &Conference::onEnvoyerSMSClicked);
    connect(ui->localisationButton, &QPushButton::clicked, this, &Conference::onLocalisationClicked);
    connect(ui->modifierFormButton, &QPushButton::clicked, this, &Conference::onModifierFormClicked);
    connect(ui->annulerModifButton, &QPushButton::clicked, this, &Conference::onAnnulerModifClicked);
    connect(ui->arduinoConfButton,  &QPushButton::clicked, this, &Conference::onArduinoConfClicked);

    if (ui->exporterWordButton) {
        ui->exporterWordButton->hide();
        ui->exporterWordButton->setEnabled(false);
    }
}

// ── refreshTable ─────────────────────────────────────────────────
void Conference::refreshTable(const QList<ConferenceData> &conferences)
{
    ui->conferencesTable->setRowCount(0);
    ui->conferencesTable->setRowCount(conferences.size());

    for (int row = 0; row < conferences.size(); ++row) {
        const ConferenceData &c = conferences.at(row);
        ui->conferencesTable->setItem(row, 0, new QTableWidgetItem(QString::number(c.id)));
        ui->conferencesTable->setItem(row, 1, new QTableWidgetItem(c.nom));
        ui->conferencesTable->setItem(row, 2, new QTableWidgetItem(c.lieu));
        ui->conferencesTable->setItem(row, 3, new QTableWidgetItem(c.dateDebut.toString(Qt::ISODate)));
        ui->conferencesTable->setItem(row, 4, new QTableWidgetItem(c.dateFin.toString(Qt::ISODate)));
        ui->conferencesTable->setItem(row, 5, new QTableWidgetItem(QString::number(c.fraisInscri, 'f', 2)));
        ui->conferencesTable->setItem(row, 6, new QTableWidgetItem(c.etat));
        ui->conferencesTable->setItem(row, 7, new QTableWidgetItem(c.theme));
    }
    ui->conferencesTable->resizeColumnsToContents();
}

// ── readConferenceFromForm ────────────────────────────────────────
ConferenceData Conference::readConferenceFromForm() const
{
    ConferenceData conf;
    conf.id          = -1;
    conf.nom         = ui->nomLineEdit->text().trimmed();
    conf.lieu        = ui->lieuLineEdit->text().trimmed();
    conf.dateDebut   = ui->dateDebutEdit->date();
    conf.dateFin     = ui->dateFinEdit->date();
    conf.fraisInscri = ui->fraisSpinBox->value();
    conf.etat        = ui->etatComboBox->currentText();
    conf.theme       = ui->themeComboBox->currentText();
    conf.idArticle   = 0;
    return conf;
}

// ── populateModifierForm ──────────────────────────────────────────
void Conference::populateModifierForm(const ConferenceData &conf)
{
    ui->modIdLineEdit->setText(QString::number(conf.id));
    ui->modNomLineEdit->setText(conf.nom);
    ui->modLieuLineEdit->setText(conf.lieu);
    ui->modDateDebutEdit->setDate(conf.dateDebut.isValid() ? conf.dateDebut : QDate::currentDate());
    ui->modDateFinEdit->setDate(conf.dateFin.isValid()     ? conf.dateFin   : QDate::currentDate());
    ui->modFraisSpinBox->setValue(conf.fraisInscri);

    int etatIdx = ui->modEtatComboBox->findText(conf.etat);
    if (etatIdx >= 0) ui->modEtatComboBox->setCurrentIndex(etatIdx);

    int themeIdx = ui->modThemeComboBox->findText(conf.theme);
    if (themeIdx >= 0) ui->modThemeComboBox->setCurrentIndex(themeIdx);
}

// ── AJOUTER ───────────────────────────────────────────────────────
void Conference::onAjouterClicked()
{
    ui->tabWidget->setCurrentIndex(2); // Ajouter tab
}

void Conference::onAjouterFormClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }

    ConferenceData confData = readConferenceFromForm();
    ConferenceModel conference;
    conference.setNom(confData.nom);
    conference.setLieu(confData.lieu);
    conference.setDateDebut(confData.dateDebut);
    conference.setDateFin(confData.dateFin);
    conference.setFraisInscri(confData.fraisInscri);
    conference.setEtat(confData.etat);
    conference.setTheme(confData.theme);

    QString erreur;
    if (!conference.valider(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }
    if (!conference.ajouter()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ajouter la conference.");
        return;
    }

    ui->nomLineEdit->clear();
    ui->lieuLineEdit->clear();
    ui->fraisSpinBox->setValue(0);

    QList<ConferenceModel> list = ConferenceModel::afficherTous();
    QList<ConferenceData> toDisplay;
    toDisplay.reserve(list.size());
    for (const ConferenceModel &c : list) {
        ConferenceData d;
        d.id = c.getIdConference(); d.nom = c.getNom(); d.lieu = c.getLieu();
        d.dateDebut = c.getDateDebut(); d.dateFin = c.getDateFin();
        d.fraisInscri = c.getFraisInscri(); d.etat = c.getEtat();
        d.theme = c.getTheme(); d.idArticle = c.getIdArticle();
        toDisplay.append(d);
    }
    refreshTable(toDisplay);
    QMessageBox::information(this, "Succes", "Conference ajoutee avec succes !");
    ui->tabWidget->setCurrentIndex(0);
}

// ── MODIFIER ──────────────────────────────────────────────────────
void Conference::onModifierClicked()
{
    int row = ui->conferencesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Modifier", "Veuillez selectionner une conference dans la liste.");
        return;
    }
    bool ok = false;
    int id = ui->conferencesTable->item(row, 0)->text().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Modifier", "ID invalide."); return; }

    ConferenceModel conf = ConferenceModel::getById(id);
    if (conf.getIdConference() <= 0) {
        QMessageBox::warning(this, "Modifier", "Impossible de charger la conference.");
        return;
    }
    ConferenceData d;
    d.id = conf.getIdConference(); d.nom = conf.getNom(); d.lieu = conf.getLieu();
    d.dateDebut = conf.getDateDebut(); d.dateFin = conf.getDateFin();
    d.fraisInscri = conf.getFraisInscri(); d.etat = conf.getEtat();
    d.theme = conf.getTheme(); d.idArticle = conf.getIdArticle();

    populateModifierForm(d);
    ui->tabWidget->setCurrentIndex(1); // Modifier tab
}

void Conference::onModifierFormClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }
    bool ok = false;
    int id = ui->modIdLineEdit->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Modifier", "Aucune conference chargee. Selectionnez-en une d'abord.");
        return;
    }

    ConferenceModel conf;
    conf.setIdConference(id);
    conf.setNom(ui->modNomLineEdit->text().trimmed());
    conf.setLieu(ui->modLieuLineEdit->text().trimmed());
    conf.setDateDebut(ui->modDateDebutEdit->date());
    conf.setDateFin(ui->modDateFinEdit->date());
    conf.setFraisInscri(ui->modFraisSpinBox->value());
    conf.setEtat(ui->modEtatComboBox->currentText());
    conf.setTheme(ui->modThemeComboBox->currentText());

    QString erreur;
    if (!conf.valider(erreur)) { QMessageBox::warning(this, "Erreur de validation", erreur); return; }
    if (!conf.modifier()) { QMessageBox::critical(this, "Erreur", "Impossible de modifier la conference."); return; }

    refreshTable(manager.getAllConferences());
    QMessageBox::information(this, "Succes", "Conference modifiee avec succes !");
    ui->tabWidget->setCurrentIndex(0);
}

void Conference::onAnnulerModifClicked()
{
    ui->modIdLineEdit->clear();
    ui->modNomLineEdit->clear();
    ui->modLieuLineEdit->clear();
    ui->modFraisSpinBox->setValue(0);
    ui->tabWidget->setCurrentIndex(0);
}

// ── SUPPRIMER ─────────────────────────────────────────────────────
void Conference::onSupprimerClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }
    int row = ui->conferencesTable->currentRow();
    if (row < 0) { QMessageBox::information(this, "Supprimer", "Veuillez selectionner une conference."); return; }

    bool ok = false;
    int id = ui->conferencesTable->item(row, 0)->text().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Supprimer", "ID invalide."); return; }

    QString nom = ui->conferencesTable->item(row, 1)->text();
    if (confirmDialog(this, "Supprimer", QString("Confirmer la suppression de \"%1\" ?").arg(nom)) != QMessageBox::Yes)
        return;

    if (!manager.deleteConference(id)) {
        QMessageBox::critical(this, "Erreur", manager.getLastError());
        return;
    }
    refreshTable(manager.getAllConferences());
    QMessageBox::information(this, "Succes", "Conference supprimee avec succes !");
}

// ── RECHERCHER ────────────────────────────────────────────────────
void Conference::onRechercherClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }
    QString searchText = ui->searchLineEdit->text();
    QString searchType = ui->searchComboBox->currentText();

    QList<ConferenceModel> list = ConferenceModel::rechercher(searchType, searchText.trimmed());
    QList<ConferenceData> result;
    result.reserve(list.size());
    for (const ConferenceModel &c : list) {
        ConferenceData d;
        d.id = c.getIdConference(); d.nom = c.getNom(); d.lieu = c.getLieu();
        d.dateDebut = c.getDateDebut(); d.dateFin = c.getDateFin();
        d.fraisInscri = c.getFraisInscri(); d.etat = c.getEtat();
        d.theme = c.getTheme(); d.idArticle = c.getIdArticle();
        result.append(d);
    }
    refreshTable(result);
}

// ── TRIER ─────────────────────────────────────────────────────────
void Conference::onTrierClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }

    auto buildResult = [](const QList<ConferenceModel> &list) {
        QList<ConferenceData> result;
        result.reserve(list.size());
        for (const ConferenceModel &c : list) {
            ConferenceData d;
            d.id = c.getIdConference(); d.nom = c.getNom(); d.lieu = c.getLieu();
            d.dateDebut = c.getDateDebut(); d.dateFin = c.getDateFin();
            d.fraisInscri = c.getFraisInscri(); d.etat = c.getEtat();
            d.theme = c.getTheme(); d.idArticle = c.getIdArticle();
            result.append(d);
        }
        return result;
    };

    QMenu menu(this);
    menu.addAction("Trier par Nom (A-Z)",   [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Nom",   true))); });
    menu.addAction("Trier par Nom (Z-A)",   [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Nom",   false))); });
    menu.addSeparator();
    menu.addAction("Trier par Lieu (A-Z)",  [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Lieu",  true))); });
    menu.addAction("Trier par Lieu (Z-A)",  [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Lieu",  false))); });
    menu.addSeparator();
    menu.addAction("Trier par Date (asc)",  [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Date",  true))); });
    menu.addAction("Trier par Date (desc)", [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Date",  false))); });
    menu.addSeparator();
    menu.addAction("Trier par Frais (asc)", [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Frais", true))); });
    menu.addAction("Trier par Frais (desc)",[this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Frais", false))); });
    menu.addSeparator();
    menu.addAction("Trier par Etat",        [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Etat",  true))); });
    menu.addAction("Trier par Theme",       [this, buildResult]() { refreshTable(buildResult(ConferenceModel::trierPar("Theme", true))); });
    menu.exec(QCursor::pos());
}

// ── EXPORTER PDF ──────────────────────────────────────────────────
void Conference::onExporterPDFClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter PDF", "conferences.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QList<ConferenceModel> list = ConferenceModel::afficherTous();
    QString html;
    html += "<html><head><meta charset='UTF-8'></head><body>";
    html += "<h2 style='color:#8B5CF6;'>Liste des Conferences</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%' style='border-collapse:collapse; font-size:12px;'>";
    html += "<tr style='background-color:#8B5CF6; color:white;'><th>ID</th><th>Nom</th><th>Lieu</th><th>Date Debut</th><th>Date Fin</th><th>Frais (DT)</th><th>Etat</th><th>Theme</th></tr>";

    for (int i = 0; i < list.size(); ++i) {
        const ConferenceModel &c = list.at(i);
        QString bg = (i % 2 == 0) ? "#FFFFFF" : "#F3F4F6";
        html += QString("<tr style='background-color:%1;'><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td></tr>")
                    .arg(bg).arg(c.getIdConference())
                    .arg(c.getNom().toHtmlEscaped()).arg(c.getLieu().toHtmlEscaped())
                    .arg(c.getDateDebut().toString("dd/MM/yyyy")).arg(c.getDateFin().toString("dd/MM/yyyy"))
                    .arg(QString::number(c.getFraisInscri(), 'f', 2))
                    .arg(c.getEtat().toHtmlEscaped()).arg(c.getTheme().toHtmlEscaped());
    }
    html += "</table>";
    html += QString("<p style='color:#6B7280; font-size:11px;'>Total: %1 conference(s) - Exporte le %2</p>")
                .arg(list.size()).arg(QDate::currentDate().toString("dd/MM/yyyy"));
    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);
    QMessageBox::information(this, "Exporter PDF", QString("PDF exporte avec succes !\n%1").arg(filePath));
}

void Conference::onExporterWordClicked()
{
    QMessageBox::information(this, "Exporter Word", "Export Word desactive.");
}

// ── SMS WHATSAPP ──────────────────────────────────────────────────
void Conference::onEnvoyerSMSClicked()
{
    if (!smsService) {
        smsService = new SMSService(nullptr);
        smsService->setWindowFlags(Qt::Window);
        smsService->setAttribute(Qt::WA_DeleteOnClose);
        connect(smsService, &QObject::destroyed, this, [this]() { smsService = nullptr; });
    }
    smsService->show();
    smsService->raise();
    smsService->activateWindow();
}

// ── LOCALISATION ──────────────────────────────────────────────────
void Conference::onLocalisationClicked()
{
    int row = ui->conferencesTable->currentRow();
    QString lieu;
    if (row >= 0 && ui->conferencesTable->item(row, 2))
        lieu = ui->conferencesTable->item(row, 2)->text();

    if (!locationService) {
        locationService = new LocationService(nullptr);
        locationService->setWindowFlags(Qt::Window);
        locationService->setAttribute(Qt::WA_DeleteOnClose);
        connect(locationService, &QObject::destroyed, this, [this]() { locationService = nullptr; });
    }
    if (!lieu.isEmpty()) locationService->setLieu(lieu);
    locationService->show();
    locationService->raise();
    locationService->activateWindow();
}

// ── STATISTIQUES ──────────────────────────────────────────────────
void Conference::onStatistiquesClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees", "Aucune connexion a la base de donnees.");
        return;
    }

    QMap<QString, int> statsTheme = ConferenceModel::getStatsParTheme();
    QMap<QString, int> statsEtat  = ConferenceModel::getStatsParEtat();

    if (statsTheme.isEmpty() && statsEtat.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune donnee disponible.");
        return;
    }

    // Pie chart - by theme
    auto *pieSeries = new QPieSeries();
    for (auto it = statsTheme.cbegin(); it != statsTheme.cend(); ++it) {
        QPieSlice *slice = pieSeries->append(QString("%1 (%2)").arg(it.key()).arg(it.value()), it.value());
        slice->setLabelVisible(true);
    }
    auto *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Repartition par Theme");
    pieChart->setAnimationOptions(QChart::AllAnimations);
    pieChart->legend()->setAlignment(Qt::AlignBottom);
    auto *pieView = new QChartView(pieChart);
    pieView->setRenderHint(QPainter::Antialiasing);

    // Bar chart - by etat
    auto *barSet = new QBarSet("Conferences");
    QStringList categories;
    for (auto it = statsEtat.cbegin(); it != statsEtat.cend(); ++it) {
        *barSet << it.value();
        categories << it.key();
    }
    auto *barSeries = new QBarSeries();
    barSeries->append(barSet);
    auto *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Repartition par Etat");
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);
    auto *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Nombre");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);
    barChart->legend()->setVisible(false);
    auto *barView = new QChartView(barChart);
    barView->setRenderHint(QPainter::Antialiasing);

    // Summary
    int total = 0;
    for (int v : statsEtat) total += v;
    double avgFees = manager.getAverageFees();

    auto *summaryLabel = new QLabel(
        QString("<b>Total conferences:</b> %1 &nbsp;&nbsp; <b>Frais moyens:</b> %2 DT &nbsp;&nbsp; <b>Themes:</b> %3 &nbsp;&nbsp; <b>Etats:</b> %4")
            .arg(total).arg(avgFees, 0, 'f', 2).arg(statsTheme.size()).arg(statsEtat.size()));
    summaryLabel->setAlignment(Qt::AlignCenter);
    summaryLabel->setStyleSheet("font-size:14px; padding:8px; background:#F3F4F6; border-radius:6px;");

    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des Conferences");
    dialog->resize(900, 600);
    auto *tabs = new QTabWidget(dialog);
    tabs->addTab(pieView, "Par Theme");
    tabs->addTab(barView, "Par Etat");
    auto *closeBtn = new QPushButton("Fermer", dialog);
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    auto *layout = new QVBoxLayout(dialog);
    layout->addWidget(summaryLabel);
    layout->addWidget(tabs);
    layout->addWidget(closeBtn);
    dialog->setLayout(layout);
    dialog->exec();
}

// ── CLEAR SEARCH ─────────────────────────────────────────────────
void Conference::onClearSearchClicked()
{
    ui->searchLineEdit->clear();
    if (Connection::createInstance().getDb().isOpen()) {
        QList<ConferenceModel> list = ConferenceModel::afficherTous();
        QList<ConferenceData> result;
        result.reserve(list.size());
        for (const ConferenceModel &c : list) {
            ConferenceData d;
            d.id = c.getIdConference(); d.nom = c.getNom(); d.lieu = c.getLieu();
            d.dateDebut = c.getDateDebut(); d.dateFin = c.getDateFin();
            d.fraisInscri = c.getFraisInscri(); d.etat = c.getEtat();
            d.theme = c.getTheme(); d.idArticle = c.getIdArticle();
            result.append(d);
        }
        refreshTable(result);
    }
}

// ── ARDUINO BUTTON (navigate to Arduino page) ────────────────────
void Conference::onArduinoConfClicked()
{
    QWidget *w = this;
    while (w && !w->inherits("QMainWindow")) w = w->parentWidget();
    if (w) {
        // Trigger the Arduino page via stackedWidget index 7
        QStackedWidget *stack = w->findChild<QStackedWidget*>("stackedWidget");
        if (stack) stack->setCurrentIndex(7);
        QLabel *title = w->findChild<QLabel*>("titleLabel");
        if (title) title->setText("Arduino - Moniteur Capteur");
    }
}
