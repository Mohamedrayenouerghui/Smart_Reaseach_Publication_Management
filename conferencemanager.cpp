#include "conferencemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <algorithm>

ConferenceManager::ConferenceManager(QObject *parent) : QObject(parent), db(QSqlDatabase())
{
}

ConferenceManager::~ConferenceManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool ConferenceManager::connectToDatabase(const QString &host, const QString &user,
                                         const QString &password, const QString &database, int port)
{
    // Utiliser le driver Oracle
    db = QSqlDatabase::addDatabase("QOCISQL");
    db.setHostName(host);
    db.setUserName(user);
    db.setPassword(password);
    db.setDatabaseName(database);
    db.setPort(port);

    if (!db.open()) {
        lastError = "Erreur de connexion: " + db.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Connexion à la base de données réussie!";
    return createTablesIfNeeded();
}

bool ConferenceManager::connectToSqlite(const QString &dbPath)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        lastError = "Erreur de connexion SQLite: " + db.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Connexion SQLite réussie!";
    return createTablesIfNeeded();
}

bool ConferenceManager::isConnected() const
{
    return db.isOpen();
}



bool ConferenceManager::createTablesIfNeeded()
{
    QSqlQuery query;

    // Vérifier si la table CONFERENCE existe
    query.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME='CONFERENCE'");

    if (query.first() && query.value(0).toInt() == 0) {
        // Table n'existe pas, la créer
        QString createTableSQL = R"(
            CREATE TABLE CONFERENCE (
                ID_conference NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
                nom VARCHAR2(255) NOT NULL,
                lieu VARCHAR2(150) NOT NULL,
                Date_debut DATE NOT NULL,
                Date_fin DATE,
                frais_inscri NUMBER(10,2),
                etat VARCHAR2(50) NOT NULL,
                theme VARCHAR2(100) NOT NULL,
                ID_article NUMBER
            )
        )";

        if (!query.exec(createTableSQL)) {
            lastError = "Erreur lors de la création de la table: " + query.lastError().text();
            qDebug() << lastError;
            return false;
        }
    }

    return true;
}

bool ConferenceManager::addConference(const ConferenceData &conference)
{
    QSqlQuery query;
    query.prepare("INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article) "
                  "VALUES (:nom, :lieu, :dateDebut, :dateFin, :fraisInscri, :etat, :theme, :idArticle)");

    query.addBindValue(conference.nom);
    query.addBindValue(conference.lieu);
    query.addBindValue(conference.dateDebut);
    query.addBindValue(conference.dateFin);
    query.addBindValue(conference.fraisInscri);
    query.addBindValue(conference.etat);
    query.addBindValue(conference.theme);
    query.addBindValue(conference.idArticle);

    if (!query.exec()) {
        lastError = "Erreur lors de l'ajout: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Conférence ajoutée avec succès!";
    return true;
}

QList<ConferenceData> ConferenceManager::getAllConferences()
{
    QList<ConferenceData> conferences;
    QSqlQuery query("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                    "FROM CONFERENCE ORDER BY ID_conference DESC");

    while (query.next()) {
        ConferenceData conf;
        conf.id = query.value(0).toInt();
        conf.nom = query.value(1).toString();
        conf.lieu = query.value(2).toString();
        conf.dateDebut = query.value(3).toDate();
        conf.dateFin = query.value(4).toDate();
        conf.fraisInscri = query.value(5).toDouble();
        conf.etat = query.value(6).toString();
        conf.theme = query.value(7).toString();
        conf.idArticle = query.value(8).toInt();

        conferences.append(conf);
    }

    if (query.lastError().isValid()) {
        lastError = "Erreur lors de la lecture: " + query.lastError().text();
        qDebug() << lastError;
    }

    return conferences;
}

ConferenceData ConferenceManager::getConferenceById(int id)
{
    ConferenceData conf;
    conf.id = -1; // Valeur par défaut pour indiquer une non-existence

    QSqlQuery query;
    query.prepare("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                  "FROM CONFERENCE WHERE ID_conference = :id");
    query.addBindValue(id);

    if (query.exec() && query.first()) {
        conf.id = query.value(0).toInt();
        conf.nom = query.value(1).toString();
        conf.lieu = query.value(2).toString();
        conf.dateDebut = query.value(3).toDate();
        conf.dateFin = query.value(4).toDate();
        conf.fraisInscri = query.value(5).toDouble();
        conf.etat = query.value(6).toString();
        conf.theme = query.value(7).toString();
        conf.idArticle = query.value(8).toInt();
    } else {
        lastError = "Conférence non trouvée";
    }

    return conf;
}

bool ConferenceManager::updateConference(const ConferenceData &conference)
{
    QSqlQuery query;
    query.prepare("UPDATE CONFERENCE SET nom = :nom, lieu = :lieu, Date_debut = :dateDebut, "
                  "Date_fin = :dateFin, frais_inscri = :fraisInscri, etat = :etat, theme = :theme, "
                  "ID_article = :idArticle WHERE ID_conference = :id");

    query.addBindValue(conference.nom);
    query.addBindValue(conference.lieu);
    query.addBindValue(conference.dateDebut);
    query.addBindValue(conference.dateFin);
    query.addBindValue(conference.fraisInscri);
    query.addBindValue(conference.etat);
    query.addBindValue(conference.theme);
    query.addBindValue(conference.idArticle);
    query.addBindValue(conference.id);

    if (!query.exec()) {
        lastError = "Erreur lors de la mise à jour: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Conférence mise à jour avec succès!";
    return true;
}

bool ConferenceManager::deleteConference(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CONFERENCE WHERE ID_conference = :id");
    query.addBindValue(id);

    if (!query.exec()) {
        lastError = "Erreur lors de la suppression: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Conférence supprimée avec succès!";
    return true;
}

QList<ConferenceData> ConferenceManager::searchConferencesByName(const QString &name)
{
    QList<ConferenceData> conferences;
    QSqlQuery query;
    query.prepare("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                  "FROM CONFERENCE WHERE UPPER(nom) LIKE UPPER(:name) ORDER BY nom");
    query.addBindValue("%" + name + "%");

    while (query.exec() && query.next()) {
        ConferenceData conf;
        conf.id = query.value(0).toInt();
        conf.nom = query.value(1).toString();
        conf.lieu = query.value(2).toString();
        conf.dateDebut = query.value(3).toDate();
        conf.dateFin = query.value(4).toDate();
        conf.fraisInscri = query.value(5).toDouble();
        conf.etat = query.value(6).toString();
        conf.theme = query.value(7).toString();
        conf.idArticle = query.value(8).toInt();

        conferences.append(conf);
    }

    return conferences;
}

QList<ConferenceData> ConferenceManager::getConferencesByState(const QString &state)
{
    QList<ConferenceData> conferences;
    QSqlQuery query;
    query.prepare("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                  "FROM CONFERENCE WHERE etat = :etat ORDER BY Date_debut DESC");
    query.addBindValue(state);

    while (query.exec() && query.next()) {
        ConferenceData conf;
        conf.id = query.value(0).toInt();
        conf.nom = query.value(1).toString();
        conf.lieu = query.value(2).toString();
        conf.dateDebut = query.value(3).toDate();
        conf.dateFin = query.value(4).toDate();
        conf.fraisInscri = query.value(5).toDouble();
        conf.etat = query.value(6).toString();
        conf.theme = query.value(7).toString();
        conf.idArticle = query.value(8).toInt();

        conferences.append(conf);
    }

    return conferences;
}

QList<ConferenceData> ConferenceManager::getConferencesByDateRange(const QDate &startDate, const QDate &endDate)
{
    QList<ConferenceData> conferences;
    QSqlQuery query;
    query.prepare("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                  "FROM CONFERENCE WHERE Date_debut >= :startDate AND Date_debut <= :endDate ORDER BY Date_debut");
    query.addBindValue(startDate);
    query.addBindValue(endDate);

    while (query.exec() && query.next()) {
        ConferenceData conf;
        conf.id = query.value(0).toInt();
        conf.nom = query.value(1).toString();
        conf.lieu = query.value(2).toString();
        conf.dateDebut = query.value(3).toDate();
        conf.dateFin = query.value(4).toDate();
        conf.fraisInscri = query.value(5).toDouble();
        conf.etat = query.value(6).toString();
        conf.theme = query.value(7).toString();
        conf.idArticle = query.value(8).toInt();

        conferences.append(conf);
    }

    return conferences;
}

QList<ConferenceData> ConferenceManager::sortConferencesByName(bool ascending)
{
    QList<ConferenceData> conferences = getAllConferences();
    std::sort(conferences.begin(), conferences.end(),
        [ascending](const ConferenceData &a, const ConferenceData &b) {
            if (ascending)
                return a.nom < b.nom;
            else
                return a.nom > b.nom;
        });
    return conferences;
}

QList<ConferenceData> ConferenceManager::sortConferencesByDate(bool ascending)
{
    QList<ConferenceData> conferences = getAllConferences();
    std::sort(conferences.begin(), conferences.end(),
        [ascending](const ConferenceData &a, const ConferenceData &b) {
            if (ascending)
                return a.dateDebut < b.dateDebut;
            else
                return a.dateDebut > b.dateDebut;
        });
    return conferences;
}

QList<ConferenceData> ConferenceManager::sortConferencesByState(bool ascending)
{
    QList<ConferenceData> conferences = getAllConferences();
    std::sort(conferences.begin(), conferences.end(),
        [ascending](const ConferenceData &a, const ConferenceData &b) {
            if (ascending)
                return a.etat < b.etat;
            else
                return a.etat > b.etat;
        });
    return conferences;
}

int ConferenceManager::getConferenceCount()
{
    QSqlQuery query("SELECT COUNT(*) FROM CONFERENCE");
    if (query.first()) {
        return query.value(0).toInt();
    }
    return 0;
}

double ConferenceManager::getAverageFees()
{
    QSqlQuery query("SELECT AVG(frais_inscri) FROM CONFERENCE");
    if (query.first()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

int ConferenceManager::getConferenceCountByState(const QString &state)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CONFERENCE WHERE etat = :etat");
    query.addBindValue(state);

    if (query.exec() && query.first()) {
        return query.value(0).toInt();
    }
    return 0;
}

QString ConferenceManager::getLastError() const
{
    return lastError;
}
