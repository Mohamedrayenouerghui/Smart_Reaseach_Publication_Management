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
                               "UID=changeme;"
                               "PWD=changeme;";

    db.setDatabaseName(connectionString);

    if (db.open()) {
        qDebug() << "✅ Oracle Database connection successful!";
        qDebug() << "   Driver  :" << db.driverName();
        qDebug() << "   Database:" << db.databaseName();
        qDebug() << "   IsValid :" << db.isValid();
        qDebug() << "   IsOpen  :" << db.isOpen();
        return true;
    } else {
        qDebug() << "❌ Database connection failed!";
        qDebug() << "   Driver error :" << db.lastError().driverText();
        qDebug() << "   Database error:" << db.lastError().databaseText();
        qDebug() << "   Full error   :" << db.lastError().text();
        return false;
    }
}

QSqlDatabase& Connection::getDb()
{
    return db;
}
