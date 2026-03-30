#include "database.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

bool Database::initDatabase()
{
    return Connection::createInstance().getDb().isOpen();
}

bool Database::isOpen() const
{
    return Connection::createInstance().getDb().isOpen();
}

bool Database::createEvaluation(const Evaluation& eval)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare(R"(
        INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
        VALUES (:soumission, :relecteur, :note, :comm, TO_DATE(:date, 'YYYY-MM-DD'), :statut)
    )");

    query.bindValue(":soumission", eval.getSubmissionId());
    query.bindValue(":relecteur",  eval.getReviewerId());
    query.bindValue(":note",       eval.getNote());
    query.bindValue(":comm",       eval.getCommentaires());
    query.bindValue(":date",       eval.getDateEvaluation().toString(Qt::ISODate));
    query.bindValue(":statut",     eval.getStatut() == Evaluation::EnCours ? "en_cours" : "terminee");

    if (!query.exec()) {
        qDebug() << "Error inserting evaluation:" << query.lastError();
        return false;
    }
    return true;
}

QVector<Evaluation> Database::getAllEvaluations()
{
    QVector<Evaluation> evaluations;
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT * FROM Evaluation ORDER BY date_evaluation DESC");

    while (query.next()) {
        evaluations.append(Evaluation(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            query.value("date_evaluation").toDate(),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        ));
    }
    return evaluations;
}

QVector<Evaluation> Database::getEvaluationsByReviewer(int reviewerId)
{
    QVector<Evaluation> evaluations;
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("SELECT * FROM Evaluation WHERE relecteur_id = :id ORDER BY date_evaluation DESC");
    query.bindValue(":id", reviewerId);
    query.exec();

    while (query.next()) {
        evaluations.append(Evaluation(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            query.value("date_evaluation").toDate(),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        ));
    }
    return evaluations;
}

QVector<Evaluation> Database::getEvaluationsByStatus(Evaluation::Status status)
{
    QVector<Evaluation> evaluations;
    QSqlQuery query(Connection::createInstance().getDb());
    QString statusStr = (status == Evaluation::EnCours) ? "en_cours" : "terminee";
    query.prepare("SELECT * FROM Evaluation WHERE statut = :status ORDER BY date_evaluation DESC");
    query.bindValue(":status", statusStr);
    query.exec();

    while (query.next()) {
        evaluations.append(Evaluation(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            query.value("date_evaluation").toDate(),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        ));
    }
    return evaluations;
}

bool Database::updateEvaluation(const Evaluation& eval)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare(R"(
        UPDATE Evaluation
        SET soumission_id = :soumission, relecteur_id = :relecteur,
            note = :note, commentaires = :comm, date_evaluation = TO_DATE(:date, 'YYYY-MM-DD'), statut = :statut
        WHERE id_evaluation = :id
    )");

    query.bindValue(":id",        eval.getId());
    query.bindValue(":soumission",eval.getSubmissionId());
    query.bindValue(":relecteur", eval.getReviewerId());
    query.bindValue(":note",      eval.getNote());
    query.bindValue(":comm",      eval.getCommentaires());
    query.bindValue(":date",      eval.getDateEvaluation().toString(Qt::ISODate));
    query.bindValue(":statut",    eval.getStatut() == Evaluation::EnCours ? "en_cours" : "terminee");

    return query.exec();
}

bool Database::deleteEvaluation(int id)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("DELETE FROM Evaluation WHERE id_evaluation = :id");
    query.bindValue(":id", id);
    return query.exec();
}

Evaluation Database::getEvaluationById(int id)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("SELECT * FROM Evaluation WHERE id_evaluation = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        return Evaluation(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            query.value("date_evaluation").toDate(),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        );
    }
    return Evaluation();
}

bool Database::createReviewer(const Reviewer& reviewer)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES (:nom, :prenom, :email, :mdp, 'Relecteur')");
    query.bindValue(":nom",    reviewer.getNom());
    query.bindValue(":prenom", QString("Relecteur"));
    query.bindValue(":email",  reviewer.getEmail());
    query.bindValue(":mdp",    QString("default123"));
    return query.exec();
}

QVector<Reviewer> Database::getAllReviewers()
{
    QVector<Reviewer> reviewers;
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT ID_user, nom, email FROM USERS WHERE Role='Relecteur' ORDER BY nom");

    while (query.next()) {
        reviewers.append(Reviewer(
            query.value("ID_user").toInt(),
            query.value("nom").toString(),
            query.value("email").toString()
        ));
    }
    return reviewers;
}

Reviewer Database::getReviewerById(int id)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("SELECT ID_user, nom, email FROM USERS WHERE ID_user = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        return Reviewer(
            query.value("ID_user").toInt(),
            query.value("nom").toString(),
            query.value("email").toString()
        );
    }
    return Reviewer();
}

double Database::getAverageEvaluationTime(int /*reviewerId*/)
{
    return 45.5; // placeholder — needs timestamp columns
}

double Database::getAverageNote()
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT AVG(note) FROM Evaluation");
    if (query.next()) return query.value(0).toDouble();
    return 0.0;
}

QMap<QString, int> Database::getEvaluationsPerMonth()
{
    QMap<QString, int> result;
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec(
        "SELECT TO_CHAR(date_evaluation, 'YYYY-MM') AS mois, COUNT(*) AS cnt "
        "FROM Evaluation "
        "GROUP BY TO_CHAR(date_evaluation, 'YYYY-MM') "
        "ORDER BY mois"
    );
    while (query.next()) {
        result[query.value("mois").toString()] = query.value("cnt").toInt();
    }
    return result;
}

QMap<int, int> Database::getNoteDistribution()
{
    QMap<int, int> distribution;
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT note, COUNT(*) as count FROM Evaluation GROUP BY note");
    while (query.next()) {
        distribution[query.value("note").toInt()] = query.value("count").toInt();
    }
    return distribution;
}

int Database::getTotalEvaluations()
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT COUNT(*) FROM Evaluation");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

int Database::getPendingEvaluations()
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT COUNT(*) FROM Evaluation WHERE statut = 'en_cours'");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

int Database::getCompletedEvaluations()
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT COUNT(*) FROM Evaluation WHERE statut = 'terminee'");
    if (query.next()) return query.value(0).toInt();
    return 0;
}
