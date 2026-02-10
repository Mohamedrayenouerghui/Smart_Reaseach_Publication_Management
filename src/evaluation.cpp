#include "headers/evaluation.h"

Evaluation::Evaluation()
    : id_evaluation(0), soumission_id(0), relecteur_id(0), note(5),
      date_evaluation(QDate::currentDate()), statut(EnCours)
{
}

Evaluation::Evaluation(int id, int submissionId, int reviewerId, int n,
                       const QString& comm, const QDate& date, Status stat)
    : id_evaluation(id), soumission_id(submissionId), relecteur_id(reviewerId),
      note(n), commentaires(comm), date_evaluation(date), statut(stat)
{
}

QString Evaluation::getStatutString() const
{
    return (statut == EnCours) ? "En cours" : "Terminée";
}
