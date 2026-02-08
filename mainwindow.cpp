#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_publicationspage.h"
#include "ui_userspage.h"
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadStyleSheet();
    setupConnections();
    setupIcons();
    setupPublicationsPage();
    setupUsersPage();

}

MainWindow::~MainWindow()
{
    delete ui;
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
    // Load the Publications UI file
    Ui::PublicationsPage publicationsUi;
    QWidget *publicationsPage = ui->stackedWidget->widget(1);
    
    // Clear any existing layout
    if (publicationsPage->layout()) {
        QLayoutItem *item;
        while ((item = publicationsPage->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete publicationsPage->layout();
    }
    
    // Setup UI from the .ui file
    publicationsUi.setupUi(publicationsPage);
    
    // Set column widths for the table
    publicationsUi.publicationsTable->setColumnWidth(0, 60);
    publicationsUi.publicationsTable->setColumnWidth(1, 400);
    publicationsUi.publicationsTable->setColumnWidth(2, 120);
    publicationsUi.publicationsTable->setColumnWidth(3, 120);
    publicationsUi.publicationsTable->setColumnWidth(4, 130);
    publicationsUi.publicationsTable->setColumnWidth(5, 150);
    
    // Set row height
    publicationsUi.publicationsTable->verticalHeader()->setDefaultSectionSize(60);
    
    // ===== ADD MENUS TO BUTTONS =====
    
    // Sort button menu
    QMenu *sortMenu = new QMenu(publicationsUi.sortButton);
    sortMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #F3E8FF; "
        "    color: #8B5CF6; "
        "}"
    );
    sortMenu->addAction("Par Date");
    sortMenu->addAction("Par Titre");
    publicationsUi.sortButton->setMenu(sortMenu);
    
    // Export button menu
    QMenu *exportMenu = new QMenu(publicationsUi.exportButton);
    exportMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #FEF3C7; "
        "    color: #F59E0B; "
        "}"
    );
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Word");
    publicationsUi.exportButton->setMenu(exportMenu);
    
    // ===== CONNECT BUTTON SIGNALS =====
    
    // Add button
    connect(publicationsUi.addButton, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Ajouter Publication", "Formulaire d'ajout de publication à implémenter");
    });
    
    // Statistics button
    connect(publicationsUi.statsButton, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
    });
    
    // Clear button - clears the search input
    connect(publicationsUi.clearButton, &QToolButton::clicked, publicationsUi.searchInput, &QLineEdit::clear);
    
    // Sort menu actions
    connect(sortMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Trier", "Tri par: " + action->text());
    });
    
    // Export menu actions
    connect(exportMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Exporter", action->text());
    });
}

void MainWindow::setupUsersPage()
{
    // Load the Users UI file
    Ui::UsersPage usersUi;
    QWidget *usersPage = ui->stackedWidget->widget(2);

    // Clear any existing layout
    if (usersPage->layout()) {
        QLayoutItem *item;
        while ((item = usersPage->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete usersPage->layout();
    }

    // Setup UI from the .ui file
    usersUi.setupUi(usersPage);

    // Set column widths for the table
    usersUi.usersTable->setColumnWidth(0, 150);
    usersUi.usersTable->setColumnWidth(1, 150);
    usersUi.usersTable->setColumnWidth(2, 150);
    usersUi.usersTable->setColumnWidth(3, 200);
    usersUi.usersTable->setColumnWidth(4, 150);
    usersUi.usersTable->setColumnWidth(5, 150);
    usersUi.usersTable->setColumnWidth(5, 150);

    // Set row height
    usersUi.usersTable->verticalHeader()->setDefaultSectionSize(60);

    // ===== ADD MENUS TO BUTTONS =====

    // Sort button menu
    QMenu *sortMenu = new QMenu(usersUi.sortButtonUser);
    sortMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #F3E8FF; "
        "    color: #8B5CF6; "
        "}"
        );
    sortMenu->addAction("Par Expertise");
    sortMenu->addAction("Par Role");
    usersUi.sortButtonUser->setMenu(sortMenu);

    // Export button menu
    QMenu *exportMenu = new QMenu(usersUi.exportButtonUser);
    exportMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #FEF3C7; "
        "    color: #F59E0B; "
        "}"
        );
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Exel");
    usersUi.exportButtonUser->setMenu(exportMenu);

    // ===== CONNECT BUTTON SIGNALS =====

    // Add button
    connect(usersUi.addButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Ajouter Utilisateur", "Formulaire d'ajout d'utilisateur à implémenter");
    });

    // Statistics button
    connect(usersUi.statsButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
    });

    // Clear button - clears the search input
    connect(usersUi.clearButtonUser, &QToolButton::clicked, usersUi.searchInputUser, &QLineEdit::clear);

    // Sort menu actions
    connect(sortMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Trier", "Tri par: " + action->text() + " à implémenter");
    });

    // Export menu actions
    connect(exportMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Exporter", action->text() + " à implémenter");
    });
}
