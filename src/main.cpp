#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("Système de Gestion des Évaluations");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("University");
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
