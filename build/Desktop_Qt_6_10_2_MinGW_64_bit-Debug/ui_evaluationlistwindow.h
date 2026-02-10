/********************************************************************************
** Form generated from reading UI file 'evaluationlistwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVALUATIONLISTWINDOW_H
#define UI_EVALUATIONLISTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EvaluationListWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QHBoxLayout *horizontalLayoutSearch;
    QLabel *lblSearch;
    QLineEdit *lineEditSearch;
    QLabel *lblFilter;
    QComboBox *comboBoxFilter;
    QHBoxLayout *horizontalLayoutSort;
    QPushButton *btnSortDate;
    QPushButton *btnSortNote;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnRefresh;
    QTableView *tableViewEvaluations;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *btnCreate;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnExportPDF;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EvaluationListWindow)
    {
        if (EvaluationListWindow->objectName().isEmpty())
            EvaluationListWindow->setObjectName("EvaluationListWindow");
        EvaluationListWindow->resize(900, 600);
        centralwidget = new QWidget(EvaluationListWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        lblTitle = new QLabel(centralwidget);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        lblTitle->setFont(font);

        verticalLayout->addWidget(lblTitle);

        horizontalLayoutSearch = new QHBoxLayout();
        horizontalLayoutSearch->setObjectName("horizontalLayoutSearch");
        lblSearch = new QLabel(centralwidget);
        lblSearch->setObjectName("lblSearch");

        horizontalLayoutSearch->addWidget(lblSearch);

        lineEditSearch = new QLineEdit(centralwidget);
        lineEditSearch->setObjectName("lineEditSearch");

        horizontalLayoutSearch->addWidget(lineEditSearch);

        lblFilter = new QLabel(centralwidget);
        lblFilter->setObjectName("lblFilter");

        horizontalLayoutSearch->addWidget(lblFilter);

        comboBoxFilter = new QComboBox(centralwidget);
        comboBoxFilter->addItem(QString());
        comboBoxFilter->addItem(QString());
        comboBoxFilter->addItem(QString());
        comboBoxFilter->setObjectName("comboBoxFilter");

        horizontalLayoutSearch->addWidget(comboBoxFilter);


        verticalLayout->addLayout(horizontalLayoutSearch);

        horizontalLayoutSort = new QHBoxLayout();
        horizontalLayoutSort->setObjectName("horizontalLayoutSort");
        btnSortDate = new QPushButton(centralwidget);
        btnSortDate->setObjectName("btnSortDate");

        horizontalLayoutSort->addWidget(btnSortDate);

        btnSortNote = new QPushButton(centralwidget);
        btnSortNote->setObjectName("btnSortNote");

        horizontalLayoutSort->addWidget(btnSortNote);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutSort->addItem(horizontalSpacer);

        btnRefresh = new QPushButton(centralwidget);
        btnRefresh->setObjectName("btnRefresh");

        horizontalLayoutSort->addWidget(btnRefresh);


        verticalLayout->addLayout(horizontalLayoutSort);

        tableViewEvaluations = new QTableView(centralwidget);
        tableViewEvaluations->setObjectName("tableViewEvaluations");
        tableViewEvaluations->setAlternatingRowColors(true);

        verticalLayout->addWidget(tableViewEvaluations);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        btnCreate = new QPushButton(centralwidget);
        btnCreate->setObjectName("btnCreate");
        btnCreate->setMinimumHeight(35);

        horizontalLayoutButtons->addWidget(btnCreate);

        btnEdit = new QPushButton(centralwidget);
        btnEdit->setObjectName("btnEdit");
        btnEdit->setMinimumHeight(35);

        horizontalLayoutButtons->addWidget(btnEdit);

        btnDelete = new QPushButton(centralwidget);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setMinimumHeight(35);

        horizontalLayoutButtons->addWidget(btnDelete);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacer_2);

        btnExportPDF = new QPushButton(centralwidget);
        btnExportPDF->setObjectName("btnExportPDF");
        btnExportPDF->setMinimumHeight(35);

        horizontalLayoutButtons->addWidget(btnExportPDF);


        verticalLayout->addLayout(horizontalLayoutButtons);

        EvaluationListWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(EvaluationListWindow);
        statusbar->setObjectName("statusbar");
        EvaluationListWindow->setStatusBar(statusbar);

        retranslateUi(EvaluationListWindow);

        QMetaObject::connectSlotsByName(EvaluationListWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EvaluationListWindow)
    {
        EvaluationListWindow->setWindowTitle(QCoreApplication::translate("EvaluationListWindow", "Liste des \303\211valuations", nullptr));
        lblTitle->setText(QCoreApplication::translate("EvaluationListWindow", "Gestion des \303\211valuations", nullptr));
        lblSearch->setText(QCoreApplication::translate("EvaluationListWindow", "Recherche:", nullptr));
        lineEditSearch->setPlaceholderText(QCoreApplication::translate("EvaluationListWindow", "Rechercher une \303\251valuation...", nullptr));
        lblFilter->setText(QCoreApplication::translate("EvaluationListWindow", "Filtrer par:", nullptr));
        comboBoxFilter->setItemText(0, QCoreApplication::translate("EvaluationListWindow", "Tous", nullptr));
        comboBoxFilter->setItemText(1, QCoreApplication::translate("EvaluationListWindow", "En cours", nullptr));
        comboBoxFilter->setItemText(2, QCoreApplication::translate("EvaluationListWindow", "Termin\303\251e", nullptr));

        btnSortDate->setText(QCoreApplication::translate("EvaluationListWindow", "Trier par Date", nullptr));
        btnSortNote->setText(QCoreApplication::translate("EvaluationListWindow", "Trier par Note", nullptr));
        btnRefresh->setText(QCoreApplication::translate("EvaluationListWindow", "Actualiser", nullptr));
        btnCreate->setText(QCoreApplication::translate("EvaluationListWindow", "Cr\303\251er", nullptr));
        btnEdit->setText(QCoreApplication::translate("EvaluationListWindow", "Modifier", nullptr));
        btnDelete->setText(QCoreApplication::translate("EvaluationListWindow", "Supprimer", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("EvaluationListWindow", "Export PDF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EvaluationListWindow: public Ui_EvaluationListWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVALUATIONLISTWINDOW_H
