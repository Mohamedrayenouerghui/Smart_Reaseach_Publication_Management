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

    // Returns the completed Evaluation object (submissionId already resolved)
    Evaluation getEvaluation() const;

private slots:
    void onSave();
    void onCancel();
    void onNoteChanged(int value);

private:
    Ui::EvaluationFormDialog *ui;
    bool editMode;
    int  evaluationId;
    int  resolvedSubmissionId; // Set during onSave() after auto-creating submission

    void setupForm();
    void loadReviewers();
    void loadArticles();
    void populateForm(const Evaluation& eval);

    // Creates or retrieves a SOUMISSION for the given article, returns soumission ID or -1
    int getOrCreateSubmission(int articleId);
};

#endif // EVALUATIONFORMDIALOG_H
