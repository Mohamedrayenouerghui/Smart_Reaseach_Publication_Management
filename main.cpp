#include "mainwindow.h"
#include "login.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Test de connexion à la base de données
    Connection c;
    bool dbConnected = c.createConnection();
    
    if (dbConnected) {
        QMessageBox::information(nullptr, QObject::tr("Connexion Réussie"),
                    QObject::tr("Connexion à la base de données réussie!\n"
                                "Cliquez OK pour continuer."), QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de Connexion"),
                    QObject::tr("Échec de connexion à la base de données.\n"
                                "Vérifiez vos paramètres ODBC.\n"
                                "Cliquez OK pour continuer quand même."), QMessageBox::Ok);
    }
    
    MainWindow w;
    Login l;
    
    // Connect the Login signal to the MainWindow show slot
    QObject::connect(&l, &Login::loginSuccessful, [&](){
        w.show();
    });

    // Show only the Login window initially
    l.show();

    return a.exec();
}
