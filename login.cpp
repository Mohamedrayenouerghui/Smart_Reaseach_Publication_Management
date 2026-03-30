#include "login.h"
#include "connection.h"
#include "ui_login.h"

#include <QMessageBox>
#include <QSqlQuery>      // ← Fixed: was causing "incomplete type"
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QToolButton>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // === CRITICAL : Garantie de connexion (singleton) ===
    Connection &conn = Connection::createInstance();
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur Connexion",
                              "Impossible de se connecter à Oracle :\n" +
                                  conn.getDb().lastError().text());
        // We continue anyway → admin/admin fallback will still work
    }

    setupConnections();
    setupIcons();
}

Login::~Login()
{
    delete ui;
}

// ====================== DATABASE VERIFICATION ======================
bool Login::verifyUserInDatabase(const QString &email, const QString &password)
{
    Connection &conn = Connection::createInstance();
    QSqlDatabase &db = conn.getDb();

    if (!db.isOpen()) {
        qDebug() << "Database is not open for verification";
        return false;
    }

    QSqlQuery query(db);

    // IMPORTANT: Change table/column names if they are different in your Oracle DB
    query.prepare("SELECT COUNT(*) FROM USERS WHERE EMAIL = :email AND MDP = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    qDebug() << "Login query error:" << query.lastError().text();
    return false;
}

void Login::setupConnections()
{
    connect(ui->loginBtn, &QPushButton::clicked, this, &Login::on_loginBtn_clicked);

    // FaceID button
    connect(ui->facialAuthBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Authentification Faciale", "Reconnaissance faciale à implémenter");
    });

    // Forgotten password button
    connect(ui->forgotPasswordBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Mot de passe oublié", "Fonctionnalité à implémenter");
    });
}

void Login::on_loginBtn_clicked()
{
    QString email = ui->emailEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Champs requis",
                             "Veuillez entrer votre email et votre mot de passe.");
        return;
    }

    bool loginSuccess = false;

    // 1. Try real database verification
    if (Connection::createInstance().getDb().isOpen()) {
        if (verifyUserInDatabase(email, password)) {
            loginSuccess = true;
        }
    }

    // 2. Fallback: admin / admin (development mode)
    if (!loginSuccess) {
        if (email == "admin" && password == "admin") {
            loginSuccess = true;
        }
    }

    if (loginSuccess) {
        emit loginSuccessful();
        this->close();
    }
    else {
        QMessageBox::warning(this, "Échec de connexion",
                             "Email ou mot de passe incorrect.\n\n"
                             "• Mode test : admin / admin\n"
                             "• Ou vérifiez vos identifiants dans Oracle");

        ui->passwordEdit->clear();
        ui->passwordEdit->setFocus();
    }
}

void Login::setupIcons()
{
    QPixmap logoPixmap(":/desktop logo.png");
    if (!logoPixmap.isNull()) {
        QPixmap scaledLogo = logoPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->logoPlaceholder->setPixmap(scaledLogo);
    }
}
