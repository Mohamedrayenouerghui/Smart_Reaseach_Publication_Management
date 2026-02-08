/********************************************************************************
** Form generated from reading UI file 'userspage.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERSPAGE_H
#define UI_USERSPAGE_H

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

class Ui_UsersPage
{
public:
    QVBoxLayout *mainLayout;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QToolButton *addButtonUser;
    QSpacerItem *horizontalSpacer;
    QToolButton *searchButtonUser;
    QToolButton *sortButtonUser;
    QToolButton *exportButtonUser;
    QToolButton *statsButtonUser;
    QWidget *searchBar;
    QHBoxLayout *searchBarLayout;
    QLabel *searchIconUser;
    QComboBox *searchTypeComboUser;
    QLineEdit *searchInputUser;
    QToolButton *clearButtonUser;
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QTableWidget *usersTable;

    void setupUi(QWidget *UsersPage)
    {
        if (UsersPage->objectName().isEmpty())
            UsersPage->setObjectName("UsersPage");
        UsersPage->resize(920, 660);
        UsersPage->setStyleSheet(QString::fromUtf8("\n"
"}"));
        mainLayout = new QVBoxLayout(UsersPage);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        toolbar = new QWidget(UsersPage);
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
        addButtonUser = new QToolButton(toolbar);
        addButtonUser->setObjectName("addButtonUser");
        addButtonUser->setMinimumSize(QSize(120, 38));
        addButtonUser->setMaximumSize(QSize(120, 38));
        addButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        addButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(addButtonUser);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);

        searchButtonUser = new QToolButton(toolbar);
        searchButtonUser->setObjectName("searchButtonUser");
        searchButtonUser->setMinimumSize(QSize(110, 38));
        searchButtonUser->setMaximumSize(QSize(110, 38));
        searchButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        searchButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(searchButtonUser);

        sortButtonUser = new QToolButton(toolbar);
        sortButtonUser->setObjectName("sortButtonUser");
        sortButtonUser->setMinimumSize(QSize(100, 38));
        sortButtonUser->setMaximumSize(QSize(100, 38));
        sortButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        sortButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        sortButtonUser->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(sortButtonUser);

        exportButtonUser = new QToolButton(toolbar);
        exportButtonUser->setObjectName("exportButtonUser");
        exportButtonUser->setMinimumSize(QSize(100, 38));
        exportButtonUser->setMaximumSize(QSize(100, 38));
        exportButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        exportButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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
        exportButtonUser->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

        toolbarLayout->addWidget(exportButtonUser);

        statsButtonUser = new QToolButton(toolbar);
        statsButtonUser->setObjectName("statsButtonUser");
        statsButtonUser->setMinimumSize(QSize(110, 38));
        statsButtonUser->setMaximumSize(QSize(110, 38));
        statsButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        statsButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        toolbarLayout->addWidget(statsButtonUser);


        mainLayout->addWidget(toolbar);

        searchBar = new QWidget(UsersPage);
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
        searchIconUser = new QLabel(searchBar);
        searchIconUser->setObjectName("searchIconUser");
        searchIconUser->setMinimumSize(QSize(24, 24));
        searchIconUser->setMaximumSize(QSize(24, 24));
        searchIconUser->setStyleSheet(QString::fromUtf8("font-size: 18px; background: transparent; color: #64748B;"));
        searchIconUser->setAlignment(Qt::AlignmentFlag::AlignCenter);

        searchBarLayout->addWidget(searchIconUser);

        searchTypeComboUser = new QComboBox(searchBar);
        searchTypeComboUser->addItem(QString());
        searchTypeComboUser->addItem(QString());
        searchTypeComboUser->addItem(QString());
        searchTypeComboUser->setObjectName("searchTypeComboUser");
        searchTypeComboUser->setMinimumSize(QSize(140, 40));
        searchTypeComboUser->setMaximumSize(QSize(140, 40));
        searchTypeComboUser->setStyleSheet(QString::fromUtf8("QComboBox {\n"
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

        searchBarLayout->addWidget(searchTypeComboUser);

        searchInputUser = new QLineEdit(searchBar);
        searchInputUser->setObjectName("searchInputUser");
        searchInputUser->setMinimumSize(QSize(0, 40));
        searchInputUser->setMaximumSize(QSize(16777215, 40));
        searchInputUser->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
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

        searchBarLayout->addWidget(searchInputUser);

        clearButtonUser = new QToolButton(searchBar);
        clearButtonUser->setObjectName("clearButtonUser");
        clearButtonUser->setMinimumSize(QSize(40, 40));
        clearButtonUser->setMaximumSize(QSize(40, 40));
        clearButtonUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        clearButtonUser->setStyleSheet(QString::fromUtf8("QToolButton {\n"
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

        searchBarLayout->addWidget(clearButtonUser);


        mainLayout->addWidget(searchBar);

        contentArea = new QWidget(UsersPage);
        contentArea->setObjectName("contentArea");
        contentArea->setStyleSheet(QString::fromUtf8("QWidget#contentArea {\n"
"    background-color: #F5F7FA;\n"
"}"));
        contentLayout = new QVBoxLayout(contentArea);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(16, 16, 16, 16);
        usersTable = new QTableWidget(contentArea);
        if (usersTable->columnCount() < 7)
            usersTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        usersTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        usersTable->setObjectName("usersTable");
        usersTable->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
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
        usersTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        usersTable->setAlternatingRowColors(true);
        usersTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        usersTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        usersTable->horizontalHeader()->setStretchLastSection(true);
        usersTable->verticalHeader()->setVisible(false);

        contentLayout->addWidget(usersTable);


        mainLayout->addWidget(contentArea);


        retranslateUi(UsersPage);

        QMetaObject::connectSlotsByName(UsersPage);
    } // setupUi

    void retranslateUi(QWidget *UsersPage)
    {
        UsersPage->setWindowTitle(QCoreApplication::translate("UsersPage", "Users", nullptr));
        addButtonUser->setText(QCoreApplication::translate("UsersPage", "+ Ajouter", nullptr));
        searchButtonUser->setText(QCoreApplication::translate("UsersPage", "Rechercher", nullptr));
        sortButtonUser->setText(QCoreApplication::translate("UsersPage", "Trier", nullptr));
        exportButtonUser->setText(QCoreApplication::translate("UsersPage", "Exporter", nullptr));
        statsButtonUser->setText(QCoreApplication::translate("UsersPage", "Statistiques", nullptr));
        searchIconUser->setText(QCoreApplication::translate("UsersPage", "\360\237\224\215", nullptr));
        searchTypeComboUser->setItemText(0, QCoreApplication::translate("UsersPage", "Par Nom", nullptr));
        searchTypeComboUser->setItemText(1, QCoreApplication::translate("UsersPage", "Par Email", nullptr));
        searchTypeComboUser->setItemText(2, QCoreApplication::translate("UsersPage", "Par Role", nullptr));

        searchInputUser->setPlaceholderText(QCoreApplication::translate("UsersPage", "Entrez votre recherche...", nullptr));
        clearButtonUser->setText(QCoreApplication::translate("UsersPage", "\342\234\225", nullptr));
        QTableWidgetItem *___qtablewidgetitem = usersTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("UsersPage", "ID_User", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = usersTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("UsersPage", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = usersTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("UsersPage", "Prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = usersTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("UsersPage", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = usersTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("UsersPage", "Mdp", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = usersTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("UsersPage", "Role", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = usersTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("UsersPage", "Photo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UsersPage: public Ui_UsersPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERSPAGE_H
