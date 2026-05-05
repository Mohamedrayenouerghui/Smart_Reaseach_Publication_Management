#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QWidget>
#include "conferencemanager.h"

class SMSService;
class LocationService;

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
    void onAjouterFormClicked();
    void onModifierClicked();
    void onModifierFormClicked();
    void onAnnulerModifClicked();
    void onSupprimerClicked();
    void onRechercherClicked();
    void onTrierClicked();
    void onExporterPDFClicked();
    void onExporterWordClicked();
    void onEnvoyerSMSClicked();
    void onLocalisationClicked();
    void onStatistiquesClicked();
    void onClearSearchClicked();
    void onArduinoConfClicked();

private:
    Ui::Conference *ui;
    ConferenceManager manager;

    SMSService     *smsService      = nullptr;
    LocationService *locationService = nullptr;

    void setupConnections();
    void refreshTable(const QList<ConferenceData> &conferences);
    ConferenceData readConferenceFromForm() const;
    void populateModifierForm(const ConferenceData &conf);
};

#endif // CONFERENCE_H
