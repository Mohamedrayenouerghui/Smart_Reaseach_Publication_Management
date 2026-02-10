#include "headers/evaluationformdialog.h"
#include "ui_evaluationformdialog.h"
#include "headers/database.h"
#include <QMessageBox>

EvaluationFormDialog::EvaluationFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EvaluationFormDialog),
    editMode(false),
    evaluationId(0)
{
    ui->setupUi(this);
    setupForm();
}

EvaluationFormDialog::EvaluationFormDialog(const Evaluation& eval, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EvaluationFormDialog),
    editMode(true),
    evaluationId(eval.getId())
{
    ui->setupUi(this);
    setupForm();
    populateForm(eval);
}

EvaluationFormDialog::~EvaluationFormDialog()
{
    delete ui;
}

void EvaluationFormDialog::setupForm()
{
    setWindowTitle(editMode ? "Modifier l'évaluation" : "Nouvelle évaluation");
    
    loadReviewers();
    
    ui->spinBoxNote->setRange(1, 10);
    ui->spinBoxNote->setValue(5);
    ui->dateEdit->setDate(QDate::currentDate());
    
    connect(ui->btnSave, &QPushButton::clicked, this, &EvaluationFormDialog::onSave);
    connect(ui->btnCancel, &QPushButton::clicked, this, &EvaluationFormDialog::onCancel);
    connect(ui->spinBoxNote, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &EvaluationFormDialog::onNoteChanged);
    
    onNoteChanged(ui->spinBoxNote->value());
}

void EvaluationFormDialog::loadReviewers()
{
    QVector<Reviewer> reviewers = Database::instance().getAllReviewers();
    
    for (const Reviewer& reviewer : reviewers) {
        ui->comboBoxReviewer->addItem(reviewer.getNom(), reviewer.getId());
    }
}

void EvaluationFormDialog::populateForm(const Evaluation& eval)
{
    ui->spinBoxSubmissionId->setValue(eval.getSubmissionId());
    
    int index = ui->comboBoxReviewer->findData(eval.getReviewerId());
    if (index != -1) {
        ui->comboBoxReviewer->setCurrentIndex(index);
    }
    
    ui->spinBoxNote->setValue(eval.getNote());
    ui->textEditCommentaires->setPlainText(eval.getCommentaires());
    ui->dateEdit->setDate(eval.getDateEvaluation());
    
    if (eval.getStatut() == Evaluation::EnCours) {
        ui->radioEnCours->setChecked(true);
    } else {
        ui->radioTerminee->setChecked(true);
    }
}

Evaluation EvaluationFormDialog::getEvaluation() const
{
    Evaluation eval;
    
    if (editMode) {
        eval.setId(evaluationId);
    }
    
    eval.setSubmissionId(ui->spinBoxSubmissionId->value());
    eval.setReviewerId(ui->comboBoxReviewer->currentData().toInt());
    eval.setNote(ui->spinBoxNote->value());
    eval.setCommentaires(ui->textEditCommentaires->toPlainText());
    eval.setDateEvaluation(ui->dateEdit->date());
    
    if (ui->radioEnCours->isChecked()) {
        eval.setStatut(Evaluation::EnCours);
    } else {
        eval.setStatut(Evaluation::Terminee);
    }
    
    return eval;
}

void EvaluationFormDialog::onSave()
{
    if (ui->comboBoxReviewer->currentIndex() == -1) {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner un relecteur");
        return;
    }
    
    accept();
}

void EvaluationFormDialog::onCancel()
{
    reject();
}

void EvaluationFormDialog::onNoteChanged(int value)
{
    ui->lblNoteValue->setText(QString::number(value) + "/10");
    
    QString color;
    if (value <= 3) {
        color = "#e74c3c"; // Red
    } else if (value <= 6) {
        color = "#f39c12"; // Orange
    } else {
        color = "#27ae60"; // Green
    }
    
    ui->lblNoteValue->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
}
