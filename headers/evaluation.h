#ifndef EVALUATION_H
#define EVALUATION_H

#include <QString>
#include <QDate>

class Evaluation
{
public:
    enum Status {
        EnCours,
        Terminee
    };

    Evaluation();
    Evaluation(int id, int submissionId, int reviewerId, int note, 
               const QString& commentaires, const QDate& date, Status statut);

    // Getters
    int getId() const { return id_evaluation; }
    int getSubmissionId() const { return soumission_id; }
    int getReviewerId() const { return relecteur_id; }
    int getNote() const { return note; }
    QString getCommentaires() const { return commentaires; }
    QDate getDateEvaluation() const { return date_evaluation; }
    Status getStatut() const { return statut; }
    QString getStatutString() const;

    // Setters
    void setId(int id) { id_evaluation = id; }
    void setSubmissionId(int id) { soumission_id = id; }
    void setReviewerId(int id) { relecteur_id = id; }
    void setNote(int n) { note = n; }
    void setCommentaires(const QString& c) { commentaires = c; }
    void setDateEvaluation(const QDate& d) { date_evaluation = d; }
    void setStatut(Status s) { statut = s; }

private:
    int id_evaluation;
    int soumission_id;
    int relecteur_id;
    int note;
    QString commentaires;
    QDate date_evaluation;
    Status statut;
};

#endif // EVALUATION_H
