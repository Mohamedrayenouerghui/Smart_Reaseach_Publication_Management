/********************************************************************************
** Form generated from reading UI file 'statisticswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICSWINDOW_H
#define UI_STATISTICSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatisticsWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QHBoxLayout *horizontalLayoutStats;
    QLabel *lblTotalEvaluations;
    QLabel *lblPendingEvaluations;
    QLabel *lblCompletedEvaluations;
    QLabel *lblAverageTime;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnRefresh;
    QPushButton *btnExport;
    QGroupBox *groupBoxCharts;
    QVBoxLayout *verticalLayout_2;
    QWidget *chartWidgetDistribution;
    QVBoxLayout *verticalLayout_3;
    QWidget *chartWidgetTimeline;
    QVBoxLayout *verticalLayout_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *StatisticsWindow)
    {
        if (StatisticsWindow->objectName().isEmpty())
            StatisticsWindow->setObjectName("StatisticsWindow");
        StatisticsWindow->resize(1000, 700);
        centralwidget = new QWidget(StatisticsWindow);
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

        horizontalLayoutStats = new QHBoxLayout();
        horizontalLayoutStats->setObjectName("horizontalLayoutStats");
        lblTotalEvaluations = new QLabel(centralwidget);
        lblTotalEvaluations->setObjectName("lblTotalEvaluations");
        QFont font1;
        font1.setPointSize(12);
        lblTotalEvaluations->setFont(font1);

        horizontalLayoutStats->addWidget(lblTotalEvaluations);

        lblPendingEvaluations = new QLabel(centralwidget);
        lblPendingEvaluations->setObjectName("lblPendingEvaluations");
        lblPendingEvaluations->setFont(font1);

        horizontalLayoutStats->addWidget(lblPendingEvaluations);

        lblCompletedEvaluations = new QLabel(centralwidget);
        lblCompletedEvaluations->setObjectName("lblCompletedEvaluations");
        lblCompletedEvaluations->setFont(font1);

        horizontalLayoutStats->addWidget(lblCompletedEvaluations);

        lblAverageTime = new QLabel(centralwidget);
        lblAverageTime->setObjectName("lblAverageTime");
        lblAverageTime->setFont(font1);

        horizontalLayoutStats->addWidget(lblAverageTime);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutStats->addItem(horizontalSpacer);

        btnRefresh = new QPushButton(centralwidget);
        btnRefresh->setObjectName("btnRefresh");

        horizontalLayoutStats->addWidget(btnRefresh);

        btnExport = new QPushButton(centralwidget);
        btnExport->setObjectName("btnExport");

        horizontalLayoutStats->addWidget(btnExport);


        verticalLayout->addLayout(horizontalLayoutStats);

        groupBoxCharts = new QGroupBox(centralwidget);
        groupBoxCharts->setObjectName("groupBoxCharts");
        verticalLayout_2 = new QVBoxLayout(groupBoxCharts);
        verticalLayout_2->setObjectName("verticalLayout_2");
        chartWidgetDistribution = new QWidget(groupBoxCharts);
        chartWidgetDistribution->setObjectName("chartWidgetDistribution");
        chartWidgetDistribution->setMinimumSize(QSize(0, 300));
        verticalLayout_3 = new QVBoxLayout(chartWidgetDistribution);
        verticalLayout_3->setObjectName("verticalLayout_3");

        verticalLayout_2->addWidget(chartWidgetDistribution);

        chartWidgetTimeline = new QWidget(groupBoxCharts);
        chartWidgetTimeline->setObjectName("chartWidgetTimeline");
        chartWidgetTimeline->setMinimumSize(QSize(0, 250));
        verticalLayout_4 = new QVBoxLayout(chartWidgetTimeline);
        verticalLayout_4->setObjectName("verticalLayout_4");

        verticalLayout_2->addWidget(chartWidgetTimeline);


        verticalLayout->addWidget(groupBoxCharts);

        StatisticsWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(StatisticsWindow);
        statusbar->setObjectName("statusbar");
        StatisticsWindow->setStatusBar(statusbar);

        retranslateUi(StatisticsWindow);

        QMetaObject::connectSlotsByName(StatisticsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StatisticsWindow)
    {
        StatisticsWindow->setWindowTitle(QCoreApplication::translate("StatisticsWindow", "Statistiques Personnelles", nullptr));
        lblTitle->setText(QCoreApplication::translate("StatisticsWindow", "Statistiques Personnelles", nullptr));
        lblTotalEvaluations->setText(QCoreApplication::translate("StatisticsWindow", "Total: 0", nullptr));
        lblPendingEvaluations->setText(QCoreApplication::translate("StatisticsWindow", "En cours: 0", nullptr));
        lblCompletedEvaluations->setText(QCoreApplication::translate("StatisticsWindow", "Termin\303\251es: 0", nullptr));
        lblAverageTime->setText(QCoreApplication::translate("StatisticsWindow", "Temps moyen: -", nullptr));
        btnRefresh->setText(QCoreApplication::translate("StatisticsWindow", "Actualiser", nullptr));
        btnExport->setText(QCoreApplication::translate("StatisticsWindow", "Exporter", nullptr));
        groupBoxCharts->setTitle(QCoreApplication::translate("StatisticsWindow", "Graphiques", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatisticsWindow: public Ui_StatisticsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICSWINDOW_H
