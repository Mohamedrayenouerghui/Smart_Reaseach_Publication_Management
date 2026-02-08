#include "mainwindow.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Login l;
    // 2. Connect the Login signal to the MainWindow show slot
    // When 'l' says loginSuccessful, 'w' will show itself.
    QObject::connect(&l, &Login::loginSuccessful, [&](){
        w.show();
    });

    // 3. Show only the Login window initially
    l.show();

    return a.exec();
}
