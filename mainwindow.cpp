#include "mainwindow.h"
#include "publicationspage.h"
#include "submission.h"
#include "conference.h"
#include "laboratoire.h"
#include "arduinomonitor.h"
#include "users.h"
#include "login.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , publicationsPage(nullptr)
    , submissionPage(nullptr)
    , conferencePage(nullptr)
    , laboratoirePage(nullptr)
    , arduinoPage(nullptr)
{
    ui->setupUi(this);
    initUserPage();
    loadStyleSheet();
    setupConnections();
    setupIcons();
    setupPublicationsPage();
    setupSubmissionPage();
    setupConferencePage();
    setupLaboratoirePage();
    setupArduinoPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUserPage()
{
    Users *usersPageWidget = new Users(this);
    QWidget *oldWidget = ui->stackedWidget->widget(2);
    ui->stackedWidget->removeWidget(oldWidget);
    ui->stackedWidget->insertWidget(2, usersPageWidget);
    if (oldWidget) oldWidget->deleteLater();
}

void MainWindow::loadStyleSheet()
{
    QFile styleFile(":/SpyBot.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }

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
    connect(ui->accueilBtn,      &QPushButton::clicked, this, &MainWindow::onAccueilClicked);
    connect(ui->publicationsBtn, &QPushButton::clicked, this, &MainWindow::onPublicationsClicked);
    connect(ui->utilisateursBtn, &QPushButton::clicked, this, &MainWindow::onUtilisateursClicked);
    connect(ui->soumissionsBtn,  &QPushButton::clicked, this, &MainWindow::onSoumissionsClicked);
    connect(ui->evaluationsBtn,  &QPushButton::clicked, this, &MainWindow::onEvaluationsClicked);
    connect(ui->conferencesBtn,  &QPushButton::clicked, this, &MainWindow::onConferencesClicked);
    connect(ui->laboratoiresBtn, &QPushButton::clicked, this, &MainWindow::onLaboratoiresClicked);
}

void MainWindow::setupIcons()
{
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

    QPixmap logoPixmap(":/desktop logo.png");
    if (!logoPixmap.isNull()) {
        ui->logoLabel->setPixmap(
            logoPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QPixmap hugeLogo(":/logo.png");
    if (!hugeLogo.isNull()) {
        ui->hugeLogo->setPixmap(
            hugeLogo.scaled(1200, 900, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::updateTitleUnderline(const QString &color)
{
    ui->titleLabel->setStyleSheet(QString(
        "font-size: 28px; font-weight: 600; color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid %1; padding-bottom: 5px;").arg(color));
}

// ── Sidebar navigation ────────────────────────────────────────────

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
    ui->titleLabel->setText("Gestion des Evaluations");
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
    ui->titleLabel->setText("Gestion des Conferences");
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

void MainWindow::onArduinoClicked()
{
    ui->accueilBtn->setChecked(false);
    ui->publicationsBtn->setChecked(false);
    ui->utilisateursBtn->setChecked(false);
    ui->soumissionsBtn->setChecked(false);
    ui->evaluationsBtn->setChecked(false);
    ui->conferencesBtn->setChecked(false);
    ui->laboratoiresBtn->setChecked(false);
    ui->titleLabel->setText("Arduino - Moniteur Capteur");
    updateTitleUnderline("#0EA5E9");
    ui->stackedWidget->setCurrentIndex(7);
}

// ── Page setup helpers ────────────────────────────────────────────

static void embedWidget(QStackedWidget *stack, int index, QWidget *widget)
{
    QWidget *pageWidget = stack->widget(index);
    if (!pageWidget) return;
    if (pageWidget->layout()) {
        QLayoutItem *item;
        while ((item = pageWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pageWidget->layout();
    }
    QVBoxLayout *layout = new QVBoxLayout(pageWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widget);
    pageWidget->setLayout(layout);
}

void MainWindow::setupPublicationsPage()
{
    publicationsPage = new PublicationsPage(this);
    embedWidget(ui->stackedWidget, 1, publicationsPage);
}

void MainWindow::setupSubmissionPage()
{
    submissionPage = new Submission(this);
    embedWidget(ui->stackedWidget, 3, submissionPage);
}

void MainWindow::setupConferencePage()
{
    conferencePage = new Conference(this);
    embedWidget(ui->stackedWidget, 5, conferencePage);
}

void MainWindow::setupLaboratoirePage()
{
    laboratoirePage = new Laboratoire(this);
    embedWidget(ui->stackedWidget, 6, laboratoirePage);
}

void MainWindow::setupArduinoPage()
{
    arduinoPage = new ArduinoMonitor(this);
    embedWidget(ui->stackedWidget, 7, arduinoPage);
}
