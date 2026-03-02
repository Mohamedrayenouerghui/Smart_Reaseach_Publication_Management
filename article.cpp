#include "article.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructeur par défaut
Article::Article()
{
    id_article = 0;
    titre = "";
    contenu = "";
    domaine = "";
    etat = "";
    date_creation = QDate::currentDate();
    id_user = 0;
    id_soumission = 0;
}

// Constructeur avec paramètres
Article::Article(int id, QString titre, QString contenu, QString domaine, QString etat, QDate dateCreation, int idUser, int idSoumission)
{
    this->id_article = id;
    this->titre = titre;
    this->contenu = contenu;
    this->domaine = domaine;
    this->etat = etat;
    this->date_creation = dateCreation;
    this->id_user = idUser;
    this->id_soumission = idSoumission;
}

// CREATE - Ajouter un article
bool Article::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO ARTICLE (ID_article, titre, contenu, domaine, Etat, Date_creation, ID_user, ID_soumission) "
                  "VALUES (SEQ_ARTICLE.NEXTVAL, :titre, :contenu, :domaine, :etat, :date, :idUser, :idSoum)");
    
    query.bindValue(":titre", titre);
    query.bindValue(":contenu", contenu);
    query.bindValue(":domaine", domaine);
    query.bindValue(":etat", etat);
    query.bindValue(":date", date_creation);
    query.bindValue(":idUser", id_user == 0 ? QVariant(QMetaType::fromType<int>()) : id_user);
    query.bindValue(":idSoum", id_soumission == 0 ? QVariant(QMetaType::fromType<int>()) : id_soumission);
    
    if (!query.exec()) {
        qDebug() << "Erreur ajout article:" << query.lastError().text();
        return false;
    }
    
    return true;
}

// READ - Afficher tous les articles
QSqlQuery Article::afficher()
{
    QSqlQuery query;
    query.prepare("SELECT ID_article, titre, domaine, Etat, Date_creation FROM ARTICLE ORDER BY Date_creation DESC");
    query.exec();
    return query;
}

// UPDATE - Modifier un article
bool Article::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE ARTICLE SET titre = :titre, contenu = :contenu, domaine = :domaine, "
                  "Etat = :etat, Date_creation = :date, ID_user = :idUser, ID_soumission = :idSoum "
                  "WHERE ID_article = :id");
    
    query.bindValue(":id", id_article);
    query.bindValue(":titre", titre);
    query.bindValue(":contenu", contenu);
    query.bindValue(":domaine", domaine);
    query.bindValue(":etat", etat);
    query.bindValue(":date", date_creation);
    query.bindValue(":idUser", id_user == 0 ? QVariant(QMetaType::fromType<int>()) : id_user);
    query.bindValue(":idSoum", id_soumission == 0 ? QVariant(QMetaType::fromType<int>()) : id_soumission);
    
    if (!query.exec()) {
        qDebug() << "Erreur modification article:" << query.lastError().text();
        return false;
    }
    
    return true;
}

// DELETE - Supprimer un article
bool Article::supprimer()
{
    QSqlQuery query;
    query.prepare("DELETE FROM ARTICLE WHERE ID_article = :id");
    query.bindValue(":id", id_article);
    
    if (!query.exec()) {
        qDebug() << "Erreur suppression article:" << query.lastError().text();
        return false;
    }
    
    return true;
}

// Rechercher par titre
QSqlQuery Article::rechercherParTitre(QString titre)
{
    QSqlQuery query;
    query.prepare("SELECT ID_article, titre, domaine, Etat, Date_creation FROM ARTICLE "
                  "WHERE LOWER(titre) LIKE LOWER(:titre) ORDER BY Date_creation DESC");
    query.bindValue(":titre", "%" + titre + "%");
    query.exec();
    return query;
}

// Rechercher par domaine
QSqlQuery Article::rechercherParDomaine(QString domaine)
{
    QSqlQuery query;
    query.prepare("SELECT ID_article, titre, domaine, Etat, Date_creation FROM ARTICLE "
                  "WHERE LOWER(domaine) LIKE LOWER(:domaine) ORDER BY Date_creation DESC");
    query.bindValue(":domaine", "%" + domaine + "%");
    query.exec();
    return query;
}
