#ifndef REVIEWERWINDOW_H
#define REVIEWERWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class ReviewerWindow;
}

class ReviewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReviewerWindow(QWidget *parent = nullptr);
    ~ReviewerWindow();

private slots:
    void onReviewerSelected(const QModelIndex& index);
    void onAddReviewer();
    void onRefresh();

private:
    Ui::ReviewerWindow *ui;
    QStandardItemModel *reviewerModel;
    QStandardItemModel *evaluationModel;
    
    void setupModels();
    void loadReviewers();
    void loadReviewerEvaluations(int reviewerId);
    void setupConnections();
};

#endif // REVIEWERWINDOW_H
