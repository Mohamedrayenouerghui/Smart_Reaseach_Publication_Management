#include "submission.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Submission window;
    window.setWindowTitle("Gestion des Soumissions");
    window.resize(1200, 700);
    window.show();
    
    return app.exec();
}
