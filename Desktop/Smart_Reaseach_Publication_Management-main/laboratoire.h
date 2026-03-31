#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include <QWidget>

namespace Ui {
class Laboratoire;
}

class Laboratoire : public QWidget
{
    Q_OBJECT

public:
    explicit Laboratoire(QWidget *parent = nullptr);
    ~Laboratoire();

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
    Ui::Laboratoire *ui;
    void setupConnections();
};

#endif // LABORATOIRE_H
