/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuitter;
    QAction *actionNouvelle_Evaluation;
    QAction *actionMes_Evaluations;
    QAction *actionStatistiques;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QHBoxLayout *horizontalLayoutStats;
    QGroupBox *groupBoxTotal;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblTotalEvaluations;
    QGroupBox *groupBoxPending;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblPendingEvaluations;
    QGroupBox *groupBoxCompleted;
    QVBoxLayout *verticalLayout_4;
    QLabel *lblCompletedEvaluations;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *btnNouvelleEvaluation;
    QPushButton *btnMesEvaluations;
    QPushButton *btnGestionRelecture;
    QPushButton *btnStatistiques;
    QGroupBox *groupBoxRecent;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnRefresh;
    QPushButton *btnExportPDF;
    QTableWidget *tableRecentEvaluations;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QMenu *menuEvaluations;
    QMenu *menuOutils;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName("actionQuitter");
        actionNouvelle_Evaluation = new QAction(MainWindow);
        actionNouvelle_Evaluation->setObjectName("actionNouvelle_Evaluation");
        actionMes_Evaluations = new QAction(MainWindow);
        actionMes_Evaluations->setObjectName("actionMes_Evaluations");
        actionStatistiques = new QAction(MainWindow);
        actionStatistiques->setObjectName("actionStatistiques");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        lblTitle = new QLabel(centralwidget);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        lblTitle->setFont(font);

        verticalLayout->addWidget(lblTitle);

        horizontalLayoutStats = new QHBoxLayout();
        horizontalLayoutStats->setObjectName("horizontalLayoutStats");
        groupBoxTotal = new QGroupBox(centralwidget);
        groupBoxTotal->setObjectName("groupBoxTotal");
        verticalLayout_2 = new QVBoxLayout(groupBoxTotal);
        verticalLayout_2->setObjectName("verticalLayout_2");
        lblTotalEvaluations = new QLabel(groupBoxTotal);
        lblTotalEvaluations->setObjectName("lblTotalEvaluations");
        lblTotalEvaluations->setAlignment(Qt::AlignCenter);
        QFont font1;
        font1.setPointSize(24);
        font1.setBold(true);
        lblTotalEvaluations->setFont(font1);

        verticalLayout_2->addWidget(lblTotalEvaluations);


        horizontalLayoutStats->addWidget(groupBoxTotal);

        groupBoxPending = new QGroupBox(centralwidget);
        groupBoxPending->setObjectName("groupBoxPending");
        verticalLayout_3 = new QVBoxLayout(groupBoxPending);
        verticalLayout_3->setObjectName("verticalLayout_3");
        lblPendingEvaluations = new QLabel(groupBoxPending);
        lblPendingEvaluations->setObjectName("lblPendingEvaluations");
        lblPendingEvaluations->setAlignment(Qt::AlignCenter);
        lblPendingEvaluations->setFont(font1);

        verticalLayout_3->addWidget(lblPendingEvaluations);


        horizontalLayoutStats->addWidget(groupBoxPending);

        groupBoxCompleted = new QGroupBox(centralwidget);
        groupBoxCompleted->setObjectName("groupBoxCompleted");
        verticalLayout_4 = new QVBoxLayout(groupBoxCompleted);
        verticalLayout_4->setObjectName("verticalLayout_4");
        lblCompletedEvaluations = new QLabel(groupBoxCompleted);
        lblCompletedEvaluations->setObjectName("lblCompletedEvaluations");
        lblCompletedEvaluations->setAlignment(Qt::AlignCenter);
        lblCompletedEvaluations->setFont(font1);

        verticalLayout_4->addWidget(lblCompletedEvaluations);


        horizontalLayoutStats->addWidget(groupBoxCompleted);


        verticalLayout->addLayout(horizontalLayoutStats);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        btnNouvelleEvaluation = new QPushButton(centralwidget);
        btnNouvelleEvaluation->setObjectName("btnNouvelleEvaluation");
        btnNouvelleEvaluation->setMinimumHeight(40);

        horizontalLayoutButtons->addWidget(btnNouvelleEvaluation);

        btnMesEvaluations = new QPushButton(centralwidget);
        btnMesEvaluations->setObjectName("btnMesEvaluations");
        btnMesEvaluations->setMinimumHeight(40);

        horizontalLayoutButtons->addWidget(btnMesEvaluations);

        btnGestionRelecture = new QPushButton(centralwidget);
        btnGestionRelecture->setObjectName("btnGestionRelecture");
        btnGestionRelecture->setMinimumHeight(40);

        horizontalLayoutButtons->addWidget(btnGestionRelecture);

        btnStatistiques = new QPushButton(centralwidget);
        btnStatistiques->setObjectName("btnStatistiques");
        btnStatistiques->setMinimumHeight(40);

        horizontalLayoutButtons->addWidget(btnStatistiques);


        verticalLayout->addLayout(horizontalLayoutButtons);

        groupBoxRecent = new QGroupBox(centralwidget);
        groupBoxRecent->setObjectName("groupBoxRecent");
        verticalLayout_5 = new QVBoxLayout(groupBoxRecent);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnRefresh = new QPushButton(groupBoxRecent);
        btnRefresh->setObjectName("btnRefresh");

        horizontalLayout->addWidget(btnRefresh);

        btnExportPDF = new QPushButton(groupBoxRecent);
        btnExportPDF->setObjectName("btnExportPDF");

        horizontalLayout->addWidget(btnExportPDF);


        verticalLayout_5->addLayout(horizontalLayout);

        tableRecentEvaluations = new QTableWidget(groupBoxRecent);
        tableRecentEvaluations->setObjectName("tableRecentEvaluations");
        tableRecentEvaluations->setAlternatingRowColors(true);
        tableRecentEvaluations->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_5->addWidget(tableRecentEvaluations);


        verticalLayout->addWidget(groupBoxRecent);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName("menuFichier");
        menuEvaluations = new QMenu(menubar);
        menuEvaluations->setObjectName("menuEvaluations");
        menuOutils = new QMenu(menubar);
        menuOutils->setObjectName("menuOutils");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFichier->menuAction());
        menubar->addAction(menuEvaluations->menuAction());
        menubar->addAction(menuOutils->menuAction());
        menuFichier->addAction(actionQuitter);
        menuEvaluations->addAction(actionNouvelle_Evaluation);
        menuEvaluations->addAction(actionMes_Evaluations);
        menuOutils->addAction(actionStatistiques);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des \303\211valuations - Dashboard", nullptr));
        actionQuitter->setText(QCoreApplication::translate("MainWindow", "Quitter", nullptr));
        actionNouvelle_Evaluation->setText(QCoreApplication::translate("MainWindow", "Nouvelle \303\211valuation", nullptr));
        actionMes_Evaluations->setText(QCoreApplication::translate("MainWindow", "Mes \303\211valuations", nullptr));
        actionStatistiques->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "Tableau de Bord - Gestion des \303\211valuations", nullptr));
        groupBoxTotal->setTitle(QCoreApplication::translate("MainWindow", "Total \303\211valuations", nullptr));
        lblTotalEvaluations->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupBoxPending->setTitle(QCoreApplication::translate("MainWindow", "En Cours", nullptr));
        lblPendingEvaluations->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblPendingEvaluations->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #f39c12;", nullptr));
        groupBoxCompleted->setTitle(QCoreApplication::translate("MainWindow", "Termin\303\251es", nullptr));
        lblCompletedEvaluations->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblCompletedEvaluations->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #27ae60;", nullptr));
        btnNouvelleEvaluation->setText(QCoreApplication::translate("MainWindow", "Nouvelle \303\211valuation", nullptr));
        btnMesEvaluations->setText(QCoreApplication::translate("MainWindow", "Mes \303\211valuations", nullptr));
        btnGestionRelecture->setText(QCoreApplication::translate("MainWindow", "Gestion Relecture", nullptr));
        btnStatistiques->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        groupBoxRecent->setTitle(QCoreApplication::translate("MainWindow", "\303\211valuations R\303\251centes", nullptr));
        btnRefresh->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        menuFichier->setTitle(QCoreApplication::translate("MainWindow", "Fichier", nullptr));
        menuEvaluations->setTitle(QCoreApplication::translate("MainWindow", "\303\211valuations", nullptr));
        menuOutils->setTitle(QCoreApplication::translate("MainWindow", "Outils", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
