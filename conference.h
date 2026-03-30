#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QWidget>
#include "conferencemanager.h"

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
    void onSearchButtonClicked();
    void onTableRowDoubleClicked(int row, int column);
    void onDeleteButtonClicked();
    void onEditButtonClicked();

private:
    Ui::Conference *ui;
    ConferenceManager *conferenceManager;
    ConferenceData currentEditingConference;
    int selectedRowId;
    
    void setupConnections();
    void setupMenus();
    void setupTable();
    void loadConferencesToTable();
    void refreshTable();
    void clearFormFields();
    void displayConferenceInForm(const ConferenceData &conference);
};

#endif // CONFERENCE_H
