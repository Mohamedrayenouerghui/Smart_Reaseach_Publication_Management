#include "arduinocontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QThread>

ArduinoController::ArduinoController(QObject *parent)
    : QObject(parent)
    , arduino(nullptr)
    , connected(false)
{
}

ArduinoController::~ArduinoController()
{
    disconnectFromArduino();
}

QString ArduinoController::findArduinoPort()
{
    // Forcer la connexion sur COM4
    return "COM4";
}

bool ArduinoController::connectToArduino()
{
    if (connected) {
        qDebug() << "Arduino déjà connecté";
        return true;
    }
    
    // Trouver le port Arduino
    portName = findArduinoPort();
    
    if (portName.isEmpty()) {
        emit errorOccurred("Aucun port série disponible");
        return false;
    }
    
    qDebug() << "Tentative de connexion au port:" << portName;
    
    // Créer et configurer le port série
    arduino = new QSerialPort(this);
    arduino->setPortName(portName);
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    
    // Ouvrir le port
    if (arduino->open(QIODevice::ReadWrite)) {
        connected = true;
        
        qDebug() << "Port ouvert:" << portName;
        
        // Connecter les signaux
        connect(arduino, &QSerialPort::readyRead, this, &ArduinoController::readSerialData);
        connect(arduino, &QSerialPort::errorOccurred, this, &ArduinoController::handleError);
        
        qDebug() << "Attente initialisation (2s)...";
        QThread::msleep(2000);
        
        // Vider le buffer
        arduino->clear();
        
        qDebug() << "Arduino connecté sur" << portName;
        emit arduinoConnected(portName);
        
        return true;
    } else {
        qDebug() << "Erreur de connexion:" << arduino->errorString();
        emit errorOccurred("Impossible d'ouvrir le port: " + arduino->errorString());
        delete arduino;
        arduino = nullptr;
        return false;
    }
}

void ArduinoController::disconnectFromArduino()
{
    if (arduino && arduino->isOpen()) {
        arduino->close();
        delete arduino;
        arduino = nullptr;
        connected = false;
        emit arduinoDisconnected();
        qDebug() << "Arduino déconnecté";
    }
}

bool ArduinoController::isConnected() const
{
    return connected && arduino && arduino->isOpen();
}

QString ArduinoController::getPortName() const
{
    return portName;
}

void ArduinoController::readSerialData()
{
    if (!arduino || !arduino->isOpen()) {
        return;
    }
    
    // Attendre que toutes les données soient disponibles
    arduino->waitForReadyRead(100);
    
    // Lire TOUTES les données disponibles
    QByteArray data = arduino->readAll();
    
    if (data.isEmpty()) {
        return;
    }
    
    qDebug() << "📥 REÇU:" << data;
    
    // Convertir en QString et traiter ligne par ligne
    QString allData = QString::fromUtf8(data);
    QStringList lines = allData.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        QString message = line.trimmed();
        if (!message.isEmpty()) {
            qDebug() << ">>> MESSAGE:" << message;
            processCommand(message);
        }
    }
}

void ArduinoController::processCommand(const QString &command)
{
    qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
    qDebug() << "║ PROCESSCOMMAND                                                ║";
    qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
    qDebug() << "📥 Commande reçue:" << command;
    qDebug() << "📏 Longueur:" << command.length();
    qDebug() << "🔤 Bytes:" << command.toUtf8().toHex();
    
    if (command == "ARDUINO_READY") {
        qDebug() << "✅ Arduino prêt";
        emit arduinoReady();
        
    } else if (command == "TEST_ALIVE") {
        qDebug() << "✅ Test OK";
        
    } else if (command == "ACCEPTER") {
        qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
        qDebug() << "║ *** ACCEPTER REÇU ***                                         ║";
        qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
        qDebug() << "📡 Émission du signal commandeAccepterReceived()...";
        emit commandeAccepterReceived();
        qDebug() << "✅ Signal émis avec succès";
        
    } else if (command == "REJETER") {
        qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
        qDebug() << "║ *** REJETER REÇU ***                                          ║";
        qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
        qDebug() << "📡 Émission du signal commandeRejeterReceived()...";
        emit commandeRejeterReceived();
        qDebug() << "✅ Signal émis avec succès";
        
    } else if (command == "LCD_UPDATED") {
        qDebug() << "✅ LCD mis à jour";
        emit lcdUpdated();
        
    } else {
        qDebug() << "⚠️  Commande inconnue:" << command;
        qDebug() << "📋 Commandes valides: ARDUINO_READY, TEST_ALIVE, ACCEPTER, REJETER, LCD_UPDATED";
    }
    qDebug() << "═══════════════════════════════════════════════════════════════";
}

void ArduinoController::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError && error != QSerialPort::TimeoutError) {
        qDebug() << "Erreur série:" << arduino->errorString();
        emit errorOccurred(arduino->errorString());
    }
}

void ArduinoController::sendAccepteOk()
{
    if (arduino && arduino->isOpen()) {
        arduino->write("ACCEPTE_OK\n");
        arduino->flush();
        qDebug() << "Envoyé: ACCEPTE_OK";
    }
}

void ArduinoController::sendRejeteOk()
{
    if (arduino && arduino->isOpen()) {
        arduino->write("REJETE_OK\n");
        arduino->flush();
        qDebug() << "Envoyé: REJETE_OK";
    }
}

void ArduinoController::sendErreurDB()
{
    if (arduino && arduino->isOpen()) {
        arduino->write("ERREUR_DB\n");
        arduino->flush();
        qDebug() << "Envoyé: ERREUR_DB";
    }
}

void ArduinoController::sendAucuneSoumission()
{
    if (arduino && arduino->isOpen()) {
        arduino->write("AUCUNE_SOUMISSION\n");
        arduino->flush();
        qDebug() << "Envoyé: AUCUNE_SOUMISSION";
    }
}

void ArduinoController::sendStatus(const QString &status)
{
    if (arduino && arduino->isOpen()) {
        // Envoyer directement le message pour l'affichage LCD
        QString message = "LCD:" + status + "\n";
        arduino->write(message.toUtf8());
        arduino->flush();
        qDebug() << "📤 Envoyé au LCD:" << status;
    }
}
