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
    
    // Configuration avec connection string complète
    QString connectionString = "DRIVER={Oracle in XE};"
                               "SERVER=localhost;"
                               "PORT=1521;"
                               "SID=XE;"
                               "UID=rayen;"
                               "PWD=rayen123;";
    
    db.setDatabaseName(connectionString);
    
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
