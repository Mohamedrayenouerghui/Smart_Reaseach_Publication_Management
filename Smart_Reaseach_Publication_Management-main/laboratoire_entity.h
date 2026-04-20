#ifndef LABORATOIRE_ENTITY_H
#define LABORATOIRE_ENTITY_H

#include <QString>
#include <QDate>
#include <QList>
#include <QSqlDatabase>

class LaboratoireEntity
{
public:
    LaboratoireEntity();
    LaboratoireEntity(int id, const QString &nom, double budget, const QDate &dateCreation,
                      const QString &domaine, const QString &localisation, int nombreChercheurs,
                      const QString &classement, const QString &historique, int idUser);

    // Getters
    int getIdLabo() const { return m_idLabo; }
    QString getNom() const { return m_nom; }
    double getBudget() const { return m_budget; }
    QDate getDateCreation() const { return m_dateCreation; }
    QString getDomaine() const { return m_domaine; }
    QString getLocalisation() const { return m_localisation; }
    int getNombreChercheurs() const { return m_nombreChercheurs; }
    QString getClassement() const { return m_classement; }
    QString getHistorique() const { return m_historique; }
    int getIdUser() const { return m_idUser; }
    QString getValidationError() const;

    // Setters
    void setIdLabo(int id) { m_idLabo = id; }
    void setNom(const QString &nom) { m_nom = nom; }
    void setBudget(double budget) { m_budget = budget; }
    void setDateCreation(const QDate &date) { m_dateCreation = date; }
    void setDomaine(const QString &domaine) { m_domaine = domaine; }
    void setLocalisation(const QString &localisation) { m_localisation = localisation; }
    void setNombreChercheurs(int n) { m_nombreChercheurs = n; }
    void setClassement(const QString &classement) { m_classement = classement; }
    void setHistorique(const QString &historique) { m_historique = historique; }
    void setIdUser(int id) { m_idUser = id; }

    // Validation
    bool isValid() const;
    bool validate();

    // Database operations
    bool save();
    bool insert();
    bool update();
    bool remove();

    // Static queries
    static LaboratoireEntity getById(int id);
    static QList<LaboratoireEntity> getAll();
    static QList<LaboratoireEntity> search(const QString &searchTerm);
    static QList<LaboratoireEntity> getByDomaine(const QString &domaine);
    static QList<LaboratoireEntity> getByUser(int idUser);

private:
    int m_idLabo;
    QString m_nom;
    double m_budget;
    QDate m_dateCreation;
    QString m_domaine;
    QString m_localisation;
    int m_nombreChercheurs;
    QString m_classement;
    QString m_historique;
    int m_idUser;
    QString m_validationError;
};

#endif // LABORATOIRE_ENTITY_H
