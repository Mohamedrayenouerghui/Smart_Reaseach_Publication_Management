#include "mainwindow.h"
#include "publicationspage.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , publicationsPage(nullptr)
{
    ui->setupUi(this);
    loadStyleSheet();
    setupConnections();
    setupIcons();
    setupPublicationsPage();
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
