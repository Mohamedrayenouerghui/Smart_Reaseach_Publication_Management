// main.cpp - Clean & Professional Version

#include "mainwindow.h"
#include "login.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ====================== DATABASE CONNECTION (Singleton) ======================
    Connection &conn = Connection::createInstance();

    bool dbConnected = conn.createConnection();

    if (dbConnected) {
        qDebug() << "✅ Database connection successful";
    } else {
        qDebug() << "⚠️ Database connection failed - using fallback mode";

        QMessageBox::warning(nullptr, "Connexion Base de Données",
                             "Impossible de se connecter à Oracle.\n\n"
                             "L'application continuera en mode test (admin/admin).\n"
                             "Vérifiez vos paramètres ODBC et le service Oracle XE.",
                             QMessageBox::Ok);
    }

    // ====================== CREATE WINDOWS ======================
    Login loginWindow;
    MainWindow mainWindow;

    // ====================== SIGNAL/SLOT CONNECTION ======================
    // When login is successful → show MainWindow and close Login
    QObject::connect(&loginWindow, &Login::loginSuccessful,
                     [&mainWindow, &loginWindow]() {
                         loginWindow.close();
                         mainWindow.show();
                     });

    // Optional: Close the entire application if Login is closed without success
    QObject::connect(&loginWindow, &Login::destroyed, [&]() {
        if (!mainWindow.isVisible()) {
            qDebug() << "Login closed without successful authentication";
        }
    });

    // ====================== START APPLICATION ======================
    loginWindow.show();   // Show Login first (MainWindow stays hidden)

    return a.exec();
}
