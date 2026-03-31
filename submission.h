#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class Submission;
}

class Submission : public QWidget
{
    Q_OBJECT

public:
    explicit Submission(QWidget *parent = nullptr);
    ~Submission();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onAddSubmission();
    void onDeleteSubmission();
    void onTableSelectionChanged();
    void onSearchClicked();
    void onSearchTextChanged(const QString &text);
    void onExportClicked();
    void onStatisticsClicked();
    void onChooseReviewer();
    void onAutoAssignReviewer();
    void onSelectArticle();
    void onEditRowClicked();
    void onDeleteRowClicked();
    void onSortTriggered();
    void clearForm();

private:
    void loadSubmissions();
    void loadSubmissionsFromDB();
    void loadRelecteurs();
    void setupTable();
    void setupMenus();
    void setupConnections();
    void addButtonsToRow(int row, int id);
    void populateFormFromRow(int row);
    bool validateForm();

    Ui::Submission *ui;
    int selectedRow;
    int selectedId;
    bool isEditMode;
};

#endif // SUBMISSION_H
