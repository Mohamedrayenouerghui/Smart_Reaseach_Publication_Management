/********************************************************************************
** Form generated from reading UI file 'reviewerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REVIEWERWINDOW_H
#define UI_REVIEWERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReviewerWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayoutReviewers;
    QLabel *lblReviewers;
    QListView *listViewReviewers;
    QHBoxLayout *horizontalLayoutReviewerButtons;
    QPushButton *btnAddReviewer;
    QPushButton *btnRefresh;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayoutEvaluations;
    QLabel *lblSelectedReviewer;
    QLabel *lblReviewerStats;
    QTableView *tableViewEvaluations;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ReviewerWindow)
    {
        if (ReviewerWindow->objectName().isEmpty())
            ReviewerWindow->setObjectName("ReviewerWindow");
        ReviewerWindow->resize(900, 600);
        centralwidget = new QWidget(ReviewerWindow);
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

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName("layoutWidget");
        verticalLayoutReviewers = new QVBoxLayout(layoutWidget);
        verticalLayoutReviewers->setObjectName("verticalLayoutReviewers");
        verticalLayoutReviewers->setContentsMargins(0, 0, 0, 0);
        lblReviewers = new QLabel(layoutWidget);
        lblReviewers->setObjectName("lblReviewers");
        QFont font1;
        font1.setBold(true);
        lblReviewers->setFont(font1);

        verticalLayoutReviewers->addWidget(lblReviewers);

        listViewReviewers = new QListView(layoutWidget);
        listViewReviewers->setObjectName("listViewReviewers");
        listViewReviewers->setMinimumWidth(250);

        verticalLayoutReviewers->addWidget(listViewReviewers);

        horizontalLayoutReviewerButtons = new QHBoxLayout();
        horizontalLayoutReviewerButtons->setObjectName("horizontalLayoutReviewerButtons");
        btnAddReviewer = new QPushButton(layoutWidget);
        btnAddReviewer->setObjectName("btnAddReviewer");

        horizontalLayoutReviewerButtons->addWidget(btnAddReviewer);

        btnRefresh = new QPushButton(layoutWidget);
        btnRefresh->setObjectName("btnRefresh");

        horizontalLayoutReviewerButtons->addWidget(btnRefresh);


        verticalLayoutReviewers->addLayout(horizontalLayoutReviewerButtons);

        splitter->addWidget(layoutWidget);
        layoutWidget2 = new QWidget(splitter);
        layoutWidget2->setObjectName("layoutWidget2");
        verticalLayoutEvaluations = new QVBoxLayout(layoutWidget2);
        verticalLayoutEvaluations->setObjectName("verticalLayoutEvaluations");
        verticalLayoutEvaluations->setContentsMargins(0, 0, 0, 0);
        lblSelectedReviewer = new QLabel(layoutWidget2);
        lblSelectedReviewer->setObjectName("lblSelectedReviewer");
        lblSelectedReviewer->setFont(font1);

        verticalLayoutEvaluations->addWidget(lblSelectedReviewer);

        lblReviewerStats = new QLabel(layoutWidget2);
        lblReviewerStats->setObjectName("lblReviewerStats");

        verticalLayoutEvaluations->addWidget(lblReviewerStats);

        tableViewEvaluations = new QTableView(layoutWidget2);
        tableViewEvaluations->setObjectName("tableViewEvaluations");
        tableViewEvaluations->setAlternatingRowColors(true);

        verticalLayoutEvaluations->addWidget(tableViewEvaluations);

        splitter->addWidget(layoutWidget2);

        verticalLayout->addWidget(splitter);

        ReviewerWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ReviewerWindow);
        statusbar->setObjectName("statusbar");
        ReviewerWindow->setStatusBar(statusbar);

        retranslateUi(ReviewerWindow);

        QMetaObject::connectSlotsByName(ReviewerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ReviewerWindow)
    {
        ReviewerWindow->setWindowTitle(QCoreApplication::translate("ReviewerWindow", "Gestion des Relecteurs", nullptr));
        lblTitle->setText(QCoreApplication::translate("ReviewerWindow", "Gestion des Relecteurs", nullptr));
        lblReviewers->setText(QCoreApplication::translate("ReviewerWindow", "Liste des Relecteurs", nullptr));
        btnAddReviewer->setText(QCoreApplication::translate("ReviewerWindow", "Ajouter", nullptr));
        btnRefresh->setText(QCoreApplication::translate("ReviewerWindow", "Actualiser", nullptr));
        lblSelectedReviewer->setText(QCoreApplication::translate("ReviewerWindow", "S\303\251lectionnez un relecteur", nullptr));
        lblReviewerStats->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ReviewerWindow: public Ui_ReviewerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVIEWERWINDOW_H
