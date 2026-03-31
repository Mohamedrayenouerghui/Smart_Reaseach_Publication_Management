#include "login.h"
#include "ui_login.h" // Depending on your build, this might be ui_loginwindow.h
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include <QPixmap>


Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setupConnections();
    setupIcons();

}

Login::~Login()
{
    delete ui;
}

void Login::setupConnections()
{
    // Connect the button manually if not using "Go to slot" feature
    connect(ui->loginBtn, &QPushButton::clicked, this, &Login::on_loginBtn_clicked);
    //Connect FaceID
    connect(ui->facialAuthBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Facial Authenticator", "Reconaissance facial à implémenter");
    });
    //Connect Forgotten password
    connect(ui->forgotPasswordBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Forgotten password", "Mot de passe oublié à implémenter");
    });
    //Connect Sign Up
    connect(ui->signupBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Sign Up", "Creation d'un nouveau compte à implémenter");
    });
}
void Login::on_loginBtn_clicked()
{
    // 1. (Optional) Add your username/password verification logic here
    // if (ui->emailEdit->text() == "admin" && ...) { ... }

    // 2. Emit the signal that login was successful
    emit loginSuccessful();

    // 3. Close the login window
    this->close();
}

void Login::setupIcons()
{

    // Set header logo
    QPixmap logoPixmap(":/desktop logo.png");
    if (!logoPixmap.isNull()) {
        QPixmap scaledLogo = logoPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->logoPlaceholder->setPixmap(scaledLogo);
    }

}
