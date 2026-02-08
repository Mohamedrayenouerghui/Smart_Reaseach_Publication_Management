#ifndef MAINWINDOWCONFFF_H
#define MAINWINDOWCONFFF_H

#include <QMainWindow>

namespace Ui {
class MainWindowconfff;
}

class MainWindowconfff : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowconfff(QWidget *parent = nullptr);
    ~MainWindowconfff();

private:
    Ui::MainWindowconfff *ui;
};

#endif // MAINWINDOWCONFFF_H
