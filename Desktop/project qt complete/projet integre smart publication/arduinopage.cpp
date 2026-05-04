#include "arduinopage.h"
#include "ui_arduinopage.h"
#include "arduinomanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>
#include <QDate>
#include <QDateTime>
#include <QTimer>

ArduinoPage::ArduinoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArduinoPage)
    , arduinoManager(nullptr)
{
    ui->setupUi(this);
    setupUI();
    
    // Timer pour mettre à jour l'aperçu LCD
    lcdUpdateTimer = new QTimer(this);
    connect(lcdUpdateTimer, &QTimer::timeout, this, &ArduinoPage::updateLCDPreview);
    lcdUpdateTimer->start(1000); // Mise à jour chaque seconde
}

ArduinoPage::~ArduinoPage()
{
    delete ui;
}

void ArduinoPage::setupUI()
{
    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Titre
    QLabel *titleLabel = new QLabel("🤖 Interface Arduino - Gestion des Décisions");
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #8B5CF6; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // === Section Statut Arduino ===
    QGroupBox *statusGroup = new QGroupBox("📡 Statut de Connexion");
    statusGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px; "
        "}"
    );
    
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    QHBoxLayout *statusInfoLayout = new QHBoxLayout();
    
    QLabel *statusIconLabel = new QLabel("🔌");
    statusIconLabel->setStyleSheet("font-size: 24px;");
    statusInfoLayout->addWidget(statusIconLabel);
    
    QLabel *statusTextLabel = new QLabel("Arduino: Déconnecté");
    statusTextLabel->setObjectName("statusTextLabel");
    statusTextLabel->setStyleSheet(
        "font-size: 14px; "
        "color: #EF4444; "
        "padding: 10px; "
        "background-color: #FEE2E2; "
        "border-radius: 6px;"
    );
    statusInfoLayout->addWidget(statusTextLabel, 1);
    
    QPushButton *testBtn = new QPushButton("🔄 Tester Connexion");
    testBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #3B82F6; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "    background-color: #2563EB; "
        "}"
    );
    connect(testBtn, &QPushButton::clicked, this, &ArduinoPage::onTestConnectionClicked);
    statusInfoLayout->addWidget(testBtn);
    
    statusLayout->addLayout(statusInfoLayout);
    mainLayout->addWidget(statusGroup);
    
    // === Section Aperçu LCD ===
    QGroupBox *lcdGroup = new QGroupBox("📺 Aperçu LCD (16x2)");
    lcdGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
    );
    
    QVBoxLayout *lcdLayout = new QVBoxLayout(lcdGroup);
    
    // Simulateur LCD
    QWidget *lcdSimulator = new QWidget();
    lcdSimulator->setFixedSize(480, 120);
    lcdSimulator->setStyleSheet(
        "background-color: #1a472a; "
        "border: 3px solid #333; "
        "border-radius: 8px;"
    );
    
    QVBoxLayout *lcdSimLayout = new QVBoxLayout(lcdSimulator);
    lcdSimLayout->setSpacing(0);
    lcdSimLayout->setContentsMargins(10, 10, 10, 10);
    
    QLabel *lcdLine1 = new QLabel("Systeme Pret");
    lcdLine1->setObjectName("lcdLine1");
    lcdLine1->setStyleSheet(
        "color: #00ff00; "
        "font-family: 'Courier New', monospace; "
        "font-size: 16px; "
        "font-weight: bold; "
        "background-color: transparent;"
    );
    lcdSimLayout->addWidget(lcdLine1);
    
    QLabel *lcdLine2 = new QLabel("En attente...");
    lcdLine2->setObjectName("lcdLine2");
    lcdLine2->setStyleSheet(
        "color: #00ff00; "
        "font-family: 'Courier New', monospace; "
        "font-size: 16px; "
        "font-weight: bold; "
        "background-color: transparent;"
    );
    lcdSimLayout->addWidget(lcdLine2);
    
    lcdLayout->addWidget(lcdSimulator, 0, Qt::AlignCenter);
    mainLayout->addWidget(lcdGroup);
    
    // === Section Soumissions en Attente ===
    QGroupBox *submissionsGroup = new QGroupBox("📋 Soumissions en Attente de Décision");
    submissionsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
    );
    
    QVBoxLayout *submissionsLayout = new QVBoxLayout(submissionsGroup);
    
    QHBoxLayout *submissionsHeaderLayout = new QHBoxLayout();
    QLabel *submissionsLabel = new QLabel("Liste des soumissions à traiter:");
    submissionsLabel->setStyleSheet("font-size: 13px; color: #64748B;");
    submissionsHeaderLayout->addWidget(submissionsLabel);
    
    submissionsHeaderLayout->addStretch();
    
    QPushButton *refreshBtn = new QPushButton("🔄 Actualiser");
    refreshBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #10B981; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px 16px; "
        "    font-size: 12px; "
        "    font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "    background-color: #059669; "
        "}"
    );
    connect(refreshBtn, &QPushButton::clicked, this, &ArduinoPage::onRefreshClicked);
    submissionsHeaderLayout->addWidget(refreshBtn);
    
    submissionsLayout->addLayout(submissionsHeaderLayout);
    
    // Tableau des soumissions
    QTableWidget *submissionsTable = new QTableWidget();
    submissionsTable->setObjectName("submissionsTable");
    submissionsTable->setColumnCount(4);
    submissionsTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Article" << "Date Limite" << "État"
    );
    submissionsTable->setColumnWidth(0, 60);
    submissionsTable->setColumnWidth(1, 300);
    submissionsTable->setColumnWidth(2, 120);
    submissionsTable->setColumnWidth(3, 120);
    submissionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    submissionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    submissionsTable->setAlternatingRowColors(true);
    submissionsTable->setStyleSheet(
        "QTableWidget { "
        "    background-color: white; "
        "    border: 1px solid #E2E8F0; "
        "    border-radius: 6px; "
        "} "
        "QTableWidget::item { "
        "    padding: 8px; "
        "} "
        "QHeaderView::section { "
        "    background-color: #F8FAFC; "
        "    padding: 10px; "
        "    border: none; "
        "    font-weight: bold; "
        "    color: #475569; "
        "}"
    );
    submissionsLayout->addWidget(submissionsTable);
    
    mainLayout->addWidget(submissionsGroup);
    
    // === Section Instructions ===
    QGroupBox *instructionsGroup = new QGroupBox("ℹ️ Instructions");
    instructionsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
    );
    
    QVBoxLayout *instructionsLayout = new QVBoxLayout(instructionsGroup);
    
    QLabel *instructionsText = new QLabel(
        "<b>Comment utiliser:</b><br>"
        "1. Vérifiez que l'Arduino est connecté (statut vert)<br>"
        "2. Les soumissions en attente s'affichent dans le tableau<br>"
        "3. Utilisez le bouton physique Arduino:<br>"
        "   • <b>1 clic</b> = ACCEPTER la première soumission<br>"
        "   • <b>2 clics</b> = REJETER la première soumission<br>"
        "4. Le LCD affiche le résultat en temps réel<br>"
        "5. La décision est automatiquement enregistrée dans la base de données"
    );
    instructionsText->setWordWrap(true);
    instructionsText->setStyleSheet(
        "font-size: 12px; "
        "color: #475569; "
        "padding: 10px; "
        "background-color: #F8FAFC; "
        "border-radius: 6px; "
        "line-height: 1.6;"
    );
    instructionsLayout->addWidget(instructionsText);
    
    mainLayout->addWidget(instructionsGroup);
    
    // === Section Journal d'Activité ===
    QGroupBox *logGroup = new QGroupBox("📝 Journal d'Activité");
    logGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
    );
    
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    
    QTextEdit *logTextEdit = new QTextEdit();
    logTextEdit->setObjectName("logTextEdit");
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumHeight(150);
    logTextEdit->setStyleSheet(
        "QTextEdit { "
        "    background-color: #1E293B; "
        "    color: #E2E8F0; "
        "    border: 1px solid #475569; "
        "    border-radius: 6px; "
        "    padding: 10px; "
        "    font-family: 'Courier New', monospace; "
        "    font-size: 11px; "
        "}"
    );
    logLayout->addWidget(logTextEdit);
    
    mainLayout->addWidget(logGroup);
    
    setLayout(mainLayout);
    
    // Charger les soumissions initiales
    loadPendingSubmissions();
    addLogMessage("Interface Arduino initialisée", "info");
}

void ArduinoPage::setArduinoManager(ArduinoManager *manager)
{
    arduinoManager = manager;
    
    if (arduinoManager) {
        // Connecter les signaux
        connect(arduinoManager, &ArduinoManager::statusChanged,
                this, &ArduinoPage::onArduinoStatusChanged);
        connect(arduinoManager, &ArduinoManager::soumissionAcceptee,
                this, &ArduinoPage::onSoumissionAcceptee);
        connect(arduinoManager, &ArduinoManager::soumissionRejetee,
                this, &ArduinoPage::onSoumissionRejetee);
        
        // Mettre à jour le statut initial
        if (arduinoManager->isArduinoConnected()) {
            updateConnectionStatus(true);
            addLogMessage("Arduino connecté sur " + arduinoManager->getArduinoPort(), "success");
        } else {
            updateConnectionStatus(false);
            addLogMessage("Arduino non connecté", "error");
        }
    }
}

void ArduinoPage::loadPendingSubmissions()
{
    QTableWidget *table = findChild<QTableWidget*>("submissionsTable");
    if (!table) return;
    
    table->setRowCount(0);
    
    QSqlQuery query;
    query.prepare(
        "SELECT s.ID_soumission, a.titre, s.Date_lim_revision, s.Etat "
        "FROM SOUMISSION s "
        "LEFT JOIN ARTICLE a ON a.ID_soumission = s.ID_soumission "
        "WHERE (s.Etat = 'En révision' OR s.Etat = 'En attente') "
        "AND (s.Decision_finale IS NULL OR s.Decision_finale = 'En attente') "
        "ORDER BY s.Date_envoi ASC"
    );
    
    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            
            int id = query.value(0).toInt();
            QString titre = query.value(1).toString();
            QDate dateLimite = query.value(2).toDate();
            QString etat = query.value(3).toString();
            
            table->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
            table->setItem(row, 1, new QTableWidgetItem(titre));
            table->setItem(row, 2, new QTableWidgetItem(dateLimite.toString("dd/MM/yyyy")));
            table->setItem(row, 3, new QTableWidgetItem(etat));
            
            row++;
        }
        
        addLogMessage(QString("Chargé %1 soumission(s) en attente").arg(row), "info");
    } else {
        addLogMessage("Erreur lors du chargement des soumissions: " + query.lastError().text(), "error");
    }
}

void ArduinoPage::updateConnectionStatus(bool connected)
{
    QLabel *statusLabel = findChild<QLabel*>("statusTextLabel");
    if (!statusLabel) return;
    
    if (connected) {
        QString port = arduinoManager ? arduinoManager->getArduinoPort() : "COM?";
        statusLabel->setText("Arduino: Connecté sur " + port);
        statusLabel->setStyleSheet(
            "font-size: 14px; "
            "color: #10B981; "
            "padding: 10px; "
            "background-color: #D1FAE5; "
            "border-radius: 6px;"
        );
    } else {
        statusLabel->setText("Arduino: Déconnecté");
        statusLabel->setStyleSheet(
            "font-size: 14px; "
            "color: #EF4444; "
            "padding: 10px; "
            "background-color: #FEE2E2; "
            "border-radius: 6px;"
        );
    }
}

void ArduinoPage::addLogMessage(const QString &message, const QString &type)
{
    QTextEdit *logEdit = findChild<QTextEdit*>("logTextEdit");
    if (!logEdit) return;
    
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString color = "#E2E8F0"; // default
    QString icon = "•";
    
    if (type == "success") {
        color = "#10B981";
        icon = "✓";
    } else if (type == "error") {
        color = "#EF4444";
        icon = "✗";
    } else if (type == "warning") {
        color = "#F59E0B";
        icon = "⚠";
    }
    
    QString html = QString("<span style='color: #64748B;'>[%1]</span> "
                          "<span style='color: %2;'>%3</span> %4")
                   .arg(timestamp, color, icon, message);
    
    logEdit->append(html);
    
    // Auto-scroll vers le bas
    logEdit->moveCursor(QTextCursor::End);
}

void ArduinoPage::onArduinoStatusChanged(const QString &status)
{
    addLogMessage("Statut: " + status, "info");
    
    if (status.contains("connecté", Qt::CaseInsensitive)) {
        updateConnectionStatus(true);
    } else if (status.contains("déconnecté", Qt::CaseInsensitive)) {
        updateConnectionStatus(false);
    }
}

void ArduinoPage::onSoumissionAcceptee(int id)
{
    addLogMessage(QString("✅ Soumission #%1 ACCEPTÉE").arg(id), "success");
    
    // Mettre à jour l'aperçu LCD
    QLabel *line1 = findChild<QLabel*>("lcdLine1");
    QLabel *line2 = findChild<QLabel*>("lcdLine2");
    if (line1 && line2) {
        line1->setText("Article");
        line2->setText("ACCEPTE");
    }
    
    // Recharger les soumissions après 2 secondes
    QTimer::singleShot(2000, this, &ArduinoPage::onRefreshClicked);
}

void ArduinoPage::onSoumissionRejetee(int id)
{
    addLogMessage(QString("❌ Soumission #%1 REJETÉE").arg(id), "error");
    
    // Mettre à jour l'aperçu LCD
    QLabel *line1 = findChild<QLabel*>("lcdLine1");
    QLabel *line2 = findChild<QLabel*>("lcdLine2");
    if (line1 && line2) {
        line1->setText("Article");
        line2->setText("REJETE");
    }
    
    // Recharger les soumissions après 2 secondes
    QTimer::singleShot(2000, this, &ArduinoPage::onRefreshClicked);
}

void ArduinoPage::onRefreshClicked()
{
    addLogMessage("Actualisation des données...", "info");
    loadPendingSubmissions();
    
    // Réinitialiser l'aperçu LCD
    QLabel *line1 = findChild<QLabel*>("lcdLine1");
    QLabel *line2 = findChild<QLabel*>("lcdLine2");
    if (line1 && line2) {
        line1->setText("Systeme Pret");
        line2->setText("En attente...");
    }
}

void ArduinoPage::onTestConnectionClicked()
{
    addLogMessage("Test de connexion Arduino...", "info");
    
    if (arduinoManager) {
        if (arduinoManager->isArduinoConnected()) {
            addLogMessage("Arduino connecté et fonctionnel", "success");
            QMessageBox::information(this, "Test de Connexion",
                "✅ Arduino connecté avec succès!\n\n"
                "Port: " + arduinoManager->getArduinoPort());
        } else {
            addLogMessage("Arduino non connecté", "error");
            QMessageBox::warning(this, "Test de Connexion",
                "❌ Arduino non connecté.\n\n"
                "Vérifiez:\n"
                "• Le câble USB\n"
                "• Le port COM\n"
                "• Les drivers");
        }
    }
}

void ArduinoPage::updateLCDPreview()
{
    // Cette fonction peut être utilisée pour synchroniser l'aperçu LCD
    // avec l'état réel de l'Arduino si nécessaire
}
