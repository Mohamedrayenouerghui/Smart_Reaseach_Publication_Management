#ifndef EVALUATIONLISTWINDOW_H
#define EVALUATIONLISTWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class EvaluationListWindow;
}

class EvaluationListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EvaluationListWindow(QWidget *parent = nullptr);
    ~EvaluationListWindow();

private slots:
    void onCreateEvaluation();
    void onEditEvaluation();
    void onDeleteEvaluation();
    void onExportPDF();
    void onSearch(const QString& text);
    void onFilterByStatus(int index);
    void onSortByDate();
    void onSortByNote();
    void onRefresh();

private:
    Ui::EvaluationListWindow *ui;
    QStandardItemModel *model;
    
    void setupTable();
    void loadEvaluations();
    void setupConnections();
};

#endif // EVALUATIONLISTWINDOW_H
