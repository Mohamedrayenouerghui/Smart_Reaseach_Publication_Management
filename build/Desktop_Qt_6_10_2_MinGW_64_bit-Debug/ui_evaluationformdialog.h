/********************************************************************************
** Form generated from reading UI file 'evaluationformdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVALUATIONFORMDIALOG_H
#define UI_EVALUATIONFORMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EvaluationFormDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QGroupBox *groupBoxInfo;
    QFormLayout *formLayout;
    QLabel *lblSubmissionId;
    QSpinBox *spinBoxSubmissionId;
    QLabel *lblReviewer;
    QComboBox *comboBoxReviewer;
    QLabel *lblNote;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBoxNote;
    QLabel *lblNoteValue;
    QLabel *lblDate;
    QDateEdit *dateEdit;
    QGroupBox *groupBoxComments;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEditCommentaires;
    QGroupBox *groupBoxStatus;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioEnCours;
    QRadioButton *radioTerminee;
    QHBoxLayout *horizontalLayoutButtons;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupUi(QDialog *EvaluationFormDialog)
    {
        if (EvaluationFormDialog->objectName().isEmpty())
            EvaluationFormDialog->setObjectName("EvaluationFormDialog");
        EvaluationFormDialog->resize(500, 550);
        verticalLayout = new QVBoxLayout(EvaluationFormDialog);
        verticalLayout->setObjectName("verticalLayout");
        lblTitle = new QLabel(EvaluationFormDialog);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        lblTitle->setFont(font);

        verticalLayout->addWidget(lblTitle);

        groupBoxInfo = new QGroupBox(EvaluationFormDialog);
        groupBoxInfo->setObjectName("groupBoxInfo");
        formLayout = new QFormLayout(groupBoxInfo);
        formLayout->setObjectName("formLayout");
        lblSubmissionId = new QLabel(groupBoxInfo);
        lblSubmissionId->setObjectName("lblSubmissionId");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, lblSubmissionId);

        spinBoxSubmissionId = new QSpinBox(groupBoxInfo);
        spinBoxSubmissionId->setObjectName("spinBoxSubmissionId");
        spinBoxSubmissionId->setMinimum(1);
        spinBoxSubmissionId->setMaximum(999999);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, spinBoxSubmissionId);

        lblReviewer = new QLabel(groupBoxInfo);
        lblReviewer->setObjectName("lblReviewer");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, lblReviewer);

        comboBoxReviewer = new QComboBox(groupBoxInfo);
        comboBoxReviewer->setObjectName("comboBoxReviewer");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, comboBoxReviewer);

        lblNote = new QLabel(groupBoxInfo);
        lblNote->setObjectName("lblNote");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, lblNote);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        spinBoxNote = new QSpinBox(groupBoxInfo);
        spinBoxNote->setObjectName("spinBoxNote");
        spinBoxNote->setMinimum(1);
        spinBoxNote->setMaximum(10);
        spinBoxNote->setValue(5);

        horizontalLayout->addWidget(spinBoxNote);

        lblNoteValue = new QLabel(groupBoxInfo);
        lblNoteValue->setObjectName("lblNoteValue");
        QFont font1;
        font1.setBold(true);
        lblNoteValue->setFont(font1);

        horizontalLayout->addWidget(lblNoteValue);


        formLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, horizontalLayout);

        lblDate = new QLabel(groupBoxInfo);
        lblDate->setObjectName("lblDate");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, lblDate);

        dateEdit = new QDateEdit(groupBoxInfo);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, dateEdit);


        verticalLayout->addWidget(groupBoxInfo);

        groupBoxComments = new QGroupBox(EvaluationFormDialog);
        groupBoxComments->setObjectName("groupBoxComments");
        verticalLayout_2 = new QVBoxLayout(groupBoxComments);
        verticalLayout_2->setObjectName("verticalLayout_2");
        textEditCommentaires = new QTextEdit(groupBoxComments);
        textEditCommentaires->setObjectName("textEditCommentaires");

        verticalLayout_2->addWidget(textEditCommentaires);


        verticalLayout->addWidget(groupBoxComments);

        groupBoxStatus = new QGroupBox(EvaluationFormDialog);
        groupBoxStatus->setObjectName("groupBoxStatus");
        horizontalLayout_2 = new QHBoxLayout(groupBoxStatus);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        radioEnCours = new QRadioButton(groupBoxStatus);
        radioEnCours->setObjectName("radioEnCours");
        radioEnCours->setChecked(true);

        horizontalLayout_2->addWidget(radioEnCours);

        radioTerminee = new QRadioButton(groupBoxStatus);
        radioTerminee->setObjectName("radioTerminee");

        horizontalLayout_2->addWidget(radioTerminee);


        verticalLayout->addWidget(groupBoxStatus);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacer);

        btnSave = new QPushButton(EvaluationFormDialog);
        btnSave->setObjectName("btnSave");
        btnSave->setMinimumSize(QSize(100, 35));

        horizontalLayoutButtons->addWidget(btnSave);

        btnCancel = new QPushButton(EvaluationFormDialog);
        btnCancel->setObjectName("btnCancel");
        btnCancel->setMinimumSize(QSize(100, 35));

        horizontalLayoutButtons->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(EvaluationFormDialog);

        QMetaObject::connectSlotsByName(EvaluationFormDialog);
    } // setupUi

    void retranslateUi(QDialog *EvaluationFormDialog)
    {
        EvaluationFormDialog->setWindowTitle(QCoreApplication::translate("EvaluationFormDialog", "Formulaire d'\303\211valuation", nullptr));
        lblTitle->setText(QCoreApplication::translate("EvaluationFormDialog", "Formulaire d'\303\211valuation", nullptr));
        groupBoxInfo->setTitle(QCoreApplication::translate("EvaluationFormDialog", "Informations de l'\303\211valuation", nullptr));
        lblSubmissionId->setText(QCoreApplication::translate("EvaluationFormDialog", "ID Soumission:", nullptr));
        lblReviewer->setText(QCoreApplication::translate("EvaluationFormDialog", "Relecteur:", nullptr));
        lblNote->setText(QCoreApplication::translate("EvaluationFormDialog", "Note (1-10):", nullptr));
        lblNoteValue->setText(QCoreApplication::translate("EvaluationFormDialog", "5/10", nullptr));
        lblDate->setText(QCoreApplication::translate("EvaluationFormDialog", "Date:", nullptr));
        groupBoxComments->setTitle(QCoreApplication::translate("EvaluationFormDialog", "Commentaires", nullptr));
        textEditCommentaires->setPlaceholderText(QCoreApplication::translate("EvaluationFormDialog", "Entrez vos commentaires ici...", nullptr));
        groupBoxStatus->setTitle(QCoreApplication::translate("EvaluationFormDialog", "Statut", nullptr));
        radioEnCours->setText(QCoreApplication::translate("EvaluationFormDialog", "En cours", nullptr));
        radioTerminee->setText(QCoreApplication::translate("EvaluationFormDialog", "Termin\303\251e", nullptr));
        btnSave->setText(QCoreApplication::translate("EvaluationFormDialog", "Enregistrer", nullptr));
        btnCancel->setText(QCoreApplication::translate("EvaluationFormDialog", "Annuler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EvaluationFormDialog: public Ui_EvaluationFormDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVALUATIONFORMDIALOG_H
