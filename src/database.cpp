#include "headers/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

Database::Database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("evaluations.db");
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::initDatabase()
{
    if (!db.open()) {
        qDebug() << "Error: connection with database failed -" << db.lastError();
        return false;
    }
    
    return createTables();
}

bool Database::isOpen() const
{
    return db.isOpen();
}

bool Database::createTables()
{
    QSqlQuery query;
    
    // Create Reviewer table
    QString createReviewerTable = R"(
        CREATE TABLE IF NOT EXISTS Reviewer (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nom TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL
        )
    )";
    
    if (!query.exec(createReviewerTable)) {
        qDebug() << "Error creating Reviewer table:" << query.lastError();
        return false;
    }
    
    // Create Evaluation table
    QString createEvaluationTable = R"(
        CREATE TABLE IF NOT EXISTS Evaluation (
            id_evaluation INTEGER PRIMARY KEY AUTOINCREMENT,
            soumission_id INTEGER NOT NULL,
            relecteur_id INTEGER NOT NULL,
            note INTEGER CHECK(note >= 1 AND note <= 10),
            commentaires TEXT,
            date_evaluation DATE NOT NULL,
            statut TEXT CHECK(statut IN ('en_cours', 'terminee')),
            FOREIGN KEY (relecteur_id) REFERENCES Reviewer(id)
        )
    )";
    
    if (!query.exec(createEvaluationTable)) {
        qDebug() << "Error creating Evaluation table:" << query.lastError();
        return false;
    }
    
    // Insert sample reviewers if table is empty
    query.exec("SELECT COUNT(*) FROM Reviewer");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO Reviewer (nom, email) VALUES ('Dr. Martin', 'martin@university.edu')");
        query.exec("INSERT INTO Reviewer (nom, email) VALUES ('Prof. Dubois', 'dubois@university.edu')");
        query.exec("INSERT INTO Reviewer (nom, email) VALUES ('Dr. Laurent', 'laurent@university.edu')");
    }
    
    return true;
}

bool Database::createEvaluation(const Evaluation& eval)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
        VALUES (:soumission, :relecteur, :note, :comm, :date, :statut)
    )");
    
    query.bindValue(":soumission", eval.getSubmissionId());
    query.bindValue(":relecteur", eval.getReviewerId());
    query.bindValue(":note", eval.getNote());
    query.bindValue(":comm", eval.getCommentaires());
    query.bindValue(":date", eval.getDateEvaluation().toString(Qt::ISODate));
    query.bindValue(":statut", eval.getStatut() == Evaluation::EnCours ? "en_cours" : "terminee");
    
    if (!query.exec()) {
        qDebug() << "Error inserting evaluation:" << query.lastError();
        return false;
    }
    
    return true;
}

QVector<Evaluation> Database::getAllEvaluations()
{
    QVector<Evaluation> evaluations;
    QSqlQuery query("SELECT * FROM Evaluation ORDER BY date_evaluation DESC");
    
    while (query.next()) {
        Evaluation eval(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            QDate::fromString(query.value("date_evaluation").toString(), Qt::ISODate),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        );
        evaluations.append(eval);
    }
    
    return evaluations;
}

QVector<Evaluation> Database::getEvaluationsByReviewer(int reviewerId)
{
    QVector<Evaluation> evaluations;
    QSqlQuery query;
    query.prepare("SELECT * FROM Evaluation WHERE relecteur_id = :id ORDER BY date_evaluation DESC");
    query.bindValue(":id", reviewerId);
    query.exec();
    
    while (query.next()) {
        Evaluation eval(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            QDate::fromString(query.value("date_evaluation").toString(), Qt::ISODate),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        );
        evaluations.append(eval);
    }
    
    return evaluations;
}

QVector<Evaluation> Database::getEvaluationsByStatus(Evaluation::Status status)
{
    QVector<Evaluation> evaluations;
    QSqlQuery query;
    QString statusStr = (status == Evaluation::EnCours) ? "en_cours" : "terminee";
    query.prepare("SELECT * FROM Evaluation WHERE statut = :status ORDER BY date_evaluation DESC");
    query.bindValue(":status", statusStr);
    query.exec();
    
    while (query.next()) {
        Evaluation eval(
            query.value("id_evaluation").toInt(),
            query.value("soumission_id").toInt(),
            query.value("relecteur_id").toInt(),
            query.value("note").toInt(),
            query.value("commentaires").toString(),
            QDate::fromString(query.value("date_evaluation").toString(), Qt::ISODate),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        );
        evaluations.append(eval);
    }
    
    return evaluations;
}

bool Database::updateEvaluation(const Evaluation& eval)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE Evaluation 
        SET soumission_id = :soumission, relecteur_id = :relecteur, 
            note = :note, commentaires = :comm, date_evaluation = :date, statut = :statut
        WHERE id_evaluation = :id
    )");
    
    query.bindValue(":id", eval.getId());
    query.bindValue(":soumission", eval.getSubmissionId());
    query.bindValue(":relecteur", eval.getReviewerId());
    query.bindValue(":note", eval.getNote());
    query.bindValue(":comm", eval.getCommentaires());
    query.bindValue(":date", eval.getDateEvaluation().toString(Qt::ISODate));
    query.bindValue(":statut", eval.getStatut() == Evaluation::EnCours ? "en_cours" : "terminee");
    
    return query.exec();
}

bool Database::deleteEvaluation(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Evaluation WHERE id_evaluation = :id");
    query.bindValue(":id", id);
    
    return query.exec();
}

Evaluation Database::getEvaluationById(int id)
{
    QSqlQuery query;
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
            QDate::fromString(query.value("date_evaluation").toString(), Qt::ISODate),
            query.value("statut").toString() == "en_cours" ? Evaluation::EnCours : Evaluation::Terminee
        );
    }
    
    return Evaluation();
}

bool Database::createReviewer(const Reviewer& reviewer)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Reviewer (nom, email) VALUES (:nom, :email)");
    query.bindValue(":nom", reviewer.getNom());
    query.bindValue(":email", reviewer.getEmail());
    
    return query.exec();
}

QVector<Reviewer> Database::getAllReviewers()
{
    QVector<Reviewer> reviewers;
    QSqlQuery query("SELECT * FROM Reviewer ORDER BY nom");
    
    while (query.next()) {
        reviewers.append(Reviewer(
            query.value("id").toInt(),
            query.value("nom").toString(),
            query.value("email").toString()
        ));
    }
    
    return reviewers;
}

Reviewer Database::getReviewerById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Reviewer WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    
    if (query.next()) {
        return Reviewer(
            query.value("id").toInt(),
            query.value("nom").toString(),
            query.value("email").toString()
        );
    }
    
    return Reviewer();
}

double Database::getAverageEvaluationTime(int reviewerId)
{
    // Placeholder: would need additional timestamp data
    return 45.5; // minutes
}

QMap<int, int> Database::getNoteDistribution()
{
    QMap<int, int> distribution;
    QSqlQuery query("SELECT note, COUNT(*) as count FROM Evaluation GROUP BY note");
    
    while (query.next()) {
        distribution[query.value("note").toInt()] = query.value("count").toInt();
    }
    
    return distribution;
}

int Database::getTotalEvaluations()
{
    QSqlQuery query("SELECT COUNT(*) FROM Evaluation");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Database::getPendingEvaluations()
{
    QSqlQuery query("SELECT COUNT(*) FROM Evaluation WHERE statut = 'en_cours'");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Database::getCompletedEvaluations()
{
    QSqlQuery query("SELECT COUNT(*) FROM Evaluation WHERE statut = 'terminee'");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
