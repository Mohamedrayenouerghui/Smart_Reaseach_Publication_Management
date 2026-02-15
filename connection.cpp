#include "connection.h"

// Ajoutez ceci à la fin de connection.cpp
Connection::~Connection() {
    if (db.isOpen()) {
        db.close();
    }
}

Connection::Connection() {
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection& Connection::createInstance() {
    static Connection instance;
    return instance;
}

bool Connection::createConnection() {
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("ala");
    db.setPassword("Sarex2022");

    if (db.open()) {
        qDebug() << "Connexion établie";
        return true;
    }
    qDebug() << "Échec de la connexion :" << db.lastError().text();
    return false;
}
