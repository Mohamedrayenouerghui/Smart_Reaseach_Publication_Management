#ifndef ARDUINOMANAGER_H
#define ARDUINOMANAGER_H

#include <QObject>
#include "arduinocontroller.h"

class ArduinoManager : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoManager(QObject *parent = nullptr);
    ~ArduinoManager();
    
    // Méthodes publiques
    bool initialize();
    bool isArduinoConnected() const;
    QString getArduinoPort() const;
    void sendStatusToArduino(const QString &message);
    void setCurrentSoumission(int idSoumission); // Définir la soumission en cours

signals:
    void statusChanged(const QString &status);
    void soumissionAcceptee(int idSoumission);
    void soumissionRejetee(int idSoumission);
    void errorOccurred(const QString &error);

private slots:
    void onCommandeAccepter();
    void onCommandeRejeter();
    void onArduinoConnected(const QString &portName);
    void onArduinoDisconnected();
    void onArduinoError(const QString &error);

private:
    ArduinoController *arduinoController;
    int currentSoumissionId; // ID de la soumission en cours de traitement
    
    // Méthodes privées pour la base de données
    int getPremiereSoumissionEnAttente();
    bool updateDecisionSoumission(int idSoumission, const QString &decision);
    QString getEtatSoumission(int idSoumission);
};

#endif // ARDUINOMANAGER_H
