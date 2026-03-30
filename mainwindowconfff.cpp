#include "mainwindowconfff.h"
#include "ui_mainwindowconfff.h"

MainWindowconfff::MainWindowconfff(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowconfff)
{
    ui->setupUi(this);
}

MainWindowconfff::~MainWindowconfff()
{
    delete ui;
}
