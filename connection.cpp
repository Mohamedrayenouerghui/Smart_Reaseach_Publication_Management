#include "connection.h"
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createConnection()
{
    bool test = false;
    
    // Créer la connexion ODBC
    db = QSqlDatabase::addDatabase("QODBC");
    
    // Configuration de la base de données
    db.setDatabaseName("SmartResearch_DB"); // Nom de la source de données ODBC
    db.setUserName("admin");                 // Nom d'utilisateur
    db.setPassword("admin123");              // Mot de passe
    
    // Tentative d'ouverture de la connexion
    if (db.open()) {
        test = true;
        qDebug() << "Database connection successful!";
    } else {
        qDebug() << "Database connection failed!";
        qDebug() << "Error:" << db.lastError().text();
    }
    
    return test;
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}
