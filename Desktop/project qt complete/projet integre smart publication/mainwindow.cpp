#include "mainwindow.h"
#include "publicationspage.h"
#include "submission.h"
#include "conference.h"
#include "laboratoire.h"
#include "evaluation.h"
#include "users.h"
#include "login.h"
#include "arduinomanager.h"
#include "arduinopage.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QDialog>
#include <QGroupBox>
#include <QSqlQuery>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , publicationsPage(nullptr)
    , submissionPage(nullptr)
    , conferencePage(nullptr)
    , laboratoirePage(nullptr)
    , evaluationPage(nullptr)
    , arduinoPage(nullptr)
    , arduinoManager(nullptr)
    , arduinoStatusLabel(nullptr)
{
    ui->setupUi(this);
    initUserPage();
    loadStyleSheet();
    setupConnections();
    setupIcons();
    setupLogoutButton();
    setupArduino();
    setupPublicationsPage();
    setupSubmissionPage();
    setupConferencePage();
    setupLaboratoirePage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUserPage() {
    // 1. Create the instance
    Users *usersPageWidget = new Users(this);

    // 2. Swap the placeholder widget (Index 2) with your real Users class
    // This ensures that when you click the "Users" button, it shows your new UI
    QWidget *oldWidget = ui->stackedWidget->widget(2);
    ui->stackedWidget->removeWidget(oldWidget);
    ui->stackedWidget->insertWidget(2, usersPageWidget);

    if(oldWidget) oldWidget->deleteLater();

}

void MainWindow::loadStyleSheet()
{   
    QFile styleFile(":/SpyBot.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
    
    // Apply sidebar button styling
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #4a5568;"
        "    font-size: 15px;"
        "    font-weight: 500;"
        "    border: none;"
        "    text-align: left;"
        "    padding-left: 20px;"
        "    border-radius: 8px;"
        "    margin: 2px 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(102, 126, 234, 0.1);"
        "    color: #667eea;"
        "}"
        "QPushButton:checked {"
        "    background-color: #667eea;"
        "    color: white;"
        "    font-weight: 600;"
        "}";
    
    ui->accueilBtn->setStyleSheet(buttonStyle);
    ui->publicationsBtn->setStyleSheet(buttonStyle);
    ui->utilisateursBtn->setStyleSheet(buttonStyle);
    ui->soumissionsBtn->setStyleSheet(buttonStyle);
    ui->evaluationsBtn->setStyleSheet(buttonStyle);
    ui->conferencesBtn->setStyleSheet(buttonStyle);
    ui->laboratoiresBtn->setStyleSheet(buttonStyle);
}

void MainWindow::setupConnections()
{
    connect(ui->accueilBtn, &QPushButton::clicked, this, &MainWindow::onAccueilClicked);
    connect(ui->publicationsBtn, &QPushButton::clicked, this, &MainWindow::onPublicationsClicked);
    connect(ui->utilisateursBtn, &QPushButton::clicked, this, &MainWindow::onUtilisateursClicked);
    connect(ui->soumissionsBtn, &QPushButton::clicked, this, &MainWindow::onSoumissionsClicked);
    connect(ui->evaluationsBtn, &QPushButton::clicked, this, &MainWindow::onEvaluationsClicked);
    connect(ui->conferencesBtn, &QPushButton::clicked, this, &MainWindow::onConferencesClicked);
    connect(ui->laboratoiresBtn, &QPushButton::clicked, this, &MainWindow::onLaboratoiresClicked);
}

void MainWindow::setupIcons()
{
    // Set icons for sidebar buttons
    ui->accueilBtn->setIcon(QIcon(":/Acuell image.png"));
    ui->accueilBtn->setIconSize(QSize(28, 28));
    
    ui->publicationsBtn->setIcon(QIcon(":/publication logo.png"));
    ui->publicationsBtn->setIconSize(QSize(28, 28));
    
    ui->utilisateursBtn->setIcon(QIcon(":/user logo.png"));
    ui->utilisateursBtn->setIconSize(QSize(28, 28));
    
    ui->soumissionsBtn->setIcon(QIcon(":/sumission logo.png"));
    ui->soumissionsBtn->setIconSize(QSize(28, 28));
    
    ui->evaluationsBtn->setIcon(QIcon(":/evaluation logo.png"));
    ui->evaluationsBtn->setIconSize(QSize(28, 28));
    
    ui->conferencesBtn->setIcon(QIcon(":/conference logo.png"));
    ui->conferencesBtn->setIconSize(QSize(28, 28));
    
    ui->laboratoiresBtn->setIcon(QIcon(":/laboratoire logo.png"));
    ui->laboratoiresBtn->setIconSize(QSize(28, 28));
    
    // Set header logo
    QPixmap logoPixmap(":/desktop logo.png");
    if (!logoPixmap.isNull()) {
        QPixmap scaledLogo = logoPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->logoLabel->setPixmap(scaledLogo);
    }
    
    // Set huge welcome page logo
    QPixmap hugeLogo(":/logo.png");
    if (!hugeLogo.isNull()) {
        QPixmap scaledHugeLogo = hugeLogo.scaled(1200, 900, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->hugeLogo->setPixmap(scaledHugeLogo);
    }
}

void MainWindow::updateTitleUnderline(const QString &color)
{
    QString style = QString(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid %1;"
        "padding-bottom: 5px;"
    ).arg(color);
    
    ui->titleLabel->setStyleSheet(style);
}

void MainWindow::onAccueilClicked()
{
    ui->accueilBtn->setChecked(true);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Accueil");
    updateTitleUnderline("#2d3748");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPublicationsClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(true);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Gestion des Publications");
    updateTitleUnderline("#667eea");
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onUtilisateursClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(true);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Gestion des Utilisateurs");
    updateTitleUnderline("#48bb78");
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onSoumissionsClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(true);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Gestion des Soumissions");
    updateTitleUnderline("#ed8936");
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::onEvaluationsClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(true);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Gestion des Évaluations");
    updateTitleUnderline("#9f7aea");
    
    // Créer l'interface Evaluation seulement maintenant (à la demande)
    if (!evaluationPage) {
        setupEvaluationPage();
    }
    
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::onConferencesClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(true);
    ui->laboratoiresBtn->setChecked(false);
    
    ui->titleLabel->setText("Gestion des Conférences");
    updateTitleUnderline("#38b2ac");
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::onLaboratoiresClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(true);
    
    ui->titleLabel->setText("Gestion des Laboratoires");
    updateTitleUnderline("#e53e3e");
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::setupPublicationsPage()
{
    // Create the PublicationsPage instance
    publicationsPage = new PublicationsPage(this);
    
    // Get the publications page widget from stacked widget
    QWidget *pageWidget = ui->stackedWidget->widget(1);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the publications page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(publicationsPage);
    pageWidget->setLayout(layout);
}


void MainWindow::setupSubmissionPage()
{
    // Create the Submission instance
    submissionPage = new Submission(this);
    
    // Passer l'ArduinoManager à Submission
    if (arduinoManager) {
        submissionPage->setArduinoManager(arduinoManager);
    }
    
    // Get the submission page widget from stacked widget (index 3)
    QWidget *pageWidget = ui->stackedWidget->widget(3);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the submission page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(submissionPage);
    pageWidget->setLayout(layout);
}

void MainWindow::setupConferencePage()
{
    // Create the Conference instance
    conferencePage = new Conference(this);
    
    // Get the conference page widget from stacked widget (index 5)
    QWidget *pageWidget = ui->stackedWidget->widget(5);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the conference page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(conferencePage);
    pageWidget->setLayout(layout);
}

void MainWindow::setupLaboratoirePage()
{
    // Create the Laboratoire instance
    laboratoirePage = new Laboratoire(this);
    
    // Get the laboratoire page widget from stacked widget (index 6)
    QWidget *pageWidget = ui->stackedWidget->widget(6);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the laboratoire page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(laboratoirePage);
    pageWidget->setLayout(layout);
}

void MainWindow::setupEvaluationPage()
{
    // Create the Evaluation instance
    evaluationPage = new Evaluation(this);
    
    // Get the evaluation page widget from stacked widget (index 4)
    QWidget *pageWidget = ui->stackedWidget->widget(4);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the evaluation page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(evaluationPage);
    pageWidget->setLayout(layout);
}

void MainWindow::showArduinoInterface()
{
    // Créer un dialog simple pour afficher l'interface Arduino
    QDialog *arduinoDialog = new QDialog(this);
    arduinoDialog->setWindowTitle("Interface Arduino - Gestion des Décisions");
    arduinoDialog->setMinimumSize(800, 500);
    arduinoDialog->setStyleSheet("QDialog { background-color: #F5F7FA; }");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(arduinoDialog);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Titre
    QLabel *titleLabel = new QLabel("🤖 Interface Arduino");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #3B82F6; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Statut de connexion
    QGroupBox *statusGroup = new QGroupBox("📡 Statut de Connexion");
    statusGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding: 15px; "
        "    background-color: white; "
        "}"
    );
    
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    QLabel *statusLabel = new QLabel();
    if (arduinoManager && arduinoManager->isArduinoConnected()) {
        statusLabel->setText("✅ Arduino connecté sur " + arduinoManager->getArduinoPort());
        statusLabel->setStyleSheet(
            "font-size: 14px; "
            "color: #10B981; "
            "padding: 10px; "
            "background-color: #D1FAE5; "
            "border-radius: 6px;"
        );
    } else {
        statusLabel->setText("❌ Arduino non connecté");
        statusLabel->setStyleSheet(
            "font-size: 14px; "
            "color: #EF4444; "
            "padding: 10px; "
            "background-color: #FEE2E2; "
            "border-radius: 6px;"
        );
    }
    statusLayout->addWidget(statusLabel);
    mainLayout->addWidget(statusGroup);
    
    // Instructions
    QGroupBox *instructionsGroup = new QGroupBox("ℹ️ Instructions");
    instructionsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding: 15px; "
        "    background-color: white; "
        "}"
    );
    
    QVBoxLayout *instructionsLayout = new QVBoxLayout(instructionsGroup);
    
    QLabel *instructionsText = new QLabel(
        "<b>Comment utiliser l'Arduino:</b><br><br>"
        "1. Vérifiez que l'Arduino est connecté (statut vert ci-dessus)<br><br>"
        "2. Utilisez le bouton physique sur l'Arduino:<br>"
        "   • <b>1 clic</b> = ACCEPTER la première soumission en attente<br>"
        "   • <b>2 clics</b> = REJETER la première soumission en attente<br><br>"
        "3. Le LCD affiche le résultat en temps réel<br><br>"
        "4. La décision est automatiquement enregistrée dans la base de données<br><br>"
        "5. Retournez à la liste des soumissions pour voir les changements"
    );
    instructionsText->setWordWrap(true);
    instructionsText->setStyleSheet(
        "font-size: 13px; "
        "color: #475569; "
        "padding: 15px; "
        "background-color: #F8FAFC; "
        "border-radius: 6px; "
        "line-height: 1.8;"
    );
    instructionsLayout->addWidget(instructionsText);
    mainLayout->addWidget(instructionsGroup);
    
    // Soumissions en attente
    QGroupBox *submissionsGroup = new QGroupBox("📋 Soumissions en Attente");
    submissionsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    border: 2px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding: 15px; "
        "    background-color: white; "
        "}"
    );
    
    QVBoxLayout *submissionsLayout = new QVBoxLayout(submissionsGroup);
    
    // Compter les soumissions en attente
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM SOUMISSION "
        "WHERE (Etat = 'En révision' OR Etat = 'En attente') "
        "AND (Decision_finale IS NULL OR Decision_finale = 'En attente')"
    );
    
    int count = 0;
    if (query.exec() && query.next()) {
        count = query.value(0).toInt();
    }
    
    QLabel *countLabel = new QLabel(QString("Nombre de soumissions en attente: <b>%1</b>").arg(count));
    countLabel->setStyleSheet(
        "font-size: 14px; "
        "color: #1E293B; "
        "padding: 10px; "
        "background-color: #F1F5F9; "
        "border-radius: 6px;"
    );
    submissionsLayout->addWidget(countLabel);
    mainLayout->addWidget(submissionsGroup);
    
    // Bouton Fermer
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #64748B; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 12px 24px; "
        "    font-size: 14px; "
        "    font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "    background-color: #475569; "
        "}"
    );
    connect(closeBtn, &QPushButton::clicked, arduinoDialog, &QDialog::accept);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);
    
    arduinoDialog->setLayout(mainLayout);
    arduinoDialog->exec();
    delete arduinoDialog;
}

void MainWindow::setupArduinoPage()
{
    // Create the ArduinoPage instance
    arduinoPage = new ArduinoPage(this);
    
    // Passer l'ArduinoManager à ArduinoPage
    if (arduinoManager) {
        arduinoPage->setArduinoManager(arduinoManager);
    }
    
    // Get the arduino page widget from stacked widget (index 7)
    QWidget *pageWidget = ui->stackedWidget->widget(7);
    
    // Clear any existing layout
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    
    // Create a new layout and add the arduino page
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(arduinoPage);
    pageWidget->setLayout(layout);
}


void MainWindow::setupLogoutButton()
{
    // Créer un bouton de déconnexion dans la page d'accueil
    QWidget *accueilPage = ui->stackedWidget->widget(0);
    
    // Chercher s'il existe déjà un bouton de déconnexion
    QPushButton *logoutBtn = accueilPage->findChild<QPushButton*>("logoutBtn");
    
    if (!logoutBtn) {
        // Créer le bouton s'il n'existe pas
        logoutBtn = new QPushButton("🚪 Se Déconnecter", accueilPage);
        logoutBtn->setObjectName("logoutBtn");
        logoutBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #EF4444; "
            "    color: white; "
            "    border: none; "
            "    border-radius: 8px; "
            "    padding: 12px 24px; "
            "    font-size: 15px; "
            "    font-weight: 600; "
            "    font-family: 'Segoe UI', sans-serif; "
            "    min-width: 180px; "
            "} "
            "QPushButton:hover { "
            "    background-color: #DC2626; "
            "} "
            "QPushButton:pressed { "
            "    background-color: #B91C1C; "
            "}"
        );
        
        // Positionner le bouton en bas à droite de la page d'accueil
        logoutBtn->setGeometry(accueilPage->width() - 220, accueilPage->height() - 80, 200, 50);
        logoutBtn->show();
    }
    
    // Connecter le bouton
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
}

void MainWindow::onLogoutClicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Déconnexion");
    msgBox.setText("Êtes-vous sûr de vouloir vous déconnecter?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    
    msgBox.setStyleSheet(
        "QMessageBox { "
        "    background-color: white; "
        "    border-radius: 12px; "
        "} "
        "QMessageBox QLabel { "
        "    color: #1E293B; "
        "    font-size: 14px; "
        "    font-family: 'Segoe UI', sans-serif; "
        "    padding: 20px; "
        "    min-width: 300px; "
        "} "
        "QPushButton { "
        "    background-color: #8B5CF6; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px 20px; "
        "    font-size: 13px; "
        "    font-weight: 600; "
        "    min-width: 80px; "
        "    margin: 5px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #7C3AED; "
        "}"
    );
    
    QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);
    QAbstractButton *noBtn = msgBox.button(QMessageBox::No);
    
    if (yesBtn) {
        yesBtn->setText("Oui");
        yesBtn->setStyleSheet(
            "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #DC2626; }"
        );
    }
    if (noBtn) {
        noBtn->setText("Non");
        noBtn->setStyleSheet(
            "QPushButton { background-color: #64748B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #475569; }"
        );
    }
    
    int reply = msgBox.exec();
    
    if (reply == QMessageBox::Yes) {
        // Effacer les informations de l'utilisateur connecté
        Login::clearCurrentUser();
        
        // Réactiver toutes les interfaces (au cas où on était en mode relecteur)
        setRelecteurMode(false);
        
        // Cacher la fenêtre principale
        this->hide();
        
        // Créer et afficher la fenêtre de login
        Login *loginWindow = new Login();
        
        // Reconnecter le signal pour réafficher MainWindow après login
        QObject::connect(loginWindow, &Login::loginSuccessful, [this, loginWindow](const QString &username, const QString &role){
            qDebug() << "User logged in:" << username << "Role:" << role;
            
            // Configurer les permissions selon le rôle
            if (role == "relecteur") {
                setRelecteurMode(true);
            } else {
                setRelecteurMode(false);
            }
            
            loginWindow->close();
            loginWindow->deleteLater();
            this->show();
        });
        
        loginWindow->show();
    }
}


void MainWindow::setRelecteurMode(bool isRelecteur)
{
    if (isRelecteur) {
        // Bloquer tous les boutons sauf Évaluations et Accueil
        ui->publicationsBtn->setEnabled(false);
        ui->utilisateursBtn->setEnabled(false);
        ui->soumissionsBtn->setEnabled(false);
        ui->conferencesBtn->setEnabled(false);
        ui->laboratoiresBtn->setEnabled(false);
        
        // Garder Accueil et Évaluations actifs
        ui->accueilBtn->setEnabled(true);
        ui->evaluationsBtn->setEnabled(true);
        
        // Styliser les boutons désactivés
        QString disabledStyle = 
            "QPushButton {"
            "    background-color: transparent;"
            "    color: #CBD5E0;"
            "    font-size: 15px;"
            "    font-weight: 500;"
            "    border: none;"
            "    text-align: left;"
            "    padding-left: 20px;"
            "    border-radius: 8px;"
            "    margin: 2px 4px;"
            "}";
        
        ui->publicationsBtn->setStyleSheet(disabledStyle);
        ui->utilisateursBtn->setStyleSheet(disabledStyle);
        ui->soumissionsBtn->setStyleSheet(disabledStyle);
        ui->conferencesBtn->setStyleSheet(disabledStyle);
        ui->laboratoiresBtn->setStyleSheet(disabledStyle);
        
        // Rediriger automatiquement vers Évaluations
        onEvaluationsClicked();
        
        qDebug() << "Mode relecteur activé - Seules Accueil et Évaluations sont accessibles";
    } else {
        // Mode admin - tout est accessible
        ui->publicationsBtn->setEnabled(true);
        ui->utilisateursBtn->setEnabled(true);
        ui->soumissionsBtn->setEnabled(true);
        ui->conferencesBtn->setEnabled(true);
        ui->laboratoiresBtn->setEnabled(true);
        ui->accueilBtn->setEnabled(true);
        ui->evaluationsBtn->setEnabled(true);
        
        // Restaurer le style normal
        loadStyleSheet();
        
        qDebug() << "Mode admin activé - Toutes les interfaces sont accessibles";
    }
}


void MainWindow::setupArduino()
{
    // Créer le label de statut Arduino dans la barre de statut
    arduinoStatusLabel = new QLabel("Arduino: Déconnecté");
    arduinoStatusLabel->setStyleSheet(
        "QLabel { "
        "    color: #EF4444; "
        "    font-size: 12px; "
        "    padding: 5px 10px; "
        "    background-color: #FEE2E2; "
        "    border-radius: 4px; "
        "    margin: 2px; "
        "}"
    );
    statusBar()->addPermanentWidget(arduinoStatusLabel);
    
    // Créer le gestionnaire Arduino
    arduinoManager = new ArduinoManager(this);
    
    // Connecter les signaux
    connect(arduinoManager, &ArduinoManager::statusChanged,
            this, &MainWindow::updateArduinoStatus);
    connect(arduinoManager, &ArduinoManager::soumissionAcceptee,
            [this](int id) {
        qDebug() << "Soumission" << id << "acceptée - Recharger les données";
        // Recharger les données si on est sur la page soumissions
        if (submissionPage) {
            // submissionPage->loadSubmissionsFromDB();
        }
    });
    connect(arduinoManager, &ArduinoManager::soumissionRejetee,
            [this](int id) {
        qDebug() << "Soumission" << id << "rejetée - Recharger les données";
        // Recharger les données si on est sur la page soumissions
        if (submissionPage) {
            // submissionPage->loadSubmissionsFromDB();
        }
    });
    connect(arduinoManager, &ArduinoManager::errorOccurred,
            [this](const QString &error) {
        QMessageBox::warning(this, "Erreur Arduino", error);
    });
    
    // Initialiser la connexion Arduino
    if (arduinoManager->initialize()) {
        qDebug() << "Arduino initialisé avec succès";
    } else {
        qDebug() << "Échec de l'initialisation Arduino";
        QMessageBox::warning(this, "Arduino",
            "Impossible de se connecter à l'Arduino.\n\n"
            "Vérifiez que:\n"
            "- L'Arduino est branché via USB\n"
            "- Le bon port COM est sélectionné\n"
            "- Aucun autre programme n'utilise le port\n\n"
            "L'application continuera sans Arduino.");
    }
}

void MainWindow::updateArduinoStatus(const QString &status)
{
    if (arduinoStatusLabel) {
        arduinoStatusLabel->setText("Arduino: " + status);
        
        // Changer la couleur selon le statut
        if (status.contains("connecté", Qt::CaseInsensitive)) {
            arduinoStatusLabel->setStyleSheet(
                "QLabel { "
                "    color: #10B981; "
                "    font-size: 12px; "
                "    padding: 5px 10px; "
                "    background-color: #D1FAE5; "
                "    border-radius: 4px; "
                "    margin: 2px; "
                "}"
            );
        } else if (status.contains("acceptée", Qt::CaseInsensitive)) {
            arduinoStatusLabel->setStyleSheet(
                "QLabel { "
                "    color: #10B981; "
                "    font-size: 12px; "
                "    padding: 5px 10px; "
                "    background-color: #D1FAE5; "
                "    border-radius: 4px; "
                "    margin: 2px; "
                "}"
            );
        } else if (status.contains("rejetée", Qt::CaseInsensitive)) {
            arduinoStatusLabel->setStyleSheet(
                "QLabel { "
                "    color: #EF4444; "
                "    font-size: 12px; "
                "    padding: 5px 10px; "
                "    background-color: #FEE2E2; "
                "    border-radius: 4px; "
                "    margin: 2px; "
                "}"
            );
        } else {
            arduinoStatusLabel->setStyleSheet(
                "QLabel { "
                "    color: #F59E0B; "
                "    font-size: 12px; "
                "    padding: 5px 10px; "
                "    background-color: #FEF3C7; "
                "    border-radius: 4px; "
                "    margin: 2px; "
                "}"
            );
        }
    }
    
    qDebug() << "Statut Arduino mis à jour:" << status;
}
