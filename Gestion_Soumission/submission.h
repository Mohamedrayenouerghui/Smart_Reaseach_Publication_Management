#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <QWidget>

namespace Ui {
class Submission;
}

class Submission : public QWidget
{
    Q_OBJECT

public:
    explicit Submission(QWidget *parent = nullptr);
    ~Submission();

private slots:
    // Empty slots - no functionality
    void onAddSubmission() {}
    void onUpdateSubmission() {}
    void onDeleteSubmission() {}
    void onTableSelectionChanged() {}
    void onSortClicked() {}
    void onSearchClicked() {}
    void onExportClicked() {}
    void onStatisticsClicked() {}
    void clearForm() {}

private:
    void loadSubmissions();
    
    Ui::Submission *ui;
    int selectedRow;
};

#endif // SUBMISSION_H
