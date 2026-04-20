#include "connection.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

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

    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // Try Oracle connection - use your existing database setup
    bool oracleConnected = tryOracleConnection();
    if (!oracleConnected) {
        qDebug() << "Oracle connection failed - but application will continue";
        qDebug() << "You may need to check your Oracle client configuration";
    }
    
    return oracleConnected;
}

bool Connection::tryOracleConnection()
{
    // Try direct Oracle driver first (more reliable)
    if (QSqlDatabase::drivers().contains("QOCI")) {
        qDebug() << "Trying direct Oracle driver (QOCI) with XE";
        db = QSqlDatabase::addDatabase("QOCI", "qt_sql_default_connection");
        db.setDatabaseName("XE");
        db.setUserName("ala");
        db.setPassword("Sarex2022");
        db.setHostName("localhost");
        db.setPort(1521);
    }
    // Fallback to ODBC
    else if (QSqlDatabase::drivers().contains("QODBC")) {
        qDebug() << "Trying ODBC driver with projet2A DSN";
        db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");
        db.setDatabaseName("projet2A");
        db.setUserName("ala");
        db.setPassword("Sarex2022");
    } else {
        qDebug() << "No Oracle drivers available";
        return false;
    }

    if (db.open()) {
        qDebug() << "Oracle Database connection successful!";
        qDebug() << "Driver:" << db.driverName();
        qDebug() << "Database:" << db.databaseName();
        return true;
    } else {
        qDebug() << "Oracle connection failed!";
        qDebug() << "Error:" << db.lastError().text();
        qDebug() << "Driver Error:" << db.lastError().driverText();
        qDebug() << "Database Error:" << db.lastError().databaseText();
        return false;
    }
}

bool Connection::createSQLiteConnection()
{
    // Remove failed Oracle connection
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName("smart_research_test.db");

    if (db.open()) {
        qDebug() << "SQLite fallback connection successful!";
        qDebug() << "Database file:" << db.databaseName();
        
        // Create basic tables for testing
        createSQLiteTables();
        return true;
    } else {
        qDebug() << "SQLite connection failed:" << db.lastError().text();
        return false;
    }
}

bool Connection::createSQLiteTables()
{
    QSqlQuery query(db);
    
    // Create USERS table
    query.exec("CREATE TABLE IF NOT EXISTS USERS ("
               "ID_user INTEGER PRIMARY KEY AUTOINCREMENT, "
               "nom TEXT NOT NULL, "
               "prenom TEXT NOT NULL, "
               "email TEXT UNIQUE NOT NULL, "
               "mdp TEXT NOT NULL, "
               "Role TEXT, "
               "photo TEXT)");
    
    // Create LABORATOIRE table  
    query.exec("CREATE TABLE IF NOT EXISTS LABORATOIRE ("
               "ID_labo INTEGER PRIMARY KEY AUTOINCREMENT, "
               "nom TEXT NOT NULL, "
               "budget REAL, "
               "Date_creation DATE NOT NULL, "
               "domaine TEXT NOT NULL, "
               "localisation TEXT NOT NULL, "
               "nombre_chercheurs INTEGER DEFAULT 0, "
               "classement TEXT, "
               "historique TEXT, "
               "ID_user INTEGER, "
               "FOREIGN KEY (ID_user) REFERENCES USERS(ID_user))");
    
    // Insert test user
    query.exec("INSERT OR IGNORE INTO USERS (ID_user, nom, prenom, email, mdp, Role) "
               "VALUES (1, 'Admin', 'User', 'admin@test.com', 'admin', 'admin')");
    
    qDebug() << "SQLite tables created successfully";
    return true;
}

QSqlDatabase& Connection::getDb()
{
    return db;
}
