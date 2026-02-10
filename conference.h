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
    void onAjouterClicked();
    void onModifierClicked();
    void onSupprimerClicked();
    void onRechercherClicked();
    void onTrierClicked();
    void onExporterPDFClicked();
    void onExporterWordClicked();
    void onStatistiquesClicked();
    void onClearSearchClicked();

private:
    Ui::Conference *ui;
    void setupConnections();
};

#endif // CONFERENCE_H
