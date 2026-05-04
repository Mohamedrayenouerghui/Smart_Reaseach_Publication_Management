#ifndef DATABASE_H
#define DATABASE_H

#include <QVector>
#include <QMap>
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
    double getAverageNote();
    QMap<int, int> getNoteDistribution();
    QMap<QString, int> getEvaluationsPerMonth();
    int getTotalEvaluations();
    int getPendingEvaluations();
    int getCompletedEvaluations();

private:
    Database() = default;
    ~Database() = default;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};

#endif // DATABASE_H
