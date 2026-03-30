#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QWidget>
#include <QHeaderView>

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
    void onAjouterClicked();
    void onSubmitFormClicked();
    void onResetFormClicked();
    void onModifierClicked();
    void onSupprimerClicked();
    void onRechercherClicked();
    void onTrierClicked();
    void onExporterPDFClicked();
    void onClearSearchClicked();

private:
    Ui::Conference *ui;

    // State
    int  editingConferenceId = -1;
    bool isEditMode          = false;

    // Helpers
    void setupTable();
    void setupConnections();
    void loadConferences(const QString &whereClause = QString());
};

#endif // CONFERENCE_H
