#include "laboratoire_entity.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

static QString formatDateIso(const QDate &date)
{
    return date.isValid() ? date.toString("yyyy-MM-dd") : QString();
}

LaboratoireEntity::LaboratoireEntity()
    : m_idLabo(0), m_budget(0.0), m_nombreChercheurs(0), m_idUser(0)
{}

LaboratoireEntity::LaboratoireEntity(int id, const QString &nom, double budget,
                                     const QDate &dateCreation, const QString &domaine,
                                     const QString &localisation, int nombreChercheurs,
                                     const QString &classement, const QString &historique, int idUser)
    : m_idLabo(id), m_nom(nom), m_budget(budget), m_dateCreation(dateCreation),
      m_domaine(domaine), m_localisation(localisation), m_nombreChercheurs(nombreChercheurs),
      m_classement(classement), m_historique(historique), m_idUser(idUser)
{}

bool LaboratoireEntity::validate()
{
    m_validationError.clear();
    if (m_nom.trimmed().isEmpty())          { m_validationError = "Le nom du laboratoire est requis"; return false; }
    if (m_domaine.trimmed().isEmpty())      { m_validationError = "Le domaine est requis";            return false; }
    if (m_localisation.trimmed().isEmpty()) { m_validationError = "La localisation est requise";      return false; }
    if (!m_dateCreation.isValid())          { m_validationError = "La date de création est invalide"; return false; }
    if (m_budget < 0)                       { m_validationError = "Le budget ne peut pas être négatif"; return false; }
    if (m_nombreChercheurs < 0)             { m_validationError = "Le nombre de chercheurs ne peut pas être négatif"; return false; }
    return true;
}

bool LaboratoireEntity::isValid() const
{
    return !m_nom.isEmpty() && !m_domaine.isEmpty() && !m_localisation.isEmpty() && m_dateCreation.isValid();
}

QString LaboratoireEntity::getValidationError() const
{
    return m_validationError;
}

bool LaboratoireEntity::save()
{
    return m_idLabo > 0 ? update() : insert();
}

bool LaboratoireEntity::insert()
{
    if (!validate()) { qWarning() << "Validation failed:" << m_validationError; return false; }

    QSqlDatabase &db = Connection::createInstance().getDb();
    if (!db.isOpen()) {
        qCritical() << "Database is not open!";
        m_validationError = "Base de données non connectée";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO LABORATOIRE (ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user) "
                  "VALUES (SEQ_LABO.NEXTVAL, ?, ?, TO_DATE(?, 'YYYY-MM-DD'), ?, ?, ?, ?, ?, ?)");
    query.addBindValue(m_nom);
    query.addBindValue(m_budget);
    query.addBindValue(formatDateIso(m_dateCreation));
    query.addBindValue(m_domaine);
    query.addBindValue(m_localisation);
    query.addBindValue(m_nombreChercheurs);
    query.addBindValue(m_classement);
    query.addBindValue(m_historique);
    query.addBindValue(m_idUser > 0 ? QVariant(m_idUser) : QVariant());

    qDebug() << "Executing INSERT with values:";
    qDebug() << "  nom:" << m_nom;
    qDebug() << "  budget:" << m_budget << "(type:" << QVariant(m_budget).typeName() << ")";
    qDebug() << "  chercheurs:" << m_nombreChercheurs << "(type:" << QVariant(m_nombreChercheurs).typeName() << ")";
    qDebug() << "  iduser:" << (m_idUser > 0 ? QString::number(m_idUser) : "NULL");
    
    if (!query.exec()) {
        qCritical() << "Failed to insert laboratoire:" << query.lastError().text();
        qCritical() << "Driver text:" << query.lastError().driverText();
        qCritical() << "Database text:" << query.lastError().databaseText();
        qCritical() << "Bound values:" << query.boundValues();
        m_validationError = query.lastError().text();
        return false;
    }

    m_idLabo = query.lastInsertId().toInt();
    qInfo() << "Laboratoire inserted successfully with ID:" << m_idLabo;
    return true;
}

bool LaboratoireEntity::update()
{
    if (m_idLabo <= 0) { qWarning() << "Cannot UPDATE without valid ID"; return false; }
    if (!validate())   { qWarning() << "Validation failed:" << m_validationError; return false; }

    QSqlDatabase &db = Connection::createInstance().getDb();
    if (!db.isOpen()) { qCritical() << "Database is not open!"; return false; }

    QSqlQuery query(db);
    query.prepare("UPDATE LABORATOIRE SET nom = ?, budget = ?, "
                  "Date_creation = TO_DATE(?, 'YYYY-MM-DD'), "
                  "domaine = ?, localisation = ?, "
                  "nombre_chercheurs = ?, classement = ?, "
                  "historique = ?, ID_user = ? WHERE ID_labo = ?");
    query.addBindValue(m_nom);
    query.addBindValue(m_budget);
    query.addBindValue(formatDateIso(m_dateCreation));
    query.addBindValue(m_domaine);
    query.addBindValue(m_localisation);
    query.addBindValue(m_nombreChercheurs);
    query.addBindValue(m_classement);
    query.addBindValue(m_historique);
    query.addBindValue(m_idUser > 0 ? QVariant(m_idUser) : QVariant());
    query.addBindValue(m_idLabo);

    if (!query.exec()) {
        qCritical() << "Failed to update laboratoire:" << query.lastError().text();
        return false;
    }

    qInfo() << "Laboratoire updated successfully";
    return true;
}

bool LaboratoireEntity::remove()
{
    if (m_idLabo <= 0) { qWarning() << "Cannot delete without valid ID"; return false; }

    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("DELETE FROM LABORATOIRE WHERE ID_labo = :id");
    query.bindValue(":id", m_idLabo);

    if (!query.exec()) {
        qCritical() << "Failed to delete laboratoire:" << query.lastError().text();
        return false;
    }

    qInfo() << "Laboratoire deleted successfully";
    m_idLabo = 0;
    return true;
}

LaboratoireEntity LaboratoireEntity::getById(int id)
{
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user FROM LABORATOIRE WHERE ID_labo = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return LaboratoireEntity(query.value(0).toInt(), query.value(1).toString(),
                                 query.value(2).toDouble(), query.value(3).toDate(),
                                 query.value(4).toString(), query.value(5).toString(),
                                 query.value(6).toInt(), query.value(7).toString(),
                                 query.value(8).toString(), query.value(9).toInt());
    }
    return LaboratoireEntity();
}

QList<LaboratoireEntity> LaboratoireEntity::getAll()
{
    QList<LaboratoireEntity> labos;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user FROM LABORATOIRE ORDER BY nom");

    if (query.exec()) {
        while (query.next()) {
            labos.append(LaboratoireEntity(query.value(0).toInt(), query.value(1).toString(),
                                           query.value(2).toDouble(), query.value(3).toDate(),
                                           query.value(4).toString(), query.value(5).toString(),
                                           query.value(6).toInt(), query.value(7).toString(),
                                           query.value(8).toString(), query.value(9).toInt()));
        }
    }
    return labos;
}

QList<LaboratoireEntity> LaboratoireEntity::search(const QString &searchTerm)
{
    QList<LaboratoireEntity> labos;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user FROM LABORATOIRE "
                  "WHERE nom LIKE :p1 OR domaine LIKE :p2 OR localisation LIKE :p3 ORDER BY nom");
    QString p = "%" + searchTerm + "%";
    query.bindValue(":p1", p);
    query.bindValue(":p2", p);
    query.bindValue(":p3", p);

    if (query.exec()) {
        while (query.next()) {
            labos.append(LaboratoireEntity(query.value(0).toInt(), query.value(1).toString(),
                                           query.value(2).toDouble(), query.value(3).toDate(),
                                           query.value(4).toString(), query.value(5).toString(),
                                           query.value(6).toInt(), query.value(7).toString(),
                                           query.value(8).toString(), query.value(9).toInt()));
        }
    }
    return labos;
}

QList<LaboratoireEntity> LaboratoireEntity::getByDomaine(const QString &domaine)
{
    QList<LaboratoireEntity> labos;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user FROM LABORATOIRE "
                  "WHERE domaine = :domaine ORDER BY nom");
    query.bindValue(":domaine", domaine);

    if (query.exec()) {
        while (query.next()) {
            labos.append(LaboratoireEntity(query.value(0).toInt(), query.value(1).toString(),
                                           query.value(2).toDouble(), query.value(3).toDate(),
                                           query.value(4).toString(), query.value(5).toString(),
                                           query.value(6).toInt(), query.value(7).toString(),
                                           query.value(8).toString(), query.value(9).toInt()));
        }
    }
    return labos;
}

QList<LaboratoireEntity> LaboratoireEntity::getByUser(int idUser)
{
    QList<LaboratoireEntity> labos;
    QSqlDatabase &db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("SELECT ID_labo, nom, budget, Date_creation, domaine, localisation, "
                  "nombre_chercheurs, classement, historique, ID_user FROM LABORATOIRE "
                  "WHERE ID_user = :iduser ORDER BY nom");
    query.bindValue(":iduser", idUser);

    if (query.exec()) {
        while (query.next()) {
            labos.append(LaboratoireEntity(query.value(0).toInt(), query.value(1).toString(),
                                           query.value(2).toDouble(), query.value(3).toDate(),
                                           query.value(4).toString(), query.value(5).toString(),
                                           query.value(6).toInt(), query.value(7).toString(),
                                           query.value(8).toString(), query.value(9).toInt()));
        }
    }
    return labos;
}
