#ifndef EVALUATION_H
#define EVALUATION_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class Evaluation;
}

class Evaluation : public QWidget
{
    Q_OBJECT

public:
    explicit Evaluation(QWidget *parent = nullptr);
    ~Evaluation();
    
    void setCurrentReviewer(const QString &reviewerName);

private slots:
    void onAddEvaluation();
    void onDeleteEvaluation();
    void onTableSelectionChanged();
    void onSearchClicked();
    void clearForm();
    void checkNotifications();
    void markNotificationAsRead(int notificationId);

private:
    void loadEvaluations();
    void loadNotifications();
    void setupTable();
    void setupConnections();
    void addButtonsToRow(int row, int id);
    void populateFormFromRow(int row);
    bool validateForm();
    void showNotificationDialog();
    void showNotificationDialog(const QStringList &notifications);
    
    Ui::Evaluation *ui;
    int selectedRow;
    int selectedId;
    bool isEditMode;
    QString currentReviewer;
    int unreadNotifications;
};

#endif // EVALUATION_H
