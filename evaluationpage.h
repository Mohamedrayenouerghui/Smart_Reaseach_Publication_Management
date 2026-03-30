#ifndef EVALUATIONPAGE_H
#define EVALUATIONPAGE_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class EvaluationPage;
}

class EvaluationPage : public QWidget
{
    Q_OBJECT

public:
    explicit EvaluationPage(QWidget *parent = nullptr);
    ~EvaluationPage();

private slots:
    // ── List tab ──
    void onCreateEvaluation();
    void onEditEvaluation();
    void onDeleteEvaluation();
    void onExportPDF();
    void onSearch(const QString& text);
    void onFilterByStatus(int index);
    void onSortByDate();
    void onSortByNote();
    void onRefreshList();

    // ── Statistics tab ──
    void onRefreshStats();
    void onExportStats();

    // ── Reviewers tab ──
    void onReviewerSelected(const QModelIndex& index);
    void onAddReviewer();
    void onRefreshReviewers();

private:
    Ui::EvaluationPage *ui;
    QStandardItemModel *model;
    QStandardItemModel *reviewerModel;
    QStandardItemModel *reviewerEvalModel;

    void setupTable();
    void setupConnections();
    void loadEvaluations();

    // Statistics
    void setupCharts();
    void createNoteDistributionChart();
    void createTimelineChart();
    void updateStatLabels();

    // Reviewers
    void setupReviewerTab();
    void loadReviewers();
    void loadReviewerEvaluations(int reviewerId);
};

#endif // EVALUATIONPAGE_H
