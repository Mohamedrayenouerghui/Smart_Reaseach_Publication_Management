/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
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
    QLabel *welcomeText;
    QLabel *hugeLogo;
    QLabel *versionText;
    QWidget *publicationsPage;
    QTabWidget *tabWidget;
    QWidget *tab;
    QFrame *frame;
    QToolButton *addButton_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLabel *label;
    QWidget *tab_2;
    QFrame *frame_2;
    QComboBox *searchTypeCombo_2;
    QLabel *searchIcon_2;
    QLineEdit *searchInput_2;
    QToolButton *searchButton_3;
    QToolButton *sortButton_3;
    QToolButton *exportButton_2;
    QFrame *frame_3;
    QTableWidget *publicationsTable_2;
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
        publicationsBtn->setStyleSheet(QString::fromUtf8("color:black;"));
        publicationsBtn->setCheckable(true);

        sidebarLayout->addWidget(publicationsBtn);

        utilisateursBtn = new QPushButton(sidebar);
        utilisateursBtn->setObjectName("utilisateursBtn");
        utilisateursBtn->setMinimumSize(QSize(0, 56));
        utilisateursBtn->setMaximumSize(QSize(16777215, 56));
        utilisateursBtn->setStyleSheet(QString::fromUtf8("color:black;"));
        utilisateursBtn->setCheckable(true);

        sidebarLayout->addWidget(utilisateursBtn);

        soumissionsBtn = new QPushButton(sidebar);
        soumissionsBtn->setObjectName("soumissionsBtn");
        soumissionsBtn->setMinimumSize(QSize(0, 56));
        soumissionsBtn->setMaximumSize(QSize(16777215, 56));
        soumissionsBtn->setStyleSheet(QString::fromUtf8("color:black;"));
        soumissionsBtn->setCheckable(true);

        sidebarLayout->addWidget(soumissionsBtn);

        evaluationsBtn = new QPushButton(sidebar);
        evaluationsBtn->setObjectName("evaluationsBtn");
        evaluationsBtn->setMinimumSize(QSize(0, 56));
        evaluationsBtn->setMaximumSize(QSize(16777215, 56));
        evaluationsBtn->setStyleSheet(QString::fromUtf8("color:black;"));
        evaluationsBtn->setCheckable(true);

        sidebarLayout->addWidget(evaluationsBtn);

        conferencesBtn = new QPushButton(sidebar);
        conferencesBtn->setObjectName("conferencesBtn");
        conferencesBtn->setMinimumSize(QSize(0, 56));
        conferencesBtn->setMaximumSize(QSize(16777215, 56));
        conferencesBtn->setStyleSheet(QString::fromUtf8("color:black;"));
        conferencesBtn->setCheckable(true);

        sidebarLayout->addWidget(conferencesBtn);

        laboratoiresBtn = new QPushButton(sidebar);
        laboratoiresBtn->setObjectName("laboratoiresBtn");
        laboratoiresBtn->setMinimumSize(QSize(0, 56));
        laboratoiresBtn->setMaximumSize(QSize(16777215, 56));
        laboratoiresBtn->setStyleSheet(QString::fromUtf8("color:black;"));
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

        hugeLogo = new QLabel(centerContainer);
        hugeLogo->setObjectName("hugeLogo");
        hugeLogo->setMinimumSize(QSize(600, 400));
        hugeLogo->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"border: none;\n"
"padding: 0px;"));
        hugeLogo->setScaledContents(false);
        hugeLogo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        centerLayout->addWidget(hugeLogo);

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
        tabWidget = new QTabWidget(publicationsPage);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 10, 1031, 601));
        tabWidget->setStyleSheet(QString::fromUtf8("color:black;"));
        tab = new QWidget();
        tab->setObjectName("tab");
        frame = new QFrame(tab);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(-10, -40, 1001, 601));
        frame->setStyleSheet(QString::fromUtf8("\n"
"    background-color: #F5F7FA;\n"
"    border-bottom: 1px solid #E0E0E0;\n"
"\n"
""));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        addButton_2 = new QToolButton(frame);
        addButton_2->setObjectName("addButton_2");
        addButton_2->setGeometry(QRect(630, 490, 221, 51));
        addButton_2->setMinimumSize(QSize(120, 38));
        addButton_2->setMaximumSize(QSize(500, 500));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setWeight(QFont::DemiBold);
        addButton_2->setFont(font);
        addButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        addButton_2->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #E0B0FF;\n"
"    border: 2px solid #E0B0FF;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 18px;\n"
"    font-weight: 600;\n"
"   \n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #E0B0FF;\n"
"    color: white;\n"
"    border-color: #E0B0FF;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #059669;\n"
"    border-color: #E0B0FF;\n"
"}"));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(100, 160, 63, 20));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(410, 170, 63, 20));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 260, 111, 20));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(710, 180, 121, 20));
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(360, 260, 141, 20));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8("color:#6C63FF\n"
""));
        label_7 = new QLabel(frame);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(90, 370, 81, 21));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(720, 270, 71, 20));
        label_8->setFont(font1);
        label_8->setStyleSheet(QString::fromUtf8("color:#6C63FF"));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(640, 220, 231, 28));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(30, 310, 231, 28));
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(310, 310, 251, 28));
        lineEdit_4 = new QLineEdit(frame);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(630, 310, 241, 28));
        lineEdit_5 = new QLineEdit(frame);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(50, 400, 241, 28));
        lineEdit_6 = new QLineEdit(frame);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(320, 210, 231, 28));
        lineEdit_7 = new QLineEdit(frame);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(40, 210, 231, 28));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 69, 361, 61));
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(true);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color:#98568D;"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        frame_2 = new QFrame(tab_2);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(0, 0, 1001, 81));
        frame_2->setStyleSheet(QString::fromUtf8(" background-color: #FFFFFF;\n"
"    border-bottom: 1px solid #E0E0E0;"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        searchTypeCombo_2 = new QComboBox(frame_2);
        searchTypeCombo_2->addItem(QString());
        searchTypeCombo_2->addItem(QString());
        searchTypeCombo_2->setObjectName("searchTypeCombo_2");
        searchTypeCombo_2->setGeometry(QRect(90, 20, 140, 40));
        searchTypeCombo_2->setMinimumSize(QSize(140, 40));
        searchTypeCombo_2->setMaximumSize(QSize(140, 40));
        searchTypeCombo_2->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    padding: 8px 12px;\n"
"    background: #FFFFFF;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 13px;\n"
"    color: #475569;\n"
"}\n"
"QComboBox:hover {\n"
"    border-color: #C7D2FE;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #6366F1;\n"
"    background-color: #FAFBFF;\n"
"}"));
        searchIcon_2 = new QLabel(frame_2);
        searchIcon_2->setObjectName("searchIcon_2");
        searchIcon_2->setGeometry(QRect(20, 30, 24, 24));
        searchIcon_2->setMinimumSize(QSize(24, 24));
        searchIcon_2->setMaximumSize(QSize(24, 24));
        searchIcon_2->setStyleSheet(QString::fromUtf8("font-size: 18px; background: transparent; color: #64748B;"));
        searchIcon_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        searchInput_2 = new QLineEdit(frame_2);
        searchInput_2->setObjectName("searchInput_2");
        searchInput_2->setGeometry(QRect(260, 20, 331, 40));
        searchInput_2->setMinimumSize(QSize(0, 40));
        searchInput_2->setMaximumSize(QSize(16777215, 40));
        searchInput_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"    background: #FFFFFF;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    color: #475569;\n"
"}\n"
"QLineEdit:hover {\n"
"    border-color: #C7D2FE;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #6366F1;\n"
"    background-color: #FAFBFF;\n"
"}"));
        searchButton_3 = new QToolButton(frame_2);
        searchButton_3->setObjectName("searchButton_3");
        searchButton_3->setGeometry(QRect(620, 20, 110, 38));
        searchButton_3->setMinimumSize(QSize(110, 38));
        searchButton_3->setMaximumSize(QSize(110, 38));
        searchButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        searchButton_3->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #3B82F6;\n"
"    border: 2px solid #3B82F6;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 0px 12px;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #3B82F6;\n"
"    color: white;\n"
"    border-color: #3B82F6;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #2563EB;\n"
"    border-color: #2563EB;\n"
"}"));
        sortButton_3 = new QToolButton(frame_2);
        sortButton_3->setObjectName("sortButton_3");
        sortButton_3->setGeometry(QRect(760, 20, 100, 38));
        sortButton_3->setMinimumSize(QSize(100, 38));
        sortButton_3->setMaximumSize(QSize(100, 38));
        sortButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        sortButton_3->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #8B5CF6;\n"
"    border: 2px solid #8B5CF6;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 0px 12px;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #8B5CF6;\n"
"    color: white;\n"
"    border-color: #8B5CF6;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #7C3AED;\n"
"    border-color: #7C3AED;\n"
"}"));
        sortButton_3->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
        exportButton_2 = new QToolButton(frame_2);
        exportButton_2->setObjectName("exportButton_2");
        exportButton_2->setGeometry(QRect(880, 20, 100, 38));
        exportButton_2->setMinimumSize(QSize(100, 38));
        exportButton_2->setMaximumSize(QSize(100, 38));
        exportButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        exportButton_2->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #F59E0B;\n"
"    border: 2px solid #F59E0B;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 0px 12px;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #F59E0B;\n"
"    color: white;\n"
"    border-color: #F59E0B;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #D97706;\n"
"    border-color: #D97706;\n"
"}"));
        exportButton_2->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
        frame_3 = new QFrame(tab_2);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(0, 80, 1001, 471));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;\n"
""));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        publicationsTable_2 = new QTableWidget(frame_3);
        if (publicationsTable_2->columnCount() < 8)
            publicationsTable_2->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        publicationsTable_2->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        publicationsTable_2->setObjectName("publicationsTable_2");
        publicationsTable_2->setGeometry(QRect(0, 0, 1001, 401));
        publicationsTable_2->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #FFFFFF;\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 12px;\n"
"    gridline-color: #F1F5F9;\n"
"    font-family: 'Segoe UI', 'Roboto', sans-serif;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 14px;\n"
"    border-bottom: 1px solid #F1F5F9;\n"
"    color: #475569;\n"
"    font-size: 13px;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #F1F5F9;\n"
"    color: #1E293B;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #F8FAFC;\n"
"    color: #64748B;\n"
"    padding: 16px;\n"
"    border: none;\n"
"    border-bottom: 2px solid #E2E8F0;\n"
"    border-right: 1px solid #F1F5F9;\n"
"    font-weight: 600;\n"
"    font-size: 12px;\n"
"    text-transform: uppercase;\n"
"    font-family: 'Segoe UI', 'Roboto', sans-serif;\n"
"    letter-spacing: 0.8px;\n"
"}"));
        publicationsTable_2->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        publicationsTable_2->setAlternatingRowColors(true);
        publicationsTable_2->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        publicationsTable_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        publicationsTable_2->horizontalHeader()->setStretchLastSection(true);
        publicationsTable_2->verticalHeader()->setVisible(false);
        tabWidget->addTab(tab_2, QString());
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

        stackedWidget->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);


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
        welcomeText->setText(QCoreApplication::translate("MainWindow", "Syst\303\250me de Gestion des Publications de Recherche", nullptr));
        hugeLogo->setText(QString());
        versionText->setText(QCoreApplication::translate("MainWindow", "RaMeGaNe", nullptr));
        addButton_2->setText(QCoreApplication::translate("MainWindow", "+ Ajouter la conference", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Date_fin ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Date_d\303\251but", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Frais_inscription", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Theme", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Gestion des conferences", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "from", nullptr));
        searchTypeCombo_2->setItemText(0, QCoreApplication::translate("MainWindow", "Par Titre", nullptr));
        searchTypeCombo_2->setItemText(1, QCoreApplication::translate("MainWindow", "Par Domaine", nullptr));

        searchIcon_2->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215", nullptr));
        searchInput_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez votre recherche...", nullptr));
        searchButton_3->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        sortButton_3->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        exportButton_2->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
        QTableWidgetItem *___qtablewidgetitem = publicationsTable_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID_conference", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = publicationsTable_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = publicationsTable_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = publicationsTable_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date_debut", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = publicationsTable_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Date_fin ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = publicationsTable_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Frais_inscription ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = publicationsTable_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = publicationsTable_2->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Th\303\251me", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "conference", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
