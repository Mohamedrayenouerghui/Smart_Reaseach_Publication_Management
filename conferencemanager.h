#ifndef CONFERENCEMANAGER_H
#define CONFERENCEMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QDate>

struct ConferenceData {
    int id;
    QString nom;
    QString lieu;
    QDate dateDebut;
    QDate dateFin;
    double fraisInscri;
    QString etat;
    QString theme;
    int idArticle;
};

class ConferenceManager : public QObject
{
    Q_OBJECT
public:
    explicit ConferenceManager(QObject *parent = nullptr);
    explicit ConferenceManager(const QSqlDatabase &database, QObject *parent = nullptr);
    ~ConferenceManager();

    void setDatabase(const QSqlDatabase &database);

    // Connexion à la base de données
    bool connectToDatabase(const QString &host, const QString &user, 
                         const QString &password, const QString &database, int port = 1521);
    bool connectToSqlite(const QString &dbPath);
    bool isConnected() const;

    // CRUD Operations
    bool addConference(const ConferenceData &conference);
    QList<ConferenceData> getAllConferences();
    ConferenceData getConferenceById(int id);
    bool updateConference(const ConferenceData &conference);
    bool deleteConference(int id);

    // Recherche et filtrage
    QList<ConferenceData> searchConferencesByName(const QString &name);
    QList<ConferenceData> getConferencesByState(const QString &state);
    QList<ConferenceData> getConferencesByDateRange(const QDate &startDate, const QDate &endDate);

    // Tri
    QList<ConferenceData> sortConferencesByName(bool ascending = true);
    QList<ConferenceData> sortConferencesByDate(bool ascending = true);
    QList<ConferenceData> sortConferencesByState(bool ascending = true);

    // Statistiques
    int getConferenceCount();
    double getAverageFees();
    int getConferenceCountByState(const QString &state);

    // Gestion d'erreurs
    QString getLastError() const;

private:
    QSqlDatabase db;
    QString lastError;
    bool createTablesIfNeeded();
};

#endif // CONFERENCEMANAGER_H
