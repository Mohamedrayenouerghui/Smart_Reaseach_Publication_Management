#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QWidget>

namespace Ui {
class Conference;
}

class Conference : public QWidget
{
    Q_OBJECT

public:
    explicit Conference(QWidget *parent = nullptr);
    ~Conference();

private slots:
    void onAddButtonClicked();
    void onStatsButtonClicked();
    void onClearButtonClicked();
    void onSortMenuTriggered(QAction *action);
    void onExportMenuTriggered(QAction *action);
    void onSubmitButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::Conference *ui;
    
    void setupConnections();
    void setupMenus();
    void setupTable();
};

#endif // CONFERENCE_H
