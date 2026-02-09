/********************************************************************************
** Form generated from reading UI file 'publicationspage.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUBLICATIONSPAGE_H
#define UI_PUBLICATIONSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PublicationsPage
{
public:
    QVBoxLayout *mainLayout;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QToolButton *addButton;
    QSpacerItem *horizontalSpacer;
    QToolButton *searchButton;
    QToolButton *sortButton;
    QToolButton *exportButton;
    QToolButton *statsButton;
    QWidget *searchBar;
    QHBoxLayout *searchBarLayout;
    QLabel *searchIcon;
    QComboBox *searchTypeCombo;
    QLineEdit *searchInput;
    QToolButton *clearButton;
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QTabWidget *publicationsTabWidget;
    QWidget *listTab;
    QVBoxLayout *listTabLayout;
    QTableWidget *publicationsTable;
    QWidget *addTab;
    QVBoxLayout *addTabLayout;
    QLabel *formTitleLabel;
    QWidget *formWidget;
    QVBoxLayout *formLayout;
    QLabel *titreLabel;
    QLineEdit *titreInput;
    QLabel *domaineLabel;
    QComboBox *domaineCombo;
    QLabel *contenuLabel;
    QWidget *uploadWidget;
    QVBoxLayout *uploadLayout;
    QPushButton *uploadButton;
    QLabel *fileNameLabel;
    QWidget *buttonWidget;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;
    QPushButton *submitButton;

    void setupUi(QWidget *PublicationsPage)
    {
        if (PublicationsPage->objectName().isEmpty())
            PublicationsPage->setObjectName("PublicationsPage");
        PublicationsPage->resize(920, 660);
        PublicationsPage->setStyleSheet(QString::fromUtf8("QWidget#PublicationsPage {\n"
"    background-color: #F5F7FA;\n"
"}"));
        mainLayout = new QVBoxLayout(PublicationsPage);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        toolbar = new QWidget(PublicationsPage);
        toolbar->setObjectName("toolbar");
        toolbar->setMinimumSize(QSize(0, 70));
        toolbar->setMaximumSize(QSize(16777215, 70));
        toolbar->setStyleSheet(QString::fromUtf8("QWidget#toolbar {\n"
"    background-color: #FFFFFF;\n"
"    border-bottom: 1px solid #E0E0E0;\n"
"}"));
        toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setSpacing(12);
        toolbarLayout->setObjectName("toolbarLayout");
        toolbarLayout->setContentsMargins(24, 16, 24, 16);
        addButton = new QToolButton(toolbar);
        addButton->setObjectName("addButton");
        addButton->setMinimumSize(QSize(120, 38));
        addButton->setMaximumSize(QSize(120, 38));
        addButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        addButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #10B981;\n"
"    border: 2px solid #10B981;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 0px 16px;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #10B981;\n"
"    color: white;\n"
"    border-color: #10B981;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #059669;\n"
"    border-color: #059669;\n"
"}"));

        toolbarLayout->addWidget(addButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);

        searchButton = new QToolButton(toolbar);
        searchButton->setObjectName("searchButton");
        searchButton->setMinimumSize(QSize(110, 38));
        searchButton->setMaximumSize(QSize(110, 38));
        searchButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        searchButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(searchButton);

        sortButton = new QToolButton(toolbar);
        sortButton->setObjectName("sortButton");
        sortButton->setMinimumSize(QSize(100, 38));
        sortButton->setMaximumSize(QSize(100, 38));
        sortButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        sortButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        sortButton->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(sortButton);

        exportButton = new QToolButton(toolbar);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(100, 38));
        exportButton->setMaximumSize(QSize(100, 38));
        exportButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        exportButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        exportButton->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(exportButton);

        statsButton = new QToolButton(toolbar);
        statsButton->setObjectName("statsButton");
        statsButton->setMinimumSize(QSize(110, 38));
        statsButton->setMaximumSize(QSize(110, 38));
        statsButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        statsButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: transparent;\n"
"    color: #14B8A6;\n"
"    border: 2px solid #14B8A6;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 0px 12px;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #14B8A6;\n"
"    color: white;\n"
"    border-color: #14B8A6;\n"
"}\n"
"QToolButton:pressed {\n"
"    background-color: #0D9488;\n"
"    border-color: #0D9488;\n"
"}"));

        toolbarLayout->addWidget(statsButton);


        mainLayout->addWidget(toolbar);

        searchBar = new QWidget(PublicationsPage);
        searchBar->setObjectName("searchBar");
        searchBar->setMinimumSize(QSize(0, 65));
        searchBar->setMaximumSize(QSize(16777215, 65));
        searchBar->setStyleSheet(QString::fromUtf8("QWidget#searchBar {\n"
"    background-color: #FFFFFF;\n"
"    border-bottom: 1px solid #E0E0E0;\n"
"}"));
        searchBarLayout = new QHBoxLayout(searchBar);
        searchBarLayout->setSpacing(12);
        searchBarLayout->setObjectName("searchBarLayout");
        searchBarLayout->setContentsMargins(24, 12, 24, 12);
        searchIcon = new QLabel(searchBar);
        searchIcon->setObjectName("searchIcon");
        searchIcon->setMinimumSize(QSize(24, 24));
        searchIcon->setMaximumSize(QSize(24, 24));
        searchIcon->setStyleSheet(QString::fromUtf8("font-size: 18px; background: transparent; color: #64748B;"));
        searchIcon->setAlignment(Qt::AlignmentFlag::AlignCenter);

        searchBarLayout->addWidget(searchIcon);

        searchTypeCombo = new QComboBox(searchBar);
        searchTypeCombo->addItem(QString());
        searchTypeCombo->addItem(QString());
        searchTypeCombo->setObjectName("searchTypeCombo");
        searchTypeCombo->setMinimumSize(QSize(140, 40));
        searchTypeCombo->setMaximumSize(QSize(140, 40));
        searchTypeCombo->setStyleSheet(QString::fromUtf8("QComboBox {\n"
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

        searchBarLayout->addWidget(searchTypeCombo);

        searchInput = new QLineEdit(searchBar);
        searchInput->setObjectName("searchInput");
        searchInput->setMinimumSize(QSize(0, 40));
        searchInput->setMaximumSize(QSize(16777215, 40));
        searchInput->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
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

        searchBarLayout->addWidget(searchInput);

        clearButton = new QToolButton(searchBar);
        clearButton->setObjectName("clearButton");
        clearButton->setMinimumSize(QSize(40, 40));
        clearButton->setMaximumSize(QSize(40, 40));
        clearButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        clearButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    background-color: #FEF2F2;\n"
"    color: #EF4444;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"QToolButton:hover {\n"
"    background-color: #EF4444;\n"
"    color: white;\n"
"}"));

        searchBarLayout->addWidget(clearButton);


        mainLayout->addWidget(searchBar);

        contentArea = new QWidget(PublicationsPage);
        contentArea->setObjectName("contentArea");
        contentArea->setStyleSheet(QString::fromUtf8("QWidget#contentArea {\n"
"    background-color: #F5F7FA;\n"
"}"));
        contentLayout = new QVBoxLayout(contentArea);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(16, 16, 16, 16);
        publicationsTabWidget = new QTabWidget(contentArea);
        publicationsTabWidget->setObjectName("publicationsTabWidget");
        publicationsTabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 12px;\n"
"    background-color: #FFFFFF;\n"
"    top: -1px;\n"
"}\n"
"QTabBar::tab {\n"
"    background-color: #F8FAFC;\n"
"    color: #64748B;\n"
"    padding: 12px 24px;\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    margin-right: 4px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background-color: #FFFFFF;\n"
"    color: #667eea;\n"
"    border-bottom: 2px solid #667eea;\n"
"}\n"
"QTabBar::tab:hover:!selected {\n"
"    background-color: #EEF2FF;\n"
"    color: #667eea;\n"
"}"));
        listTab = new QWidget();
        listTab->setObjectName("listTab");
        listTabLayout = new QVBoxLayout(listTab);
        listTabLayout->setSpacing(0);
        listTabLayout->setObjectName("listTabLayout");
        listTabLayout->setContentsMargins(0, 0, 0, 0);
        publicationsTable = new QTableWidget(listTab);
        if (publicationsTable->columnCount() < 6)
            publicationsTable->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        publicationsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        publicationsTable->setObjectName("publicationsTable");
        publicationsTable->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #FFFFFF;\n"
"    border: none;\n"
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
        publicationsTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        publicationsTable->setAlternatingRowColors(true);
        publicationsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        publicationsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        publicationsTable->horizontalHeader()->setStretchLastSection(true);
        publicationsTable->verticalHeader()->setVisible(false);

        listTabLayout->addWidget(publicationsTable);

        publicationsTabWidget->addTab(listTab, QString());
        addTab = new QWidget();
        addTab->setObjectName("addTab");
        addTabLayout = new QVBoxLayout(addTab);
        addTabLayout->setSpacing(12);
        addTabLayout->setObjectName("addTabLayout");
        addTabLayout->setContentsMargins(30, 20, 30, 20);
        formTitleLabel = new QLabel(addTab);
        formTitleLabel->setObjectName("formTitleLabel");
        formTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: 600; color: #1E293B; background: transparent;"));

        addTabLayout->addWidget(formTitleLabel);

        formWidget = new QWidget(addTab);
        formWidget->setObjectName("formWidget");
        formWidget->setStyleSheet(QString::fromUtf8("QWidget#formWidget {\n"
"    background-color: transparent;\n"
"}"));
        formLayout = new QVBoxLayout(formWidget);
        formLayout->setSpacing(12);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        titreLabel = new QLabel(formWidget);
        titreLabel->setObjectName("titreLabel");
        titreLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; font-weight: 600; color: #475569; background: transparent;"));

        formLayout->addWidget(titreLabel);

        titreInput = new QLineEdit(formWidget);
        titreInput->setObjectName("titreInput");
        titreInput->setMinimumSize(QSize(0, 38));
        titreInput->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    padding: 12px 16px;\n"
"    background: #FFFFFF;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    color: #475569;\n"
"}\n"
"QLineEdit:hover {\n"
"    border-color: #C7D2FE;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #667eea;\n"
"    background-color: #FAFBFF;\n"
"}"));

        formLayout->addWidget(titreInput);

        domaineLabel = new QLabel(formWidget);
        domaineLabel->setObjectName("domaineLabel");
        domaineLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; font-weight: 600; color: #475569; background: transparent;"));

        formLayout->addWidget(domaineLabel);

        domaineCombo = new QComboBox(formWidget);
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->addItem(QString());
        domaineCombo->setObjectName("domaineCombo");
        domaineCombo->setMinimumSize(QSize(0, 38));
        domaineCombo->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 1.5px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    padding: 12px 16px;\n"
"    background: #FFFFFF;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 14px;\n"
"    color: #475569;\n"
"}\n"
"QComboBox:hover {\n"
"    border-color: #C7D2FE;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #667eea;\n"
"    background-color: #FAFBFF;\n"
"}\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    padding-right: 10px;\n"
"}\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 6px solid #64748B;\n"
"    margin-right: 8px;\n"
"}"));

        formLayout->addWidget(domaineCombo);

        contenuLabel = new QLabel(formWidget);
        contenuLabel->setObjectName("contenuLabel");
        contenuLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; font-weight: 600; color: #475569; background: transparent;"));

        formLayout->addWidget(contenuLabel);

        uploadWidget = new QWidget(formWidget);
        uploadWidget->setObjectName("uploadWidget");
        uploadWidget->setMinimumSize(QSize(0, 100));
        uploadWidget->setStyleSheet(QString::fromUtf8("QWidget#uploadWidget {\n"
"    border: 2px dashed #C7D2FE;\n"
"    border-radius: 12px;\n"
"    background-color: #F8FAFC;\n"
"}"));
        uploadLayout = new QVBoxLayout(uploadWidget);
        uploadLayout->setSpacing(8);
        uploadLayout->setObjectName("uploadLayout");
        uploadButton = new QPushButton(uploadWidget);
        uploadButton->setObjectName("uploadButton");
        uploadButton->setMinimumSize(QSize(160, 36));
        uploadButton->setMaximumSize(QSize(160, 36));
        uploadButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        uploadButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #667eea;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 13px;\n"
"    font-weight: 600;\n"
"    padding: 8px 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #5568d3;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #4c51bf;\n"
"}"));

        uploadLayout->addWidget(uploadButton, 0, Qt::AlignmentFlag::AlignHCenter);

        fileNameLabel = new QLabel(uploadWidget);
        fileNameLabel->setObjectName("fileNameLabel");
        fileNameLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #64748B; background: transparent;"));
        fileNameLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        uploadLayout->addWidget(fileNameLabel);


        formLayout->addWidget(uploadWidget);

        buttonWidget = new QWidget(formWidget);
        buttonWidget->setObjectName("buttonWidget");
        buttonWidget->setStyleSheet(QString::fromUtf8("QWidget#buttonWidget {\n"
"    background-color: transparent;\n"
"}"));
        buttonLayout = new QHBoxLayout(buttonWidget);
        buttonLayout->setSpacing(16);
        buttonLayout->setObjectName("buttonLayout");
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer_2);

        cancelButton = new QPushButton(buttonWidget);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(100, 40));
        cancelButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        cancelButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #64748B;\n"
"    border: 2px solid #E2E8F0;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 13px;\n"
"    font-weight: 600;\n"
"    padding: 8px 20px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #F8FAFC;\n"
"    border-color: #CBD5E1;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #F1F5F9;\n"
"}"));

        buttonLayout->addWidget(cancelButton);

        submitButton = new QPushButton(buttonWidget);
        submitButton->setObjectName("submitButton");
        submitButton->setMinimumSize(QSize(140, 40));
        submitButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        submitButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #10B981;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 13px;\n"
"    font-weight: 600;\n"
"    padding: 8px 20px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #059669;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #047857;\n"
"}"));

        buttonLayout->addWidget(submitButton);


        formLayout->addWidget(buttonWidget);


        addTabLayout->addWidget(formWidget);

        publicationsTabWidget->addTab(addTab, QString());

        contentLayout->addWidget(publicationsTabWidget);


        mainLayout->addWidget(contentArea);


        retranslateUi(PublicationsPage);

        publicationsTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PublicationsPage);
    } // setupUi

    void retranslateUi(QWidget *PublicationsPage)
    {
        PublicationsPage->setWindowTitle(QCoreApplication::translate("PublicationsPage", "Publications", nullptr));
        addButton->setText(QCoreApplication::translate("PublicationsPage", "+ Ajouter", nullptr));
        searchButton->setText(QCoreApplication::translate("PublicationsPage", "Rechercher", nullptr));
        sortButton->setText(QCoreApplication::translate("PublicationsPage", "Trier", nullptr));
        exportButton->setText(QCoreApplication::translate("PublicationsPage", "Exporter", nullptr));
        statsButton->setText(QCoreApplication::translate("PublicationsPage", "Statistiques", nullptr));
        searchIcon->setText(QCoreApplication::translate("PublicationsPage", "\360\237\224\215", nullptr));
        searchTypeCombo->setItemText(0, QCoreApplication::translate("PublicationsPage", "Par Titre", nullptr));
        searchTypeCombo->setItemText(1, QCoreApplication::translate("PublicationsPage", "Par Domaine", nullptr));

        searchInput->setPlaceholderText(QCoreApplication::translate("PublicationsPage", "Entrez votre recherche...", nullptr));
        clearButton->setText(QCoreApplication::translate("PublicationsPage", "\342\234\225", nullptr));
        QTableWidgetItem *___qtablewidgetitem = publicationsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("PublicationsPage", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = publicationsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("PublicationsPage", "Titre", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = publicationsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("PublicationsPage", "Domaine", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = publicationsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("PublicationsPage", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = publicationsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("PublicationsPage", "Date Cr\303\251ation", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = publicationsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("PublicationsPage", "Actions", nullptr));
        publicationsTabWidget->setTabText(publicationsTabWidget->indexOf(listTab), QCoreApplication::translate("PublicationsPage", "Liste des Publications", nullptr));
        formTitleLabel->setText(QCoreApplication::translate("PublicationsPage", "Ajouter un Nouvel Article", nullptr));
        titreLabel->setText(QCoreApplication::translate("PublicationsPage", "Titre de l'Article", nullptr));
        titreInput->setPlaceholderText(QCoreApplication::translate("PublicationsPage", "Entrez le titre de l'article...", nullptr));
        domaineLabel->setText(QCoreApplication::translate("PublicationsPage", "Domaine", nullptr));
        domaineCombo->setItemText(0, QCoreApplication::translate("PublicationsPage", "S\303\251lectionnez un domaine...", nullptr));
        domaineCombo->setItemText(1, QCoreApplication::translate("PublicationsPage", "Intelligence Artificielle", nullptr));
        domaineCombo->setItemText(2, QCoreApplication::translate("PublicationsPage", "R\303\251seaux et S\303\251curit\303\251", nullptr));
        domaineCombo->setItemText(3, QCoreApplication::translate("PublicationsPage", "G\303\251nie Logiciel", nullptr));
        domaineCombo->setItemText(4, QCoreApplication::translate("PublicationsPage", "Syst\303\250mes Distribu\303\251s", nullptr));
        domaineCombo->setItemText(5, QCoreApplication::translate("PublicationsPage", "Base de Donn\303\251es", nullptr));
        domaineCombo->setItemText(6, QCoreApplication::translate("PublicationsPage", "Autre", nullptr));

        contenuLabel->setText(QCoreApplication::translate("PublicationsPage", "Contenu (PDF)", nullptr));
        uploadButton->setText(QCoreApplication::translate("PublicationsPage", "Choisir un fichier PDF", nullptr));
        fileNameLabel->setText(QCoreApplication::translate("PublicationsPage", "Aucun fichier s\303\251lectionn\303\251", nullptr));
        cancelButton->setText(QCoreApplication::translate("PublicationsPage", "Annuler", nullptr));
        submitButton->setText(QCoreApplication::translate("PublicationsPage", "Soumettre l'Article", nullptr));
        publicationsTabWidget->setTabText(publicationsTabWidget->indexOf(addTab), QCoreApplication::translate("PublicationsPage", "Ajouter un Nouvel Article", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PublicationsPage: public Ui_PublicationsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUBLICATIONSPAGE_H
