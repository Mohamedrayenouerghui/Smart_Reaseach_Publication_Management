#ifndef CONFERENCEMODEL_H
#define CONFERENCEMODEL_H

#include <QDate>
#include <QList>
#include <QMap>
#include <QString>

class ConferenceModel
{
public:
    ConferenceModel();
    ConferenceModel(int idConference, const QString &nom, const QString &lieu,
                    const QDate &dateDebut, const QDate &dateFin,
                    double fraisInscri, const QString &etat, const QString &theme, int idArticle = 0);

    int getIdConference() const { return m_idConference; }
    QString getNom() const { return m_nom; }
    QString getLieu() const { return m_lieu; }
    QDate getDateDebut() const { return m_dateDebut; }
    QDate getDateFin() const { return m_dateFin; }
    double getFraisInscri() const { return m_fraisInscri; }
    QString getEtat() const { return m_etat; }
    QString getTheme() const { return m_theme; }
    int getIdArticle() const { return m_idArticle; }

    void setIdConference(int id) { m_idConference = id; }
    void setNom(const QString &nom) { m_nom = nom; }
    void setLieu(const QString &lieu) { m_lieu = lieu; }
    void setDateDebut(const QDate &date) { m_dateDebut = date; }
    void setDateFin(const QDate &date) { m_dateFin = date; }
    void setFraisInscri(double frais) { m_fraisInscri = frais; }
    void setEtat(const QString &etat) { m_etat = etat; }
    void setTheme(const QString &theme) { m_theme = theme; }
    void setIdArticle(int id) { m_idArticle = id; }

    bool ajouter();
    bool modifier();
    bool supprimer();

    static QList<ConferenceModel> afficherTous();
    static QList<ConferenceModel> rechercher(const QString &critere, const QString &valeur);
    static QList<ConferenceModel> trierPar(const QString &colonne, bool croissant = true);
    static ConferenceModel getById(int id);

    static QMap<QString, int> getStatsParTheme();
    static QMap<QString, int> getStatsParEtat();

    bool valider(QString &erreur) const;

private:
    int m_idConference;
    QString m_nom;
    QString m_lieu;
    QDate m_dateDebut;
    QDate m_dateFin;
    double m_fraisInscri;
    QString m_etat;
    QString m_theme;
    int m_idArticle;
};

#endif // CONFERENCEMODEL_H
