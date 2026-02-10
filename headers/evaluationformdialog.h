#ifndef EVALUATIONFORMDIALOG_H
#define EVALUATIONFORMDIALOG_H

#include <QDialog>
#include "evaluation.h"

namespace Ui {
class EvaluationFormDialog;
}

class EvaluationFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EvaluationFormDialog(QWidget *parent = nullptr);
    explicit EvaluationFormDialog(const Evaluation& eval, QWidget *parent = nullptr);
    ~EvaluationFormDialog();
    
    Evaluation getEvaluation() const;

private slots:
    void onSave();
    void onCancel();
    void onNoteChanged(int value);

private:
    Ui::EvaluationFormDialog *ui;
    bool editMode;
    int evaluationId;
    
    void setupForm();
    void loadReviewers();
    void populateForm(const Evaluation& eval);
};

#endif // EVALUATIONFORMDIALOG_H
