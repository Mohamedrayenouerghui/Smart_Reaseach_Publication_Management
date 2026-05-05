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
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");

    QString connectionString = QString("DRIVER={Oracle in XE};"
                                       "DBQ=%1;"
                                       "UID=%2;"
                                       "PWD=%3;")
                                   .arg(m_host, m_uid, m_pwd);

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

void Connection::setCredentials(const QString &host, const QString &uid, const QString &pwd)
{
    m_host = host;
    m_uid  = uid;
    m_pwd  = pwd;
}

QSqlDatabase& Connection::getDb()
{
    return db;
}

QSqlDatabase Connection::getDatabase()
{
    return Connection::createInstance().getDb();
}
