#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection {
public:
    static Connection& createInstance(); // Retourne l'instance unique
    bool createConnection();// Configure et ouvre la DB
    QSqlDatabase& getDb();          // ← Add this declaration


private:
    Connection();                        // Constructeur privé
    ~Connection();
    QSqlDatabase db;
};

#endif
