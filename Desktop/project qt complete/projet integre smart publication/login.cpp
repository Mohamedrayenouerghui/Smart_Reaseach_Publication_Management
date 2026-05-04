#include "login.h"
#include "connection.h"
#include "ui_login.h"
#include "FaceAuthDialog.h"  // ← NOUVEAU : Dialog de reconnaissance faciale

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

// Initialisation des variables statiques
QString Login::currentUser = "";
QString Login::currentRole = "";

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // Connexion Oracle
    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(this, "Erreur Connexion",
                              "Impossible de se connecter à Oracle");
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
    QSqlDatabase db = QSqlDatabase::database();

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

    // FaceID button - INTÉGRÉ avec OpenCV
    connect(ui->facialAuthBtn, &QPushButton::clicked, this, &Login::onFaceAuthClicked);

    // Forgotten password button
    connect(ui->forgotPasswordBtn, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Mot de passe oublié", "Fonctionnalité à implémenter");
    });
}

void Login::on_loginBtn_clicked()
{
    QString email    = ui->emailEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    // ── 1. Validation rapide des champs obligatoires ──
    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Champs requis",
                             "Veuillez entrer votre email et votre mot de passe.");
        return;
    }

    bool loginSuccess = false;
    QString username = "";
    QString role = "";

    // ── 2. Vérification des comptes relecteurs (hardcodés) ──
    QMap<QString, QString> relecteurs = {
        {"Mehdi Karaa", "relecteur"},
        {"Rayen Ouerghui", "relecteur"},
        {"Louay Fkiri", "relecteur"},
        {"Leo Messi", "relecteur"}
    };
    
    for (auto it = relecteurs.begin(); it != relecteurs.end(); ++it) {
        if (email.compare(it.key(), Qt::CaseInsensitive) == 0 && password == "relecteur") {
            loginSuccess = true;
            username = it.key();
            role = "relecteur";
            qDebug() << "✅ Login RELECTEUR:" << username;
            break;
        }
    }

    // ── 3. Mode développement : identifiant admin/admin ──
    if (!loginSuccess && email.compare("admin", Qt::CaseInsensitive) == 0 && password == "admin") {
        qDebug() << "✅ Login ADMIN";
        loginSuccess = true;
        username = "admin";
        role = "admin";
    }

    // ── 4. Vérification réelle en base de données ──
    if (!loginSuccess) {
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen()) {
            if (verifyUserInDatabase(email, password)) {
                loginSuccess = true;
                username = email;
                role = "user";
                qDebug() << "✅ Login réussi via base Oracle pour :" << email;
            }
        } else {
            qWarning() << "⚠️ Base de données non ouverte lors de la connexion";
        }
    }

    // ── 5. Résultat final ──
    if (loginSuccess) {
        currentUser = username;
        currentRole = role;
        
        // Afficher les notifications pour les relecteurs
        if (role == "relecteur") {
            showNotificationsForReviewer(username);
        }
        
        emit loginSuccessful(username, role);
        this->close();
    } else {
        QMessageBox::warning(this, "Échec de connexion",
                             "Email ou mot de passe incorrect.");

        ui->passwordEdit->clear();
        ui->passwordEdit->setFocus();
    }
}

void Login::showNotificationsForReviewer(const QString &reviewerName)
{
    QString notificationsPath = QDir::tempPath() + "/notifications.txt";
    QFile file(notificationsPath);
    
    qDebug() << "=== CHECKING NOTIFICATIONS ===";
    qDebug() << "Reviewer name:" << reviewerName;
    qDebug() << "Notifications path:" << notificationsPath;
    qDebug() << "File exists:" << file.exists();
    
    if (!file.exists()) {
        qDebug() << "Aucune notification trouvée - fichier n'existe pas";
        return;
    }
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier de notifications";
        return;
    }
    
    QTextStream in(&file);
    QStringList notifications;
    
    qDebug() << "=== READING FILE ===";
    // Lire toutes les notifications pour ce relecteur
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Line read:" << line;
        QStringList parts = line.split("|");
        
        if (parts.size() >= 4) {
            QString relecteur = parts[0];
            QString article = parts[1];
            QString dateLimite = parts[2];
            QString dateCreation = parts[3];
            
            qDebug() << "Parsed - Relecteur:" << relecteur << "Article:" << article;
            
            // Vérifier si la notification est pour ce relecteur
            if (relecteur.contains(reviewerName, Qt::CaseInsensitive)) {
                qDebug() << "MATCH! Adding notification for" << reviewerName;
                notifications << QString("%1|%2|%3").arg(article, dateLimite, dateCreation);
            } else {
                qDebug() << "No match:" << relecteur << "vs" << reviewerName;
            }
        }
    }
    
    file.close();
    
    qDebug() << "Total notifications found:" << notifications.size();
    
    // Afficher les notifications s'il y en a
    if (!notifications.isEmpty()) {
        // Stocker les notifications pour affichage différé
        QStringList* notifList = new QStringList(notifications);
        QString name = reviewerName;
        
        // Afficher la notification avec un délai pour que l'interface principale apparaisse d'abord
        QTimer::singleShot(800, [notifList, name]() {
            // Créer un QDialog personnalisé pour une vraie notification
            QDialog *notifDialog = new QDialog();
            notifDialog->setWindowTitle("Notifications");
            notifDialog->setModal(true);
            notifDialog->setFixedSize(480, 320 + (notifList->size() * 80));
            notifDialog->setStyleSheet("QDialog { background-color: #FFFFFF; border-radius: 16px; }");
            
            QVBoxLayout *mainLayout = new QVBoxLayout(notifDialog);
            mainLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->setSpacing(0);
            
            // En-tête avec icône et titre
            QWidget *headerWidget = new QWidget();
            headerWidget->setStyleSheet(
                "QWidget { "
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "        stop:0 #8B5CF6, stop:1 #7C3AED); "
                "    border-radius: 16px 16px 0 0; "
                "    padding: 20px; "
                "}"
            );
            QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
            
            QLabel *iconLabel = new QLabel("🔔");
            iconLabel->setStyleSheet("font-size: 48px; background: transparent;");
            iconLabel->setAlignment(Qt::AlignCenter);
            
            QLabel *titleLabel = new QLabel("Nouvelles Notifications");
            titleLabel->setStyleSheet(
                "font-size: 22px; "
                "font-weight: bold; "
                "color: white; "
                "background: transparent; "
                "padding: 5px;"
            );
            titleLabel->setAlignment(Qt::AlignCenter);
            
            QLabel *subtitleLabel = new QLabel(QString("Bonjour %1!").arg(name));
            subtitleLabel->setStyleSheet(
                "font-size: 14px; "
                "color: rgba(255, 255, 255, 0.9); "
                "background: transparent; "
                "padding: 2px;"
            );
            subtitleLabel->setAlignment(Qt::AlignCenter);
            
            headerLayout->addWidget(iconLabel);
            headerLayout->addWidget(titleLabel);
            headerLayout->addWidget(subtitleLabel);
            
            mainLayout->addWidget(headerWidget);
            
            // Corps avec les notifications
            QWidget *bodyWidget = new QWidget();
            bodyWidget->setStyleSheet("QWidget { background-color: #F8FAFC; padding: 20px; }");
            QVBoxLayout *bodyLayout = new QVBoxLayout(bodyWidget);
            bodyLayout->setSpacing(12);
            
            QLabel *countLabel = new QLabel(QString("Vous avez %1 soumission(s) en attente d'évaluation:").arg(notifList->size()));
            countLabel->setStyleSheet(
                "font-size: 13px; "
                "color: #64748B; "
                "font-weight: 600; "
                "padding: 5px 0;"
            );
            bodyLayout->addWidget(countLabel);
            
            // Ajouter chaque notification dans une carte
            for (const QString &notif : *notifList) {
                QStringList parts = notif.split("|");
                if (parts.size() >= 3) {
                    QString article = parts[0];
                    QString dateLimite = parts[1];
                    QString dateCreation = parts[2];
                    
                    QWidget *notifCard = new QWidget();
                    notifCard->setStyleSheet(
                        "QWidget { "
                        "    background-color: white; "
                        "    border: 1px solid #E2E8F0; "
                        "    border-left: 4px solid #8B5CF6; "
                        "    border-radius: 8px; "
                        "    padding: 12px; "
                        "}"
                    );
                    
                    QVBoxLayout *cardLayout = new QVBoxLayout(notifCard);
                    cardLayout->setSpacing(6);
                    cardLayout->setContentsMargins(12, 10, 12, 10);
                    
                    QLabel *articleLabel = new QLabel("📄 " + article);
                    articleLabel->setStyleSheet(
                        "font-size: 14px; "
                        "font-weight: 600; "
                        "color: #1E293B; "
                        "background: transparent; "
                        "border: none;"
                    );
                    articleLabel->setWordWrap(true);
                    
                    QLabel *dateLabel = new QLabel(QString("📅 Date limite: %1  •  🗓️ Créé le: %2").arg(dateLimite, dateCreation));
                    dateLabel->setStyleSheet(
                        "font-size: 12px; "
                        "color: #64748B; "
                        "background: transparent; "
                        "border: none;"
                    );
                    
                    cardLayout->addWidget(articleLabel);
                    cardLayout->addWidget(dateLabel);
                    
                    bodyLayout->addWidget(notifCard);
                }
            }
            
            bodyLayout->addStretch();
            mainLayout->addWidget(bodyWidget);
            
            // Pied avec bouton
            QWidget *footerWidget = new QWidget();
            footerWidget->setStyleSheet("QWidget { background-color: white; padding: 15px; border-radius: 0 0 16px 16px; }");
            QHBoxLayout *footerLayout = new QHBoxLayout(footerWidget);
            
            QPushButton *okBtn = new QPushButton("Compris, merci!");
            okBtn->setStyleSheet(
                "QPushButton { "
                "    background-color: #8B5CF6; "
                "    color: white; "
                "    border: none; "
                "    border-radius: 8px; "
                "    padding: 12px 32px; "
                "    font-size: 14px; "
                "    font-weight: 600; "
                "    min-width: 150px; "
                "} "
                "QPushButton:hover { "
                "    background-color: #7C3AED; "
                "} "
                "QPushButton:pressed { "
                "    background-color: #6D28D9; "
                "}"
            );
            
            connect(okBtn, &QPushButton::clicked, notifDialog, &QDialog::accept);
            
            footerLayout->addStretch();
            footerLayout->addWidget(okBtn);
            footerLayout->addStretch();
            
            mainLayout->addWidget(footerWidget);
            
            notifDialog->exec();
            notifDialog->deleteLater();
            delete notifList;
            
            // Supprimer les notifications du fichier après affichage
            QString notificationsPath = QDir::tempPath() + "/notifications.txt";
            QFile::remove(notificationsPath);
            qDebug() << "Notifications affichées et fichier supprimé";
        });
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

// ═══════════════════════════════════════════════════════════════════════════
//  NOUVEAU : Authentification par reconnaissance faciale (Arduino 3)
// ═══════════════════════════════════════════════════════════════════════════
void Login::onFaceAuthClicked()
{
    qDebug() << "[Login] Démarrage de l'authentification faciale...";
    
    FaceAuthDialog faceDialog(this);
    
    if (faceDialog.exec() == QDialog::Accepted) {
        QString matchedEmail = faceDialog.matchedEmail();
        
        if (matchedEmail.isEmpty()) {
            QMessageBox::warning(this, "Échec", 
                "Aucun visage reconnu. Veuillez réessayer ou utiliser l'authentification classique.");
            return;
        }
        
        qDebug() << "[Login] Visage reconnu pour:" << matchedEmail;
        
        // Vérifier que l'utilisateur existe dans la base de données
        QSqlQuery query;
        query.prepare("SELECT NOM, PRENOM, ROLE FROM USERS WHERE EMAIL = :email");
        query.bindValue(":email", matchedEmail);
        
        if (query.exec() && query.next()) {
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();
            QString role = query.value(2).toString();
            QString fullName = prenom + " " + nom;
            
            currentUser = fullName;
            currentRole = role;
            
            qDebug() << "✅ Authentification faciale réussie:" << fullName << "(" << role << ")";
            
            QMessageBox::information(this, "Connexion Réussie", 
                QString("Bienvenue %1 !\n\nRôle : %2").arg(fullName, role));
            
            emit loginSuccessful(fullName, role);
            this->close();
        } else {
            QMessageBox::critical(this, "Erreur", 
                "Visage reconnu mais utilisateur introuvable dans la base de données.");
            qDebug() << "[Login] Erreur SQL:" << query.lastError().text();
        }
    } else {
        qDebug() << "[Login] Authentification faciale annulée";
    }
}
