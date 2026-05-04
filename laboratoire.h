#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include <QWidget>
#include <QDate>
#include <QList>
#include <QMap>

namespace Ui {
class Laboratoire;
}

// ── Structure de données ──────────────────────────────────────────
struct LaboratoireData {
    int     id;
    QString nom;
    double  budget;
    QDate   dateCreation;
    QString domaine;
    QString localisation;
    int     nbEmployes;
    QString classement;
    QString historique;   // CLOB Oracle
};

// ── Classe modèle (logique métier + accès Oracle) ─────────────────
class LaboratoireModel
{
public:
    LaboratoireModel();
    LaboratoireModel(int id, const QString &nom, double budget,
                     const QDate &dateCreation, const QString &domaine,
                     const QString &localisation, int nbEmployes,
                     const QString &classement, const QString &historique = QString());

    // Getters
    int     getId()           const { return m_id; }
    QString getNom()          const { return m_nom; }
    double  getBudget()       const { return m_budget; }
    QDate   getDateCreation() const { return m_dateCreation; }
    QString getDomaine()      const { return m_domaine; }
    QString getLocalisation() const { return m_localisation; }
    int     getNbEmployes()   const { return m_nbEmployes; }
    QString getClassement()   const { return m_classement; }
    QString getHistorique()   const { return m_historique; }

    // Setters
    void setId(int v)                     { m_id = v; }
    void setNom(const QString &v)         { m_nom = v; }
    void setBudget(double v)              { m_budget = v; }
    void setDateCreation(const QDate &v)  { m_dateCreation = v; }
    void setDomaine(const QString &v)     { m_domaine = v; }
    void setLocalisation(const QString &v){ m_localisation = v; }
    void setNbEmployes(int v)             { m_nbEmployes = v; }
    void setClassement(const QString &v)  { m_classement = v; }
    void setHistorique(const QString &v)  { m_historique = v; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer();

    // Requêtes statiques
    static QList<LaboratoireModel> afficherTous();
    static QList<LaboratoireModel> rechercher(const QString &critere, const QString &valeur);
    static QList<LaboratoireModel> trierPar(const QString &colonne, bool croissant = true);
    static LaboratoireModel        getById(int id);

    // Statistiques
    static QMap<QString, int>    getStatsParDomaine();
    static QMap<QString, int>    getStatsParClassement();
    static QMap<QString, double> getBudgetMoyenParDomaine();
    static QList<LaboratoireModel> getLabsDepassantBudget(double seuilPct, double budgetRef);

    // Validation
    bool valider(QString &erreur) const;

    // Dernier message d'erreur SQL
    QString getLastError() const { return m_lastError; }

private:
    mutable QString m_lastError;
    int     m_id;
    QString m_nom;
    double  m_budget;
    QDate   m_dateCreation;
    QString m_domaine;
    QString m_localisation;
    int     m_nbEmployes;
    QString m_classement;
    QString m_historique;
};

// ── Widget principal ──────────────────────────────────────────────
class Laboratoire : public QWidget
{
    Q_OBJECT

public:
    explicit Laboratoire(QWidget *parent = nullptr);
    ~Laboratoire();

private slots:
    void onAjouterClicked();
    void onAjouterFormClicked();
    void onModifierClicked();
    void onModifierFormClicked();
    void onAnnulerModifClicked();
    void onSupprimerClicked();
    void onRechercherClicked();
    void onTrierClicked();
    void onExporterPDFClicked();
    void onExporterWordClicked();
    void onStatistiquesClicked();
    void onClearSearchClicked();
    void onHistoriqueClicked();
    void onAlerteBudgetClicked();
    void onSupprimerHistoriqueClicked();
    void onExporterHistPDFClicked();
    void refreshHistorique();

private:
    Ui::Laboratoire *ui;
    // Stocke l'ID du labo sélectionné (pour la table sans colonne ID)
    QList<int> m_labIds;
    void setupConnections();
    void refreshTable(const QList<LaboratoireData> &labs);
    LaboratoireData readFormData() const;
    void populateModifierForm(const LaboratoireData &lab);
    void verifierAlertesBudget(bool silencieux = false);
};

#endif // LABORATOIRE_H
