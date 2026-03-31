#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    
    // Méthode 1: ODBC (actuelle)
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smart_publication_db");
    db.setUserName("SYSTEM");
    db.setPassword("mehdi22");
    
    /* Méthode 2: Oracle Direct (alternative si ODBC ne marche pas)
    // Décommente ces lignes et commente les lignes ODBC ci-dessus
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("localhost");
    db.setPort(1521);
    db.setDatabaseName("xe");
    db.setUserName("SYSTEM");
    db.setPassword("mehdi22");
    */

    if (db.open()) {
        test = true;
    } else {
        // Afficher l'erreur pour debug
        qDebug() << "Database error:" << db.lastError().text();
    }

    return test;
}
