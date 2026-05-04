#ifndef ARDUINOCONTROLLER_H
#define ARDUINOCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>

class ArduinoController : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoController(QObject *parent = nullptr);
    ~ArduinoController();
    
    // Méthodes publiques
    bool connectToArduino();
    void disconnectFromArduino();
    bool isConnected() const;
    QString getPortName() const;
    
    // Envoyer des commandes à l'Arduino
    void sendAccepteOk();
    void sendRejeteOk();
    void sendErreurDB();
    void sendAucuneSoumission();
    void sendStatus(const QString &status);

signals:
    void arduinoConnected(const QString &portName);
    void arduinoDisconnected();
    void commandeAccepterReceived();
    void commandeRejeterReceived();
    void arduinoReady();
    void lcdUpdated();
    void errorOccurred(const QString &error);

private slots:
    void readSerialData();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *arduino;
    QString portName;
    bool connected;
    
    QString findArduinoPort();
    void processCommand(const QString &command);
};

#endif // ARDUINOCONTROLLER_H
