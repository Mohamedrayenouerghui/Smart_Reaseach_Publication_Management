#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
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
    void setupPrototypeUser();
    bool ajouter();

    // Helper methods
    void getFormData();
    void clearForm();
    void loadUserToTable();
    void showAddUserForm();
    void hideAddUserForm();
    void uploadPhoto();



private slots:
    void onModifyUser();
    void onDeleteUser();
    void updateUser();
    void loadUserDataForEdit(int);
    void resetToAddMode();

};

#endif // USERS_H
