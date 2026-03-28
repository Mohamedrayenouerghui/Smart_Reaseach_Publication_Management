/*#include "connection.h"
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
}*/
// connection.cpp
#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection* Connection::instance = nullptr;

Connection::Connection(QObject *parent) : QObject(parent)
{
}

Connection& Connection::createInstance()
{
    if (!instance) {
        instance = new Connection();
    }
    return *instance;
}

bool Connection::createConnection()
{
    // Remove any previous connection with same name to avoid conflicts
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");

    QString connectionString = "DRIVER={Oracle in XE};"
                               "DBQ=localhost:1521/XE;"
                               "UID=rayen;"
                               "PWD=rayen123;";

    db.setDatabaseName(connectionString);

    if (db.open()) {
        qDebug() << "✅ Oracle Database connection successful!";
        return true;
    } else {
        qDebug() << "❌ Database connection failed!";
        qDebug() << "Error:" << db.lastError().text();
        return false;
    }
}

QSqlDatabase& Connection::getDb()
{
    return db;
}
