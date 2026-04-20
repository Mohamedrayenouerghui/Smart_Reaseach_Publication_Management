#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>

class Article
{
public:
    // Constructeurs
    Article();
    Article(int id, QString titre, QString contenu, QString domaine, QString etat, QDate dateCreation, int idUser, int idSoumission);
    
    // Getters
    int getIdArticle() const { return id_article; }
    QString getTitre() const { return titre; }
    QString getContenu() const { return contenu; }
    QString getDomaine() const { return domaine; }
    QString getEtat() const { return etat; }
    QDate getDateCreation() const { return date_creation; }
    int getIdUser() const { return id_user; }
    int getIdSoumission() const { return id_soumission; }
    
    // Setters
    void setIdArticle(int id) { id_article = id; }
    void setTitre(QString t) { titre = t; }
    void setContenu(QString c) { contenu = c; }
    void setDomaine(QString d) { domaine = d; }
    void setEtat(QString e) { etat = e; }
    void setDateCreation(QDate d) { date_creation = d; }
    void setIdUser(int id) { id_user = id; }
    void setIdSoumission(int id) { id_soumission = id; }
    
    // CRUD Operations
    bool ajouter();                    // CREATE
    static QSqlQuery afficher();       // READ
    bool modifier();                   // UPDATE
    bool supprimer();                  // DELETE
    
    // Recherche
    static QSqlQuery rechercherParTitre(QString titre);
    static QSqlQuery rechercherParDomaine(QString domaine);

private:
    int id_article;
    QString titre;
    QString contenu;
    QString domaine;
    QString etat;
    QDate date_creation;
    int id_user;
    int id_soumission;
};

#endif // ARTICLE_H
