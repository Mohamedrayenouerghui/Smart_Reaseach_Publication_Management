#include "mainwindow.h"
#include "login.h"
#include "connection.h"

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

// ── Dialog de connexion Oracle ────────────────────────────────────
static bool showOracleConnectionDialog()
{
    QDialog dlg;
    dlg.setWindowTitle("Connexion Oracle");
    dlg.setFixedSize(400, 260);
    dlg.setStyleSheet(
        "QDialog { background-color: #1E293B; }"
        "QLabel  { color: #E2E8F0; font-size: 13px; }"
        "QLineEdit { background:#0F172A; color:#E2E8F0; border:1px solid #334155;"
        "            border-radius:6px; padding:8px; font-size:13px; }"
        "QPushButton { background-color:#3B82F6; color:white; border:none;"
        "              border-radius:6px; padding:10px 24px; font-size:13px; font-weight:bold; }"
        "QPushButton:hover { background-color:#2563EB; }"
    );

    auto *layout = new QVBoxLayout(&dlg);
    layout->setSpacing(12);
    layout->setContentsMargins(24, 24, 24, 24);

    auto *title = new QLabel("<b style='font-size:16px; color:#38BDF8;'>Connexion Base de Données Oracle</b>");
    title->setAlignment(Qt::AlignCenter);

    auto *form = new QFormLayout();
    auto *hostEdit = new QLineEdit("localhost:1521/XE");
    auto *uidEdit  = new QLineEdit();
    uidEdit->setPlaceholderText("Votre nom d'utilisateur Oracle");
    auto *pwdEdit  = new QLineEdit();
    pwdEdit->setEchoMode(QLineEdit::Password);
    pwdEdit->setPlaceholderText("Votre mot de passe Oracle");

    form->addRow("Serveur :", hostEdit);
    form->addRow("Utilisateur :", uidEdit);
    form->addRow("Mot de passe :", pwdEdit);

    auto *btnLayout = new QHBoxLayout();
    auto *connectBtn = new QPushButton("Se connecter");
    auto *skipBtn    = new QPushButton("Ignorer");
    skipBtn->setStyleSheet(
        "QPushButton { background-color:#475569; color:white; border:none;"
        "              border-radius:6px; padding:10px 24px; font-size:13px; }"
        "QPushButton:hover { background-color:#334155; }");
    btnLayout->addWidget(connectBtn);
    btnLayout->addWidget(skipBtn);

    layout->addWidget(title);
    layout->addLayout(form);
    layout->addLayout(btnLayout);

    bool connected = false;

    QObject::connect(connectBtn, &QPushButton::clicked, [&]() {
        QString host = hostEdit->text().trimmed();
        QString uid  = uidEdit->text().trimmed();
        QString pwd  = pwdEdit->text();

        if (uid.isEmpty()) {
            QMessageBox::warning(&dlg, "Erreur", "Veuillez entrer votre nom d'utilisateur.");
            return;
        }

        // Update connection string dynamically
        Connection &conn = Connection::createInstance();
        conn.setCredentials(host, uid, pwd);

        if (conn.createConnection()) {
            connected = true;
            dlg.accept();
        } else {
            QMessageBox::critical(&dlg, "Erreur de connexion",
                "Impossible de se connecter à Oracle.\n"
                "Vérifiez vos identifiants et que le serveur est démarré.");
        }
    });

    QObject::connect(skipBtn, &QPushButton::clicked, [&]() {
        connected = false;
        dlg.accept();
    });

    dlg.exec();
    return connected;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Show Oracle connection dialog first
    showOracleConnectionDialog();

    MainWindow w;
    Login l;
    QObject::connect(&l, &Login::loginSuccessful, [&](){
        w.show();
    });
    l.show();

    return a.exec();
}
