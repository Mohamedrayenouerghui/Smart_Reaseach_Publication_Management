#include "mainwindow.h"
#include <QFile>
#include <QHeaderView>
#include <QIcon>
#include <QDebug>
#include <QDateTime>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadStyleSheet();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Set window properties
    setWindowTitle("Système de Gestion des Publications de Recherche");
    setMinimumSize(1200, 800);

    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main layout
    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create and add sidebar
    QWidget *sidebar = createSidebar();
    mainLayout->addWidget(sidebar);

    // Create and add content area
    QWidget *contentArea = createContentArea();
    mainLayout->addWidget(contentArea);
}

QWidget *MainWindow::createSidebar()
{
    QWidget *sidebar = new QWidget();
    sidebar->setMaximumWidth(280);
    sidebar->setMinimumWidth(280);
    sidebar->setStyleSheet(
        "QWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #f8fafc, stop:1 #f1f5f9);"
        "    border-right: 1px solid #e2e8f0;"
        "}"
    );

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);
    sidebarLayout->setSpacing(8);

    // App logo and title at top
    QLabel *appTitle = new QLabel(" Management System");
    appTitle->setStyleSheet(
        "QLabel {"
        "    color: #2d3748;"
        "    font-size: 20px;"
        "    font-weight: 600;"
        "    padding: 20px 0px 30px 0px;"
        "    text-align: center;"
        "    background-color: transparent;"
        "}"
    );
    appTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(appTitle);

    // Create sidebar buttons with professional text and logos
    accueilBtn = createSidebarButton("Accueil", ":/Acuell image.png");
    publicationsBtn = createSidebarButton("Publications", ":/publication logo.png");
    utilisateursBtn = createSidebarButton("Utilisateurs", ":/user logo.png");
    soumissionsBtn = createSidebarButton("Soumissions", ":/sumission logo.png");
    evaluationsBtn = createSidebarButton("Évaluations", ":/evaluation logo.png");
    conferencesBtn = createSidebarButton("Conférences", ":/conference logo.png");
    laboratoiresBtn = createSidebarButton("Laboratoires", ":/laboratoire logo.png");

    // Add buttons to layout
    sidebarLayout->addWidget(accueilBtn);
    sidebarLayout->addWidget(publicationsBtn);
    sidebarLayout->addWidget(utilisateursBtn);
    sidebarLayout->addWidget(soumissionsBtn);
    sidebarLayout->addWidget(evaluationsBtn);
    sidebarLayout->addWidget(conferencesBtn);
    sidebarLayout->addWidget(laboratoiresBtn);
    sidebarLayout->addStretch();

    // Connect signals
    connect(accueilBtn, &QPushButton::clicked, this, &MainWindow::onAccueilClicked);
    connect(publicationsBtn, &QPushButton::clicked, this, &MainWindow::onPublicationsClicked);
    connect(utilisateursBtn, &QPushButton::clicked, this, &MainWindow::onUtilisateursClicked);
    connect(soumissionsBtn, &QPushButton::clicked, this, &MainWindow::onSoumissionsClicked);
    connect(evaluationsBtn, &QPushButton::clicked, this, &MainWindow::onEvaluationsClicked);
    connect(conferencesBtn, &QPushButton::clicked, this, &MainWindow::onConferencesClicked);
    connect(laboratoiresBtn, &QPushButton::clicked, this, &MainWindow::onLaboratoiresClicked);

    // Set default selection
    accueilBtn->setChecked(true);

    return sidebar;
}

QPushButton *MainWindow::createSidebarButton(const QString &text, const QString &iconPath)
{
    QPushButton *btn = new QPushButton(text);
    btn->setMinimumHeight(56);
    btn->setMaximumHeight(56);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setCheckable(true);

    // Set icon if provided
    if (!iconPath.isEmpty()) {
        QIcon icon(iconPath);
        btn->setIcon(icon);
        btn->setIconSize(QSize(28, 28));
    }

    // Ultra-smooth professional styling with icon support
    btn->setStyleSheet(
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
        "    spacing: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(102, 126, 234, 0.1);"
        "    color: #667eea;"
        "}"
        "QPushButton:checked {"
        "    background-color: #667eea;"
        "    color: white;"
        "    font-weight: 600;"
        "}"
    );

    return btn;
}

QWidget *MainWindow::createContentArea()
{
    QWidget *contentArea = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // Header with smoother, more professional design
    headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ffffff, stop:1 #f8fafc);"
        "border-bottom: 1px solid #e2e8f0;"
    );
    headerWidget->setMinimumHeight(140);
    headerWidget->setMaximumHeight(140);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(30, 35, 30, 35);
    
    // Title
    titleLabel = new QLabel("Accueil");
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #2d3748;"
        "padding-bottom: 5px;"
        );
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    // Much bigger professional logo
    logoLabel = new QLabel();
    QPixmap logoPixmap(":/desktop logo.png");
    if (!logoPixmap.isNull()) {
        // Scale the logo much bigger for the header
        QPixmap scaledLogo = logoPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        logoLabel->setPixmap(scaledLogo);
        logoLabel->setStyleSheet(
            "background-color: transparent;"
            "border: none;"
            "padding: 5px;"
        );
    } else {
        // Fallback text if logo doesn't load
        logoLabel->setText("LOGO");
        logoLabel->setStyleSheet(
            "font-size: 26px;"
            "color: #667eea;"
            "background-color: transparent;"
            "font-weight: 600;"
            "border: 2px solid #667eea;"
            "border-radius: 8px;"
            "padding: 16px 20px;"
        );
    }
    headerLayout->addWidget(logoLabel);
    
    contentLayout->addWidget(headerWidget);

    // Create welcome page as default
    currentContentWidget = createWelcomePage();
    contentLayout->addWidget(currentContentWidget);

    return contentArea;
}

void MainWindow::loadStyleSheet()
{
    QFile styleFile(":/SpyBot.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        qDebug() << "Failed to load stylesheet";
    }
}

// Slot implementations
void MainWindow::onAccueilClicked()
{
    // Uncheck all other buttons
    publicationsBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    conferencesBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    accueilBtn->setChecked(true);
    titleLabel->setText("Accueil");
    // Add underline only for Accueil
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #2d3748;"
        "padding-bottom: 5px;"
    );
    switchToPage(createWelcomePage());
}

void MainWindow::onPublicationsClicked()
{
    accueilBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    conferencesBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    publicationsBtn->setChecked(true);
    titleLabel->setText("Gestion des Publications");
    // Add underline for Publications
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #667eea;"
        "padding-bottom: 5px;"
    );
    switchToPage(createPublicationsPage());
}

void MainWindow::onUtilisateursClicked()
{
    accueilBtn->setChecked(false);
    publicationsBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    conferencesBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    utilisateursBtn->setChecked(true);
    titleLabel->setText("Gestion des Utilisateurs");
    // Add underline for Utilisateurs
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #48bb78;"
        "padding-bottom: 5px;"
    );
    switchToPage(createUsersPage());
}

void MainWindow::onSoumissionsClicked()
{
    accueilBtn->setChecked(false);
    publicationsBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    conferencesBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    soumissionsBtn->setChecked(true);
    titleLabel->setText("Gestion des Soumissions");
    // Add underline for Soumissions
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #ed8936;"
        "padding-bottom: 5px;"
    );
    switchToPage(createSubmissionsPage());
}

void MainWindow::onEvaluationsClicked()
{
    accueilBtn->setChecked(false);
    publicationsBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    conferencesBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    evaluationsBtn->setChecked(true);
    titleLabel->setText("Gestion des Évaluations");
    // Add underline for Evaluations
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #9f7aea;"
        "padding-bottom: 5px;"
    );
    switchToPage(createEvaluationsPage());
}

void MainWindow::onConferencesClicked()
{
    accueilBtn->setChecked(false);
    publicationsBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    laboratoiresBtn->setChecked(false);

    conferencesBtn->setChecked(true);
    titleLabel->setText("Gestion des Conférences");
    // Add underline for Conferences
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #38b2ac;"
        "padding-bottom: 5px;"
    );
    switchToPage(createConferencesPage());
}

void MainWindow::onLaboratoiresClicked()
{
    accueilBtn->setChecked(false);
    publicationsBtn->setChecked(false);
    utilisateursBtn->setChecked(false);
    soumissionsBtn->setChecked(false);
    evaluationsBtn->setChecked(false);
    conferencesBtn->setChecked(false);

    laboratoiresBtn->setChecked(true);
    titleLabel->setText("Gestion des Laboratoires");
    // Add underline for Laboratories
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 600;"
        "color: #2d3748;"
        "background-color: transparent;"
        "border-bottom: 3px solid #e53e3e;"
        "padding-bottom: 5px;"
    );
    switchToPage(createLaboratoriesPage());
}

QWidget *MainWindow::createWelcomePage()
{
    QWidget *welcomePage = new QWidget();
    welcomePage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #667eea, stop:1 #764ba2);"
    );
    
    QVBoxLayout *layout = new QVBoxLayout(welcomePage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create a centered container for the logo
    QWidget *centerContainer = new QWidget();
    centerContainer->setStyleSheet("background-color: transparent;");
    
    QVBoxLayout *centerLayout = new QVBoxLayout(centerContainer);
    centerLayout->setContentsMargins(20, 20, 20, 20);
    centerLayout->setSpacing(30);
    centerLayout->setAlignment(Qt::AlignCenter);

    // Ubuntu-style MASSIVE logo - takes most of the window space
    QLabel *hugeLogo = new QLabel();
    QPixmap logoPixmap(":/logo.png");
    if (!logoPixmap.isNull()) {
        // Make it REALLY MASSIVE - up to 1200px for maximum impact
        int logoSize = qMin(1200, 900); // Max 1200px width, 900px height
        QPixmap scaledLogo = logoPixmap.scaled(logoSize, logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        hugeLogo->setPixmap(scaledLogo);
        hugeLogo->setAlignment(Qt::AlignCenter);
        hugeLogo->setStyleSheet(
            "background-color: transparent;"
            "border: none;"
            "padding: 0px;"
        );
    } else {
        // Fallback if logo doesn't load
        hugeLogo->setText("LOGO");
        hugeLogo->setStyleSheet(
            "font-size: 200px;"
            "font-weight: bold;"
            "color: rgba(255,255,255,0.9);"
            "text-align: center;"
            "background-color: transparent;"
            "border: none;"
            "padding: 40px;"
        );
        hugeLogo->setAlignment(Qt::AlignCenter);
    }
    
    centerLayout->addWidget(hugeLogo);

    // Enhanced welcome text below logo - smaller to emphasize logo
    QLabel *welcomeText = new QLabel("Système de Gestion des Publications de Recherche");
    welcomeText->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 300;"
        "color: rgba(255,255,255,0.95);"
        "text-align: center;"
        "background-color: transparent;"
        "margin-top: 20px;"
        "letter-spacing: 1px;"
    );
    welcomeText->setAlignment(Qt::AlignCenter);
    welcomeText->setWordWrap(true);
    centerLayout->addWidget(welcomeText);

    // Add a subtle version indicator
    QLabel *versionText = new QLabel("RaMeGaNe");
    versionText->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 300;"
        "color: rgba(255,255,255,0.7);"
        "text-align: center;"
        "background-color: transparent;"
        "margin-top: 10px;"
    );
    versionText->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(versionText);

    layout->addWidget(centerContainer);

    return welcomePage;
}

QWidget *MainWindow::createPublicationsPage()
{
    QWidget *publicationsPage = new QWidget();
    publicationsPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #f0f4ff, stop:1 #f7fafc);"
    );
    
    return publicationsPage;
}

QWidget *MainWindow::createUsersPage()
{
    QWidget *usersPage = new QWidget();
    usersPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #f0fff4, stop:1 #f7fafc);"
    );
    
    return usersPage;
}

QWidget *MainWindow::createSubmissionsPage()
{
    QWidget *submissionsPage = new QWidget();
    submissionsPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #fffaf0, stop:1 #f7fafc);"
    );
    
    return submissionsPage;
}

QWidget *MainWindow::createEvaluationsPage()
{
    QWidget *evaluationsPage = new QWidget();
    evaluationsPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #faf5ff, stop:1 #f7fafc);"
    );
    
    return evaluationsPage;
}

QWidget *MainWindow::createConferencesPage()
{
    QWidget *conferencesPage = new QWidget();
    conferencesPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #f0fdfa, stop:1 #f7fafc);"
    );
    
    return conferencesPage;
}

QWidget *MainWindow::createLaboratoriesPage()
{
    QWidget *laboratoriesPage = new QWidget();
    laboratoriesPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #fff5f5, stop:1 #f7fafc);"
    );
    
    return laboratoriesPage;
}

void MainWindow::switchToPage(QWidget *newPage)
{
    if (currentContentWidget) {
        currentContentWidget->setParent(nullptr);
        delete currentContentWidget;
    }
    
    currentContentWidget = newPage;
    
    // Add the new page to the content area (after the header)
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(centralWidget->layout()->itemAt(1)->widget()->layout());
    if (contentLayout) {
        contentLayout->addWidget(currentContentWidget);
    }
}