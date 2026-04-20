#ifndef HISTORIQUE_ENTITY_H
#define HISTORIQUE_ENTITY_H

#include <QString>
#include <QDateTime>
#include <QSqlDatabase>

class HistoriqueEntity
{
public:
    enum TypeOperation {
        AJOUT,
        MODIFICATION,
        SUPPRESSION
    };

    HistoriqueEntity();
    HistoriqueEntity(int id, const QDateTime &date, const QString &laboratoireNom,
                     TypeOperation type, const QString &utilisateur, const QString &details);

    // Getters
    int getId() const { return m_id; }
    QDateTime getDate() const { return m_date; }
    QString getLaboratoireNom() const { return m_laboratoireNom; }
    TypeOperation getType() const { return m_type; }
    QString getTypeString() const;
    QString getUtilisateur() const { return m_utilisateur; }
    QString getDetails() const { return m_details; }

    // Setters
    void setId(int id) { m_id = id; }
    void setDate(const QDateTime &date) { m_date = date; }
    void setLaboratoireNom(const QString &nom) { m_laboratoireNom = nom; }
    void setType(TypeOperation type) { m_type = type; }
    void setUtilisateur(const QString &utilisateur) { m_utilisateur = utilisateur; }
    void setDetails(const QString &details) { m_details = details; }

    // Database operations
    bool save();
    bool insert();
    static QList<HistoriqueEntity> getAll();
    static QList<HistoriqueEntity> getByLaboratoire(const QString &laboratoireNom);
    static QList<HistoriqueEntity> getByType(TypeOperation type);
    static bool clearAll();

    // Static factory methods for logging
    static bool logAjout(const QString &laboratoireNom, const QString &utilisateur = "admin");
    static bool logModification(const QString &laboratoireNom, const QString &details, const QString &utilisateur = "admin");
    static bool logSuppression(const QString &laboratoireNom, const QString &utilisateur = "admin");

private:
    int m_id;
    QDateTime m_date;
    QString m_laboratoireNom;
    TypeOperation m_type;
    QString m_utilisateur;
    QString m_details;

    static QString getTypeStringStatic(TypeOperation type);
};

#endif // HISTORIQUE_ENTITY_H
