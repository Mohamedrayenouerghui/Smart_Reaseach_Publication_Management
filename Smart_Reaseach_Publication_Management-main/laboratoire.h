#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QLabel>
#include "laboratoire_entity.h"

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
    void onHistoriqueClicked();
    void onModifierClicked();
    void onSupprimerClicked();
    void onRechercherClicked();
    void onTrierClicked();
    void onExporterPDFClicked();
    void onNotificationClicked();
    void onStatistiquesClicked();
    void onClearSearchClicked();
    void onSupprimerHistoriqueClicked();
    void onExporterHistoriqueClicked();
    void onValiderAjoutClicked();
    void onAnnulerClicked();
    void onTableSelectionChanged();
    void validateNomInput(const QString &text);
    void validateLocalisationInput(const QString &text);

private:
    Ui::Laboratoire *ui;
    int m_currentLaboId = 0;
    QLabel *nomWarningLabel;
    QLabel *localisationWarningLabel;

    void setupConnections();
    void setupTableWidget();
    void setupValidationLabels();
    void loadAllLaboratoires();
    void displayLaboratoires(const QList<LaboratoireEntity> &labos);
    void clearForm();
    void loadLaboIntoForm(int id);
    void updateStatisticsLabel();
    void populateHistoriqueTable();
};

#endif // LABORATOIRE_H
