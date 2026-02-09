/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *appTitle;
    QPushButton *accueilBtn;
    QPushButton *publicationsBtn;
    QPushButton *utilisateursBtn;
    QPushButton *soumissionsBtn;
    QPushButton *evaluationsBtn;
    QPushButton *conferencesBtn;
    QPushButton *laboratoiresBtn;
    QSpacerItem *verticalSpacer;
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QWidget *headerWidget;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *logoLabel;
    QStackedWidget *stackedWidget;
    QWidget *accueilPage;
    QVBoxLayout *accueilLayout;
    QWidget *centerContainer;
    QVBoxLayout *centerLayout;
    QLabel *hugeLogo;
    QLabel *welcomeText;
    QLabel *versionText;
    QWidget *publicationsPage;
    QWidget *utilisateursPage;
    QWidget *soumissionsPage;
    QWidget *evaluationsPage;
    QWidget *conferencesPage;
    QWidget *laboratoiresPage;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        MainWindow->setMinimumSize(QSize(1200, 800));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(280, 0));
        sidebar->setMaximumSize(QSize(280, 16777215));
        sidebar->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \n"
"    stop:0 #f8fafc, stop:1 #f1f5f9);\n"
"    border-right: 1px solid #e2e8f0;\n"
"}"));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(8);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(20, 30, 20, 30);
        appTitle = new QLabel(sidebar);
        appTitle->setObjectName("appTitle");
        appTitle->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #2d3748;\n"
"    font-size: 20px;\n"
"    font-weight: 600;\n"
"    padding: 20px 0px 30px 0px;\n"
"    text-align: center;\n"
"    background-color: transparent;\n"
"}"));
        appTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sidebarLayout->addWidget(appTitle);

        accueilBtn = new QPushButton(sidebar);
        accueilBtn->setObjectName("accueilBtn");
        accueilBtn->setMinimumSize(QSize(0, 56));
        accueilBtn->setMaximumSize(QSize(16777215, 56));
        accueilBtn->setCheckable(true);
        accueilBtn->setChecked(true);

        sidebarLayout->addWidget(accueilBtn);

        publicationsBtn = new QPushButton(sidebar);
        publicationsBtn->setObjectName("publicationsBtn");
        publicationsBtn->setMinimumSize(QSize(0, 56));
        publicationsBtn->setMaximumSize(QSize(16777215, 56));
        publicationsBtn->setCheckable(true);

        sidebarLayout->addWidget(publicationsBtn);

        utilisateursBtn = new QPushButton(sidebar);
        utilisateursBtn->setObjectName("utilisateursBtn");
        utilisateursBtn->setMinimumSize(QSize(0, 56));
        utilisateursBtn->setMaximumSize(QSize(16777215, 56));
        utilisateursBtn->setCheckable(true);

        sidebarLayout->addWidget(utilisateursBtn);

        soumissionsBtn = new QPushButton(sidebar);
        soumissionsBtn->setObjectName("soumissionsBtn");
        soumissionsBtn->setMinimumSize(QSize(0, 56));
        soumissionsBtn->setMaximumSize(QSize(16777215, 56));
        soumissionsBtn->setCheckable(true);

        sidebarLayout->addWidget(soumissionsBtn);

        evaluationsBtn = new QPushButton(sidebar);
        evaluationsBtn->setObjectName("evaluationsBtn");
        evaluationsBtn->setMinimumSize(QSize(0, 56));
        evaluationsBtn->setMaximumSize(QSize(16777215, 56));
        evaluationsBtn->setCheckable(true);

        sidebarLayout->addWidget(evaluationsBtn);

        conferencesBtn = new QPushButton(sidebar);
        conferencesBtn->setObjectName("conferencesBtn");
        conferencesBtn->setMinimumSize(QSize(0, 56));
        conferencesBtn->setMaximumSize(QSize(16777215, 56));
        conferencesBtn->setCheckable(true);

        sidebarLayout->addWidget(conferencesBtn);

        laboratoiresBtn = new QPushButton(sidebar);
        laboratoiresBtn->setObjectName("laboratoiresBtn");
        laboratoiresBtn->setMinimumSize(QSize(0, 56));
        laboratoiresBtn->setMaximumSize(QSize(16777215, 56));
        laboratoiresBtn->setCheckable(true);

        sidebarLayout->addWidget(laboratoiresBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(verticalSpacer);


        mainLayout->addWidget(sidebar);

        contentArea = new QWidget(centralwidget);
        contentArea->setObjectName("contentArea");
        contentLayout = new QVBoxLayout(contentArea);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        headerWidget = new QWidget(contentArea);
        headerWidget->setObjectName("headerWidget");
        headerWidget->setMinimumSize(QSize(0, 140));
        headerWidget->setMaximumSize(QSize(16777215, 140));
        headerWidget->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \n"
"stop:0 #ffffff, stop:1 #f8fafc);\n"
"border-bottom: 1px solid #e2e8f0;"));
        headerLayout = new QHBoxLayout(headerWidget);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(30, 35, 30, 35);
        titleLabel = new QLabel(headerWidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"font-weight: 600;\n"
"color: #2d3748;\n"
"background-color: transparent;\n"
"border-bottom: 3px solid #2d3748;\n"
"padding-bottom: 5px;"));

        headerLayout->addWidget(titleLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(horizontalSpacer);

        logoLabel = new QLabel(headerWidget);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(140, 140));
        logoLabel->setMaximumSize(QSize(140, 140));
        logoLabel->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"border: none;\n"
"padding: 5px;"));
        logoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        headerLayout->addWidget(logoLabel);


        contentLayout->addWidget(headerWidget);

        stackedWidget = new QStackedWidget(contentArea);
        stackedWidget->setObjectName("stackedWidget");
        accueilPage = new QWidget();
        accueilPage->setObjectName("accueilPage");
        accueilPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #667eea, stop:1 #764ba2);"));
        accueilLayout = new QVBoxLayout(accueilPage);
        accueilLayout->setSpacing(0);
        accueilLayout->setObjectName("accueilLayout");
        accueilLayout->setContentsMargins(0, 0, 0, 0);
        centerContainer = new QWidget(accueilPage);
        centerContainer->setObjectName("centerContainer");
        centerContainer->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        centerLayout = new QVBoxLayout(centerContainer);
        centerLayout->setSpacing(30);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setContentsMargins(20, 20, 20, 20);
        hugeLogo = new QLabel(centerContainer);
        hugeLogo->setObjectName("hugeLogo");
        hugeLogo->setMinimumSize(QSize(600, 400));
        hugeLogo->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"border: none;\n"
"padding: 0px;"));
        hugeLogo->setScaledContents(false);
        hugeLogo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        centerLayout->addWidget(hugeLogo);

        welcomeText = new QLabel(centerContainer);
        welcomeText->setObjectName("welcomeText");
        welcomeText->setStyleSheet(QString::fromUtf8("font-size: 22px;\n"
"font-weight: 300;\n"
"color: rgba(255,255,255,0.95);\n"
"text-align: center;\n"
"background-color: transparent;\n"
"margin-top: 20px;\n"
"letter-spacing: 1px;"));
        welcomeText->setAlignment(Qt::AlignmentFlag::AlignCenter);
        welcomeText->setWordWrap(true);

        centerLayout->addWidget(welcomeText);

        versionText = new QLabel(centerContainer);
        versionText->setObjectName("versionText");
        versionText->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"font-weight: 300;\n"
"color: rgba(255,255,255,0.7);\n"
"text-align: center;\n"
"background-color: transparent;\n"
"margin-top: 10px;"));
        versionText->setAlignment(Qt::AlignmentFlag::AlignCenter);

        centerLayout->addWidget(versionText);


        accueilLayout->addWidget(centerContainer);

        stackedWidget->addWidget(accueilPage);
        publicationsPage = new QWidget();
        publicationsPage->setObjectName("publicationsPage");
        publicationsPage->setStyleSheet(QString::fromUtf8("background-color: #f5f5f5;"));
        stackedWidget->addWidget(publicationsPage);
        utilisateursPage = new QWidget();
        utilisateursPage->setObjectName("utilisateursPage");
        utilisateursPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #f0fff4, stop:1 #f7fafc);"));
        stackedWidget->addWidget(utilisateursPage);
        soumissionsPage = new QWidget();
        soumissionsPage->setObjectName("soumissionsPage");
        soumissionsPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #fffaf0, stop:1 #f7fafc);"));
        stackedWidget->addWidget(soumissionsPage);
        evaluationsPage = new QWidget();
        evaluationsPage->setObjectName("evaluationsPage");
        evaluationsPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #faf5ff, stop:1 #f7fafc);"));
        stackedWidget->addWidget(evaluationsPage);
        conferencesPage = new QWidget();
        conferencesPage->setObjectName("conferencesPage");
        conferencesPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #f0fdfa, stop:1 #f7fafc);"));
        stackedWidget->addWidget(conferencesPage);
        laboratoiresPage = new QWidget();
        laboratoiresPage->setObjectName("laboratoiresPage");
        laboratoiresPage->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \n"
"stop:0 #fff5f5, stop:1 #f7fafc);"));
        stackedWidget->addWidget(laboratoiresPage);

        contentLayout->addWidget(stackedWidget);


        mainLayout->addWidget(contentArea);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Syst\303\250me de Gestion des Publications de Recherche", nullptr));
        appTitle->setText(QCoreApplication::translate("MainWindow", " Management System", nullptr));
        accueilBtn->setText(QCoreApplication::translate("MainWindow", "Accueil", nullptr));
        publicationsBtn->setText(QCoreApplication::translate("MainWindow", "Publications", nullptr));
        utilisateursBtn->setText(QCoreApplication::translate("MainWindow", "Utilisateurs", nullptr));
        soumissionsBtn->setText(QCoreApplication::translate("MainWindow", "Soumissions", nullptr));
        evaluationsBtn->setText(QCoreApplication::translate("MainWindow", "\303\211valuations", nullptr));
        conferencesBtn->setText(QCoreApplication::translate("MainWindow", "Conf\303\251rences", nullptr));
        laboratoiresBtn->setText(QCoreApplication::translate("MainWindow", "Laboratoires", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Accueil", nullptr));
        logoLabel->setText(QCoreApplication::translate("MainWindow", "LOGO", nullptr));
        hugeLogo->setText(QString());
        welcomeText->setText(QCoreApplication::translate("MainWindow", "Syst\303\250me de Gestion des Publications de Recherche", nullptr));
        versionText->setText(QCoreApplication::translate("MainWindow", "RaMeGaNe", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
