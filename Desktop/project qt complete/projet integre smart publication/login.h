#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login; // Note: In your XML the class is LoginWindow
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    
    // Getters pour l'utilisateur connecté
    static QString getCurrentUser() { return currentUser; }
    static QString getCurrentRole() { return currentRole; }
    static void clearCurrentUser() { currentUser.clear(); currentRole.clear(); }

signals:
    void loginSuccessful(const QString &username, const QString &role);

private slots:
    void on_loginBtn_clicked();
    void onFaceAuthClicked();  // ← NOUVEAU : Authentification faciale

private:
    Ui::Login *ui;
    void setupConnections();
    void setupIcons();
    bool verifyUserInDatabase(const QString &email, const QString &password);
    void showNotificationsForReviewer(const QString &reviewerName);
    
    // Variables statiques pour stocker l'utilisateur connecté
    static QString currentUser;
    static QString currentRole;
};

#endif // LOGIN_H
