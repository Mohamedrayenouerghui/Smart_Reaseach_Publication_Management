#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== Test de Connexion Oracle ===";
    
    // Test 1: Format standard
    qDebug() << "\nTest 1: localhost:1521/Source_Projet2A";
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QOCISQL", "test1");
    db1.setHostName("localhost");
    db1.setUserName("marwa");
    db1.setPassword("1234");
    db1.setDatabaseName("Source_Projet2A");
    db1.setPort(1521);
    
    if (db1.open()) {
        qDebug() << "✓ Succès!";
        db1.close();
    } else {
        qDebug() << "✗ Erreur:" << db1.lastError().text();
        qDebug() << "  Driver:" << db1.lastError().driverText();
        qDebug() << "  Database:" << db1.lastError().databaseText();
    }
    
    // Test 2: Sans hostname
    qDebug() << "\nTest 2: Sans hostname (utiliser SID direct)";
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QOCISQL", "test2");
    db2.setUserName("marwa");
    db2.setPassword("1234");
    db2.setDatabaseName("Source_Projet2A");
    
    if (db2.open()) {
        qDebug() << "✓ Succès!";
        db2.close();
    } else {
        qDebug() << "✗ Erreur:" << db2.lastError().text();
        qDebug() << "  Driver:" << db2.lastError().driverText();
        qDebug() << "  Database:" << db2.lastError().databaseText();
    }
    
    // Test 3: Vérifier les drivers disponibles
    qDebug() << "\n=== Drivers SQL disponibles ===";
    QStringList drivers = QSqlDatabase::drivers();
    for (const QString &driver : drivers) {
        qDebug() << " -" << driver;
    }
    
    qDebug() << "\n✓ Test terminé";
    return 0;
}
