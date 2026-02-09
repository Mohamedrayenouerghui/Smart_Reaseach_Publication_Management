/********************************************************************************
** Form generated from reading UI file 'conference.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFERENCE_H
#define UI_CONFERENCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Conference
{
public:
    QVBoxLayout *mainLayout;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QToolButton *addButtonConference;
    QSpacerItem *horizontalSpacer;
    QToolButton *searchButtonConference;
    QToolButton *sortButtonConference;
    QToolButton *exportButtonConference;
    QToolButton *statsButtonConference;
    QWidget *searchBar;
    QHBoxLayout *searchBarLayout;
    QLabel *searchIconConference;
    QComboBox *searchTypeComboConference;
    QLineEdit *searchInputConference;
    QToolButton *clearButtonConference;
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QTableWidget *conferenceTable;

    void setupUi(QWidget *Conference)
    {
        if (Conference->objectName().isEmpty())
            Conference->setObjectName("Conference");
        Conference->resize(920, 660);
        Conference->setStyleSheet(QString::fromUtf8("QWidget#Conference {\n"
"    background-color: #F5F7FA;\n"
"}"));
        mainLayout = new QVBoxLayout(Conference);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        toolbar = new QWidget(Conference);
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
        addButtonConference = new QToolButton(toolbar);
        addButtonConference->setObjectName("addButtonConference");
        addButtonConference->setMinimumSize(QSize(120, 38));
        addButtonConference->setMaximumSize(QSize(120, 38));
        addButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        addButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(addButtonConference);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);

        searchButtonConference = new QToolButton(toolbar);
        searchButtonConference->setObjectName("searchButtonConference");
        searchButtonConference->setMinimumSize(QSize(110, 38));
        searchButtonConference->setMaximumSize(QSize(110, 38));
        searchButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        searchButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(searchButtonConference);

        sortButtonConference = new QToolButton(toolbar);
        sortButtonConference->setObjectName("sortButtonConference");
        sortButtonConference->setMinimumSize(QSize(100, 38));
        sortButtonConference->setMaximumSize(QSize(100, 38));
        sortButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        sortButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        sortButtonConference->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(sortButtonConference);

        exportButtonConference = new QToolButton(toolbar);
        exportButtonConference->setObjectName("exportButtonConference");
        exportButtonConference->setMinimumSize(QSize(100, 38));
        exportButtonConference->setMaximumSize(QSize(100, 38));
        exportButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        exportButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        exportButtonConference->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(exportButtonConference);

        statsButtonConference = new QToolButton(toolbar);
        statsButtonConference->setObjectName("statsButtonConference");
        statsButtonConference->setMinimumSize(QSize(110, 38));
        statsButtonConference->setMaximumSize(QSize(110, 38));
        statsButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        statsButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(statsButtonConference);


        mainLayout->addWidget(toolbar);

        searchBar = new QWidget(Conference);
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
        searchIconConference = new QLabel(searchBar);
        searchIconConference->setObjectName("searchIconConference");
        searchIconConference->setMinimumSize(QSize(24, 24));
        searchIconConference->setMaximumSize(QSize(24, 24));
        searchIconConference->setStyleSheet(QString::fromUtf8("font-size: 18px; background: transparent; color: #64748B;"));
        searchIconConference->setAlignment(Qt::AlignmentFlag::AlignCenter);

        searchBarLayout->addWidget(searchIconConference);

        searchTypeComboConference = new QComboBox(searchBar);
        searchTypeComboConference->addItem(QString());
        searchTypeComboConference->addItem(QString());
        searchTypeComboConference->addItem(QString());
        searchTypeComboConference->addItem(QString());
        searchTypeComboConference->setObjectName("searchTypeComboConference");
        searchTypeComboConference->setMinimumSize(QSize(140, 40));
        searchTypeComboConference->setMaximumSize(QSize(140, 40));
        searchTypeComboConference->setStyleSheet(QString::fromUtf8("QComboBox {\n"
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

        searchBarLayout->addWidget(searchTypeComboConference);

        searchInputConference = new QLineEdit(searchBar);
        searchInputConference->setObjectName("searchInputConference");
        searchInputConference->setMinimumSize(QSize(0, 40));
        searchInputConference->setMaximumSize(QSize(16777215, 40));
        searchInputConference->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
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

        searchBarLayout->addWidget(searchInputConference);

        clearButtonConference = new QToolButton(searchBar);
        clearButtonConference->setObjectName("clearButtonConference");
        clearButtonConference->setMinimumSize(QSize(40, 40));
        clearButtonConference->setMaximumSize(QSize(40, 40));
        clearButtonConference->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        clearButtonConference->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        searchBarLayout->addWidget(clearButtonConference);


        mainLayout->addWidget(searchBar);

        contentArea = new QWidget(Conference);
        contentArea->setObjectName("contentArea");
        contentArea->setStyleSheet(QString::fromUtf8("QWidget#contentArea {\n"
"    background-color: #F5F7FA;\n"
"}"));
        contentLayout = new QVBoxLayout(contentArea);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(16, 16, 16, 16);
        conferenceTable = new QTableWidget(contentArea);
        if (conferenceTable->columnCount() < 9)
            conferenceTable->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        conferenceTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        conferenceTable->setObjectName("conferenceTable");
        conferenceTable->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
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
        conferenceTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        conferenceTable->setAlternatingRowColors(true);
        conferenceTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        conferenceTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        conferenceTable->horizontalHeader()->setStretchLastSection(true);
        conferenceTable->verticalHeader()->setVisible(false);

        contentLayout->addWidget(conferenceTable);


        mainLayout->addWidget(contentArea);


        retranslateUi(Conference);

        QMetaObject::connectSlotsByName(Conference);
    } // setupUi

    void retranslateUi(QWidget *Conference)
    {
        Conference->setWindowTitle(QCoreApplication::translate("Conference", "Conf\303\251rences", nullptr));
        addButtonConference->setText(QCoreApplication::translate("Conference", "+ Ajouter", nullptr));
        searchButtonConference->setText(QCoreApplication::translate("Conference", "Rechercher", nullptr));
        sortButtonConference->setText(QCoreApplication::translate("Conference", "Trier", nullptr));
        exportButtonConference->setText(QCoreApplication::translate("Conference", "Exporter", nullptr));
        statsButtonConference->setText(QCoreApplication::translate("Conference", "Statistiques", nullptr));
        searchIconConference->setText(QCoreApplication::translate("Conference", "\360\237\224\215", nullptr));
        searchTypeComboConference->setItemText(0, QCoreApplication::translate("Conference", "Par Nom", nullptr));
        searchTypeComboConference->setItemText(1, QCoreApplication::translate("Conference", "Par Lieu", nullptr));
        searchTypeComboConference->setItemText(2, QCoreApplication::translate("Conference", "Par \303\211tat", nullptr));
        searchTypeComboConference->setItemText(3, QCoreApplication::translate("Conference", "Par Date", nullptr));

        searchInputConference->setPlaceholderText(QCoreApplication::translate("Conference", "Entrez votre recherche...", nullptr));
        clearButtonConference->setText(QCoreApplication::translate("Conference", "\342\234\225", nullptr));
        QTableWidgetItem *___qtablewidgetitem = conferenceTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Conference", "ID_conf\303\251rence", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = conferenceTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Conference", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = conferenceTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Conference", "Lieu", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = conferenceTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Conference", "Date_d\303\251but", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = conferenceTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Conference", "Date_fin", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = conferenceTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Conference", "Frais_inscription", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = conferenceTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Conference", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = conferenceTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Conference", "Th\303\250me", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = conferenceTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("Conference", "Actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Conference: public Ui_Conference {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFERENCE_H
