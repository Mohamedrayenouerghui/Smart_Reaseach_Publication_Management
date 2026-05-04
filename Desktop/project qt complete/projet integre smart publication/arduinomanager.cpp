#include "arduinomanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ArduinoManager::ArduinoManager(QObject *parent)
    : QObject(parent)
    , arduinoController(nullptr)
    , currentSoumissionId(-1)
{
}

ArduinoManager::~ArduinoManager()
{
    if (arduinoController) {
        arduinoController->disconnectFromArduino();
        delete arduinoController;
    }
}

bool ArduinoManager::initialize()
{
    // Créer le contrôleur Arduino
    arduinoController = new ArduinoController(this);
    
    // Connecter les signaux
    connect(arduinoController, &ArduinoController::commandeAccepterReceived,
            this, &ArduinoManager::onCommandeAccepter);
    connect(arduinoController, &ArduinoController::commandeRejeterReceived,
            this, &ArduinoManager::onCommandeRejeter);
    connect(arduinoController, &ArduinoController::arduinoConnected,
            this, &ArduinoManager::onArduinoConnected);
    connect(arduinoController, &ArduinoController::arduinoDisconnected,
            this, &ArduinoManager::onArduinoDisconnected);
    connect(arduinoController, &ArduinoController::errorOccurred,
            this, &ArduinoManager::onArduinoError);
    
    // Tenter la connexion
    if (arduinoController->connectToArduino()) {
        emit statusChanged("Arduino connecté sur " + arduinoController->getPortName());
        return true;
    } else {
        emit statusChanged("Échec de connexion à l'Arduino");
        return false;
    }
}

bool ArduinoManager::isArduinoConnected() const
{
    return arduinoController && arduinoController->isConnected();
}

QString ArduinoManager::getArduinoPort() const
{
    if (arduinoController) {
        return arduinoController->getPortName();
    }
    return QString();
}

void ArduinoManager::sendStatusToArduino(const QString &message)
{
    if (arduinoController && arduinoController->isConnected()) {
        arduinoController->sendStatus(message);
    }
}

void ArduinoManager::setCurrentSoumission(int idSoumission)
{
    currentSoumissionId = idSoumission;
    qDebug() << "Soumission en cours définie:" << idSoumission;
}

int ArduinoManager::getPremiereSoumissionEnAttente()
{
    QSqlQuery query;
    
    // Chercher la première soumission en attente
    query.prepare(
        "SELECT ID_soumission FROM SOUMISSION "
        "WHERE (Etat = 'En révision' OR Etat = 'En attente') "
        "AND (Decision_finale IS NULL OR Decision_finale = 'En attente') "
        "ORDER BY Date_envoi ASC "
        "FETCH FIRST 1 ROWS ONLY"
    );
    
    if (query.exec()) {
        if (query.next()) {
            int id = query.value(0).toInt();
            qDebug() << "✓ Soumission trouvée: ID =" << id;
            return id;
        } else {
            qDebug() << "✗ Aucune soumission en attente trouvée";
        }
    } else {
        qDebug() << "✗ Erreur SQL:" << query.lastError().text();
    }
    
    return -1;
}

bool ArduinoManager::updateDecisionSoumission(int idSoumission, const QString &decision)
{
    QSqlQuery query;
    
    // Mettre à jour la décision finale et l'état
    query.prepare(
        "UPDATE SOUMISSION "
        "SET Decision_finale = :decision, "
        "    Etat = 'Traité', "
        "    Date_decision = SYSDATE "
        "WHERE ID_soumission = :id"
    );
    
    query.bindValue(":decision", decision);
    query.bindValue(":id", idSoumission);
    
    if (query.exec()) {
        qDebug() << "Soumission" << idSoumission << "mise à jour avec décision:" << decision;
        return true;
    } else {
        qDebug() << "Erreur lors de la mise à jour:" << query.lastError().text();
        return false;
    }
}

QString ArduinoManager::getEtatSoumission(int idSoumission)
{
    QSqlQuery query;
    
    query.prepare("SELECT Decision_finale FROM SOUMISSION WHERE ID_soumission = :id");
    query.bindValue(":id", idSoumission);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    
    return QString();
}

void ArduinoManager::onCommandeAccepter()
{
    qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
    qDebug() << "║ *** TRAITEMENT ACCEPTER ***                                   ║";
    qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
    
    int idSoumission = currentSoumissionId;
    qDebug() << "📊 currentSoumissionId =" << currentSoumissionId;
    
    if (idSoumission == -1) {
        qDebug() << "⚠️  Aucune soumission en cours, recherche de la première en attente...";
        idSoumission = getPremiereSoumissionEnAttente();
    }
    
    if (idSoumission == -1) {
        qDebug() << "❌ Aucune soumission trouvée";
        arduinoController->sendAucuneSoumission();
        return;
    }
    
    qDebug() << "✅ Soumission ID:" << idSoumission;
    qDebug() << "📝 Mise à jour de la base de données...";
    
    if (updateDecisionSoumission(idSoumission, "Accepté")) {
        qDebug() << "✅ Base de données mise à jour avec succès";
        arduinoController->sendAccepteOk();
        qDebug() << "📤 Envoi de ACCEPTE_OK à Arduino";
        emit soumissionAcceptee(idSoumission);
        qDebug() << "📡 Signal soumissionAcceptee(" << idSoumission << ") émis";
        qDebug() << "🎉 SUCCÈS - Soumission" << idSoumission << "acceptée";
        currentSoumissionId = -1;
    } else {
        qDebug() << "❌ ERREUR lors de la mise à jour de la base de données";
        arduinoController->sendErreurDB();
        qDebug() << "📤 Envoi de ERREUR_DB à Arduino";
    }
    qDebug() << "═══════════════════════════════════════════════════════════════";
}

void ArduinoManager::onCommandeRejeter()
{
    qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
    qDebug() << "║ *** TRAITEMENT REJETER ***                                    ║";
    qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
    
    int idSoumission = currentSoumissionId;
    qDebug() << "📊 currentSoumissionId =" << currentSoumissionId;
    
    if (idSoumission == -1) {
        qDebug() << "⚠️  Aucune soumission en cours, recherche de la première en attente...";
        idSoumission = getPremiereSoumissionEnAttente();
    }
    
    if (idSoumission == -1) {
        qDebug() << "❌ Aucune soumission trouvée";
        arduinoController->sendAucuneSoumission();
        return;
    }
    
    qDebug() << "✅ Soumission ID:" << idSoumission;
    qDebug() << "📝 Mise à jour de la base de données...";
    
    if (updateDecisionSoumission(idSoumission, "Rejeté")) {
        qDebug() << "✅ Base de données mise à jour avec succès";
        arduinoController->sendRejeteOk();
        qDebug() << "📤 Envoi de REJETE_OK à Arduino";
        emit soumissionRejetee(idSoumission);
        qDebug() << "📡 Signal soumissionRejetee(" << idSoumission << ") émis";
        qDebug() << "🎉 SUCCÈS - Soumission" << idSoumission << "rejetée";
        currentSoumissionId = -1;
    } else {
        qDebug() << "❌ ERREUR lors de la mise à jour de la base de données";
        arduinoController->sendErreurDB();
        qDebug() << "📤 Envoi de ERREUR_DB à Arduino";
    }
    qDebug() << "═══════════════════════════════════════════════════════════════";
}

void ArduinoManager::onArduinoConnected(const QString &portName)
{
    qDebug() << "Arduino connecté sur" << portName;
    emit statusChanged("Arduino connecté sur " + portName);
}

void ArduinoManager::onArduinoDisconnected()
{
    qDebug() << "Arduino déconnecté";
    emit statusChanged("Arduino déconnecté");
}

void ArduinoManager::onArduinoError(const QString &error)
{
    qDebug() << "Erreur Arduino:" << error;
    emit errorOccurred(error);
}
