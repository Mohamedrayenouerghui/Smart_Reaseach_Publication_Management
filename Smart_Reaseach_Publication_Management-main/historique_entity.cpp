#include "historique_entity.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

HistoriqueEntity::HistoriqueEntity()
    : m_id(0), m_date(QDateTime::currentDateTime()), m_type(AJOUT), m_utilisateur("admin")
{}

HistoriqueEntity::HistoriqueEntity(int id, const QDateTime &date, const QString &laboratoireNom,
                                   TypeOperation type, const QString &utilisateur, const QString &details)
    : m_id(id), m_date(date), m_laboratoireNom(laboratoireNom),
      m_type(type), m_utilisateur(utilisateur), m_details(details)
{}

QString HistoriqueEntity::getTypeString() const
{
    switch (m_type) {
    case AJOUT:        return "Ajout";
    case MODIFICATION: return "Modification";
    case SUPPRESSION:  return "Suppression";
    default:           return "Inconnu";
    }
}

bool HistoriqueEntity::save()
{
    return insert();
}

bool HistoriqueEntity::insert()
{
    QSqlDatabase &db = Connection::createInstance().getDb();
    if (!db.isOpen()) {
        qCritical() << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO HISTORIQUE (ID_historique, date_modification, nom_laboratoire, type_operation, utilisateur, details) "
                  "VALUES (SEQ_HISTORIQUE.NEXTVAL, TO_TIMESTAMP(?, 'YYYY-MM-DD HH24:MI:SS'), ?, ?, ?, ?)");
    query.addBindValue(m_date.toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(m_laboratoireNom);
    query.addBindValue(getTypeString());
    query.addBindValue(m_utilisateur);
    query.addBindValue(m_details);

    if (!query.exec()) {
        qCritical() << "Failed to insert historique:" << query.lastError().text();
        return false;
    }

    m_id = query.lastInsertId().toInt();
    qInfo() << "Historique entry added successfully";
    return true;
}

QList<HistoriqueEntity> HistoriqueEntity::getAll()
{
    QList<HistoriqueEntity> historique;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_historique, TO_CHAR(date_modification, 'YYYY-MM-DD HH24:MI:SS'), nom_laboratoire, type_operation, utilisateur, details "
                  "FROM HISTORIQUE ORDER BY date_modification DESC");

    if (query.exec()) {
        while (query.next()) {
            TypeOperation type = AJOUT;
            QString typeStr = query.value(3).toString();
            if (typeStr == "Modification") type = MODIFICATION;
            else if (typeStr == "Suppression") type = SUPPRESSION;

            historique.append(HistoriqueEntity(query.value(0).toInt(),
                                               QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd HH:mm:ss"),
                                               query.value(2).toString(), type,
                                               query.value(4).toString(), query.value(5).toString()));
        }
    }
    return historique;
}

QList<HistoriqueEntity> HistoriqueEntity::getByLaboratoire(const QString &laboratoireNom)
{
    QList<HistoriqueEntity> historique;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_historique, TO_CHAR(date_modification, 'YYYY-MM-DD HH24:MI:SS'), nom_laboratoire, type_operation, utilisateur, details "
                  "FROM HISTORIQUE WHERE nom_laboratoire = ? ORDER BY date_modification DESC");
    query.addBindValue(laboratoireNom);

    if (query.exec()) {
        while (query.next()) {
            TypeOperation type = AJOUT;
            QString typeStr = query.value(3).toString();
            if (typeStr == "Modification") type = MODIFICATION;
            else if (typeStr == "Suppression") type = SUPPRESSION;

            historique.append(HistoriqueEntity(query.value(0).toInt(),
                                               QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd HH:mm:ss"),
                                               query.value(2).toString(), type,
                                               query.value(4).toString(), query.value(5).toString()));
        }
    }
    return historique;
}

QList<HistoriqueEntity> HistoriqueEntity::getByType(TypeOperation type)
{
    QList<HistoriqueEntity> historique;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_historique, TO_CHAR(date_modification, 'YYYY-MM-DD HH24:MI:SS'), nom_laboratoire, type_operation, utilisateur, details "
                  "FROM HISTORIQUE WHERE type_operation = ? ORDER BY date_modification DESC");
    query.addBindValue(getTypeStringStatic(type));

    if (query.exec()) {
        while (query.next()) {
            TypeOperation t = AJOUT;
            QString typeStr = query.value(3).toString();
            if (typeStr == "Modification") t = MODIFICATION;
            else if (typeStr == "Suppression") t = SUPPRESSION;

            historique.append(HistoriqueEntity(query.value(0).toInt(),
                                               QDateTime::fromString(query.value(1).toString(), "yyyy-MM-dd HH:mm:ss"),
                                               query.value(2).toString(), t,
                                               query.value(4).toString(), query.value(5).toString()));

            historique.append(HistoriqueEntity(query.value(0).toInt(), query.value(1).toDateTime(),
                                               query.value(2).toString(), t,
                                               query.value(4).toString(), query.value(5).toString()));
        }
    }
    return historique;
}

bool HistoriqueEntity::clearAll()
{
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("DELETE FROM HISTORIQUE");
    if (!query.exec()) {
        qCritical() << "Failed to clear historique:" << query.lastError().text();
        return false;
    }
    qInfo() << "Historique cleared successfully";
    return true;
}

bool HistoriqueEntity::logAjout(const QString &laboratoireNom, const QString &utilisateur)
{
    HistoriqueEntity entry(0, QDateTime::currentDateTime(), laboratoireNom, AJOUT,
                           utilisateur, "Nouveau laboratoire créé");
    return entry.save();
}

bool HistoriqueEntity::logModification(const QString &laboratoireNom, const QString &details,
                                       const QString &utilisateur)
{
    HistoriqueEntity entry(0, QDateTime::currentDateTime(), laboratoireNom, MODIFICATION,
                           utilisateur, details);
    return entry.save();
}

bool HistoriqueEntity::logSuppression(const QString &laboratoireNom, const QString &utilisateur)
{
    HistoriqueEntity entry(0, QDateTime::currentDateTime(), laboratoireNom, SUPPRESSION,
                           utilisateur, "Laboratoire supprimé");
    return entry.save();
}

QString HistoriqueEntity::getTypeStringStatic(TypeOperation type)
{
    switch (type) {
    case AJOUT:        return "Ajout";
    case MODIFICATION: return "Modification";
    case SUPPRESSION:  return "Suppression";
    default:           return "Inconnu";
    }
}
