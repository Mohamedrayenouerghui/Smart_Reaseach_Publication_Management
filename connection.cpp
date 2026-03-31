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
                               "UID=ala;"
                               "PWD=Sarex2022;";

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
