#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

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
    void onAddSubmission();
    void onDeleteSubmission();
    void onTableSelectionChanged();
    void onSortClicked();
    void onSearchClicked();
    void onExportClicked();
    void onStatisticsClicked();
    void clearForm();
    void onArticleChanged(int index);
    void onUpdateSubmission() {} // stub

private:
    Ui::Submission *ui;
    int selectedRow;

    void setupTable();
    void setupConnections();
    void loadArticlesCombo();
    void loadUsersCombo();
    void loadConferencesCombo();
    void loadSubmissions(const QString &filter = QString());
};

#endif // SUBMISSION_H
