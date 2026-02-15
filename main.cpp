#include "mainwindow.h"
#include "login.h"
#include "connection.h" // Nouvel include

#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Login l;

    // Récupération de l'instance unique et tentative de connexion
    Connection& c = Connection::createInstance();
    bool test = c.createConnection();

    if (test) {
        l.show(); // On montre le login si la DB est prête
        QMessageBox::information(nullptr, "Base de données", "Connexion réussie.");
    } else {
        QMessageBox::critical(nullptr, "Base de données", "Échec de la connexion.");
        return -1; // On ferme l'appli si la DB n'est pas accessible
    }
    // 2. Connect the Login signal to the MainWindow show slot
    // When 'l' says loginSuccessful, 'w' will show itself.
    QObject::connect(&l, &Login::loginSuccessful, [&](){
        w.show();
    });

    // 3. Show only the Login window initially
    l.show();

    return a.exec();
}
