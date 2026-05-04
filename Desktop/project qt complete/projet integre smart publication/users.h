#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QSqlQuery>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>

namespace Ui {
class Users;
}

class Users : public QWidget
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = nullptr);
    ~Users();

private:
    Ui::Users *ui;

    // Member variables for user data
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString password;
    QString role;
    QString photoPath;
    int currentEditingId = -1;   // -1 means we're in "Add" mode
    bool isEditMode = false;

    // Private methods
    void setupTable();
    void setupMenus();
    void setupConnections();

    bool ajouter();

    // Helper methods
    void getFormData();
    void clearForm();
    void uploadPhoto();

    void showAddUserForm();
    void hideAddUserForm();

    // === Surcharges pour loadUserToTable ===
    void loadUserToTable();                                           // Sans paramètre
    void loadUserToTable(const QString &searchText,
                         const QString &searchType);                  // Avec recherche
    void loadUserToTable(const QString &searchText,
                         const QString &searchType,
                         const QString &sortColumn);                  // Avec recherche + tri

private slots:
    void onModifyUser();
    void onDeleteUser();
    void updateUser();
    void loadUserDataForEdit(int userId);
    void resetToAddMode();
    void searchUsers();
    void sortUsers(QAction *action);   // ← MODIFIÉ : prend QAction*
    void exportUsers();
    void statsUsers();
    void on_btnTogglePassword_clicked();
    void onCaptureFaceClicked();  // ← NOUVEAU : Capture faciale (Arduino 3)

};

#endif // USERS_H
