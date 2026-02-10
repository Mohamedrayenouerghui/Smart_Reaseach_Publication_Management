#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QVector>
#include "evaluation.h"
#include "reviewer.h"

class Database
{
public:
    static Database& instance();
    
    bool initDatabase();
    bool isOpen() const;
    
    // Evaluation CRUD operations
    bool createEvaluation(const Evaluation& eval);
    QVector<Evaluation> getAllEvaluations();
    QVector<Evaluation> getEvaluationsByReviewer(int reviewerId);
    QVector<Evaluation> getEvaluationsByStatus(Evaluation::Status status);
    bool updateEvaluation(const Evaluation& eval);
    bool deleteEvaluation(int id);
    Evaluation getEvaluationById(int id);
    
    // Reviewer operations
    bool createReviewer(const Reviewer& reviewer);
    QVector<Reviewer> getAllReviewers();
    Reviewer getReviewerById(int id);
    
    // Statistics
    double getAverageEvaluationTime(int reviewerId);
    QMap<int, int> getNoteDistribution();
    int getTotalEvaluations();
    int getPendingEvaluations();
    int getCompletedEvaluations();

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    QSqlDatabase db;
    bool createTables();
};

#endif // DATABASE_H
