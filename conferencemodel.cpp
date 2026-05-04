#include "conferencemodel.h"

#include "connection.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

ConferenceModel::ConferenceModel()
    : m_idConference(0), m_fraisInscri(0.0), m_idArticle(0)
{
}

ConferenceModel::ConferenceModel(int idConference, const QString &nom, const QString &lieu,
                                 const QDate &dateDebut, const QDate &dateFin,
                                 double fraisInscri, const QString &etat, const QString &theme, int idArticle)
    : m_idConference(idConference), m_nom(nom), m_lieu(lieu), m_dateDebut(dateDebut), m_dateFin(dateFin),
      m_fraisInscri(fraisInscri), m_etat(etat), m_theme(theme), m_idArticle(idArticle)
{
}

bool ConferenceModel::valider(QString &erreur) const
{
    if (m_nom.trimmed().isEmpty()) {
        erreur = "Le nom de la conférence est obligatoire.";
        return false;
    }

    if (m_lieu.trimmed().isEmpty()) {
        erreur = "Le lieu est obligatoire.";
        return false;
    }

    if (!m_dateDebut.isValid()) {
        erreur = "La date de début est invalide.";
        return false;
    }

    if (m_dateFin.isValid() && (m_dateDebut > m_dateFin)) {
        erreur = "La date de début doit être antérieure à la date de fin.";
        return false;
    }

    if (m_fraisInscri < 0) {
        erreur = "Les frais d'inscription ne peuvent pas être négatifs.";
        return false;
    }

    return true;
}

static QSqlDatabase openDbOrLog()
{
    QSqlDatabase db = Connection::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Base de données non connectée";
    }
    return db;
}

bool ConferenceModel::ajouter()
{
    QString erreur;
    if (!valider(erreur)) {
        qDebug() << "Validation échouée:" << erreur;
        return false;
    }

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);

    QString dateDebutStr = m_dateDebut.toString("yyyy-MM-dd");
    QString dateFinStr = m_dateFin.isValid() ? m_dateFin.toString("yyyy-MM-dd") : "NULL";
    QString dateFinSql = (dateFinStr == "NULL") ? "NULL" : QString("TO_DATE('%1', 'YYYY-MM-DD')").arg(dateFinStr);
    QString fraisSql = QString::number(m_fraisInscri, 'f', 2);
    QString idArticleSql = (m_idArticle > 0) ? QString::number(m_idArticle) : "NULL";

    QString sql = QString(
        "INSERT INTO CONFERENCE (ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article) "
        "VALUES (CONFERENCE_SEQ.NEXTVAL, :nom, :lieu, TO_DATE('%1', 'YYYY-MM-DD'), %2, %3, :etat, :theme, %4)"
    ).arg(dateDebutStr, dateFinSql, fraisSql, idArticleSql);

    query.prepare(sql);
    query.bindValue(":nom", m_nom);
    query.bindValue(":lieu", m_lieu);
    query.bindValue(":etat", m_etat);
    query.bindValue(":theme", m_theme);

    if (!query.exec()) {
        const QString firstError = query.lastError().text();
        qDebug() << "Erreur lors de l'ajout (tentative principale):" << firstError;
        qDebug() << "SQL tenté:" << sql;

        QString sqlFallback = QString(
            "INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article) "
            "VALUES (:nom, :lieu, TO_DATE('%1', 'YYYY-MM-DD'), %2, %3, :etat, :theme, %4)"
        ).arg(dateDebutStr, dateFinSql, fraisSql, idArticleSql);

        QSqlQuery fallback(db);
        fallback.prepare(sqlFallback);
        fallback.bindValue(":nom", m_nom);
        fallback.bindValue(":lieu", m_lieu);
        fallback.bindValue(":etat", m_etat);
        fallback.bindValue(":theme", m_theme);

        if (!fallback.exec()) {
            qDebug() << "Erreur lors de l'ajout (fallback):" << fallback.lastError().text();
            return false;
        }
    }

    qDebug() << "Conférence ajoutée avec succès";
    return true;
}

bool ConferenceModel::modifier()
{
    QString erreur;
    if (!valider(erreur)) {
        qDebug() << "Validation échouée:" << erreur;
        return false;
    }

    if (m_idConference <= 0) {
        qDebug() << "ID invalide pour la modification:" << m_idConference;
        return false;
    }

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return false;
    }

    QString dateDebutStr = m_dateDebut.toString("yyyy-MM-dd");
    QString dateFinStr = m_dateFin.isValid() ? m_dateFin.toString("yyyy-MM-dd") : "NULL";
    QString dateFinSql = (dateFinStr == "NULL") ? "NULL" : QString("TO_DATE('%1', 'YYYY-MM-DD')").arg(dateFinStr);
    QString fraisSql = QString::number(m_fraisInscri, 'f', 2);
    QString idArticleSql = (m_idArticle > 0) ? QString::number(m_idArticle) : "NULL";

    QString sql = QString(
        "UPDATE CONFERENCE SET "
        "nom          = :nom, "
        "lieu         = :lieu, "
        "Date_debut   = TO_DATE('%1', 'YYYY-MM-DD'), "
        "Date_fin     = %2, "
        "frais_inscri = %3, "
        "etat         = :etat, "
        "theme        = :theme, "
        "ID_article   = %4 "
        "WHERE ID_conference = %5"
    ).arg(dateDebutStr, dateFinSql, fraisSql, idArticleSql, QString::number(m_idConference));

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":nom", m_nom);
    query.bindValue(":lieu", m_lieu);
    query.bindValue(":etat", m_etat);
    query.bindValue(":theme", m_theme);

    if (!query.exec()) {
        qDebug() << "=== ERREUR MODIFICATION CONFÉRENCE ===";
        qDebug() << "SQL Error   :" << query.lastError().text();
        qDebug() << "SQL tenté   :" << sql;
        return false;
    }

    qDebug() << "Conférence modifiée avec succès (ID:" << m_idConference << ")";
    return true;
}

bool ConferenceModel::supprimer()
{
    if (m_idConference <= 0) {
        qDebug() << "ID invalide pour suppression";
        return false;
    }

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM CONFERENCE WHERE ID_conference = :id");
    query.bindValue(":id", m_idConference);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }

    qDebug() << "Conférence supprimée avec succès";
    return true;
}

QList<ConferenceModel> ConferenceModel::afficherTous()
{
    QList<ConferenceModel> conferences;

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return conferences;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                    "FROM CONFERENCE ORDER BY Date_debut DESC")) {
        qDebug() << "Erreur lors de la récupération:" << query.lastError().text();
        return conferences;
    }

    while (query.next()) {
        ConferenceModel conf;
        conf.setIdConference(query.value(0).toInt());
        conf.setNom(query.value(1).toString());
        conf.setLieu(query.value(2).toString());
        conf.setDateDebut(query.value(3).toDate());
        conf.setDateFin(query.value(4).toDate());
        conf.setFraisInscri(query.value(5).toDouble());
        conf.setEtat(query.value(6).toString());
        conf.setTheme(query.value(7).toString());
        conf.setIdArticle(query.value(8).toInt());
        conferences.append(conf);
    }

    return conferences;
}

QList<ConferenceModel> ConferenceModel::rechercher(const QString &critere, const QString &valeur)
{
    QList<ConferenceModel> conferences;

    if (valeur.trimmed().isEmpty()) {
        return afficherTous();
    }

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return conferences;
    }

    QString sql = "SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article FROM CONFERENCE WHERE ";

    if (critere == "Rechercher par Nom") {
        sql += "UPPER(nom) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Lieu") {
        sql += "UPPER(lieu) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Thème") {
        sql += "UPPER(theme) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Date") {
        sql += "TO_CHAR(Date_debut, 'YYYY-MM-DD') LIKE :valeur OR TO_CHAR(Date_fin, 'YYYY-MM-DD') LIKE :valeur";
    } else {
        return afficherTous();
    }

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":valeur", "%" + valeur + "%");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche:" << query.lastError().text();
        return conferences;
    }

    while (query.next()) {
        ConferenceModel conf;
        conf.setIdConference(query.value(0).toInt());
        conf.setNom(query.value(1).toString());
        conf.setLieu(query.value(2).toString());
        conf.setDateDebut(query.value(3).toDate());
        conf.setDateFin(query.value(4).toDate());
        conf.setFraisInscri(query.value(5).toDouble());
        conf.setEtat(query.value(6).toString());
        conf.setTheme(query.value(7).toString());
        conf.setIdArticle(query.value(8).toInt());
        conferences.append(conf);
    }

    return conferences;
}

QList<ConferenceModel> ConferenceModel::trierPar(const QString &colonne, bool croissant)
{
    QList<ConferenceModel> conferences;

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return conferences;
    }

    QString colonneSQL;
    if (colonne == "Nom") {
        colonneSQL = "nom";
    } else if (colonne == "Lieu") {
        colonneSQL = "lieu";
    } else if (colonne == "Date") {
        colonneSQL = "Date_debut";
    } else if (colonne == "Frais") {
        colonneSQL = "frais_inscri";
    } else if (colonne == "État") {
        colonneSQL = "etat";
    } else if (colonne == "Thème") {
        colonneSQL = "theme";
    } else {
        colonneSQL = "Date_debut";
    }

    QString ordre = croissant ? "ASC" : "DESC";

    QString sql = QString("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                          "FROM CONFERENCE ORDER BY %1 %2")
                      .arg(colonneSQL, ordre);

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qDebug() << "Erreur lors du tri:" << query.lastError().text();
        return conferences;
    }

    while (query.next()) {
        ConferenceModel conf;
        conf.setIdConference(query.value(0).toInt());
        conf.setNom(query.value(1).toString());
        conf.setLieu(query.value(2).toString());
        conf.setDateDebut(query.value(3).toDate());
        conf.setDateFin(query.value(4).toDate());
        conf.setFraisInscri(query.value(5).toDouble());
        conf.setEtat(query.value(6).toString());
        conf.setTheme(query.value(7).toString());
        conf.setIdArticle(query.value(8).toInt());
        conferences.append(conf);
    }

    return conferences;
}

ConferenceModel ConferenceModel::getById(int id)
{
    ConferenceModel conf;

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return conf;
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID_conference, nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme, ID_article "
                  "FROM CONFERENCE WHERE ID_conference = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération par ID:" << query.lastError().text();
        return conf;
    }

    if (query.next()) {
        conf.setIdConference(query.value(0).toInt());
        conf.setNom(query.value(1).toString());
        conf.setLieu(query.value(2).toString());
        conf.setDateDebut(query.value(3).toDate());
        conf.setDateFin(query.value(4).toDate());
        conf.setFraisInscri(query.value(5).toDouble());
        conf.setEtat(query.value(6).toString());
        conf.setTheme(query.value(7).toString());
        conf.setIdArticle(query.value(8).toInt());
    }

    return conf;
}

QMap<QString, int> ConferenceModel::getStatsParTheme()
{
    QMap<QString, int> stats;

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return stats;
    }

    QSqlQuery query(db);
    if (query.exec("SELECT theme, COUNT(*) FROM CONFERENCE GROUP BY theme")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    }

    return stats;
}

QMap<QString, int> ConferenceModel::getStatsParEtat()
{
    QMap<QString, int> stats;

    QSqlDatabase db = openDbOrLog();
    if (!db.isOpen()) {
        return stats;
    }

    QSqlQuery query(db);
    if (query.exec("SELECT etat, COUNT(*) FROM CONFERENCE GROUP BY etat")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    }

    return stats;
}
