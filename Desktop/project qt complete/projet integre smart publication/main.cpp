#include "mainwindow.h"
#include "login.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Test database connection
    Connection c;
    bool test = c.createconnect();
    
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                    QObject::tr("Connection successful.\n"
                                "Click OK to continue."), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                    QObject::tr("Connection failed.\n"
                                "Click OK to exit."), QMessageBox::Ok);
        return 1; // Exit if connection fails
    }
    
    MainWindow w;
    Login l;
    
    // Connect the Login signal to the MainWindow show slot
    QObject::connect(&l, &Login::loginSuccessful, [&](const QString &username, const QString &role){
        qDebug() << "User logged in:" << username << "Role:" << role;
        
        // Configurer les permissions selon le rôle
        if (role == "relecteur") {
            w.setRelecteurMode(true);
        } else {
            w.setRelecteurMode(false);
        }
        
        w.show();
    });

    // Show only the Login window initially
    l.show();

    return a.exec();
}
