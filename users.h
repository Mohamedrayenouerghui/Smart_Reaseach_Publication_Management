#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QSqlQuery>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QTimer>

#include "arduino.h"   // ← RFID / Arduino integration

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

    // ── User form data ───────────────────────────────────────────────────────
    int     id;
    QString nom;
    QString prenom;
    QString email;
    QString password;
    QString role;
    QString photoPath;
    QString rfidUid;          // ← holds the scanned RFID UID during registration

    int  currentEditingId = -1;
    bool isEditMode       = false;
    bool isRegistrationMode = false;

    // ── Arduino / RFID ──────────────────────────────────────────────────────
    Arduino *arduino = nullptr;   // lazily created when needed

    // ── Private helpers ─────────────────────────────────────────────────────
    void setupTable();
    void setupMenus();
    void setupConnections();

    bool ajouter();

    void getFormData();
    void clearForm();
    void uploadPhoto();

    void showAddUserForm();
    void hideAddUserForm();

    // loadUserToTable overloads
    void loadUserToTable();
    void loadUserToTable(const QString &searchText,
                         const QString &searchType);
    void loadUserToTable(const QString &searchText,
                         const QString &searchType,
                         const QString &sortColumn);

    bool    checkUidInDatabase(const QString &uid, int *outUserId = nullptr);

private slots:
    void onModifyUser();
    void onDeleteUser();
    void updateUser();
    void loadUserDataForEdit(int userId);
    void resetToAddMode();
    void searchUsers();
    void sortUsers(QAction *action);
    void exportUsers();
    void statsUsers();
    void on_btnTogglePassword_clicked();
    void onRegisterFaceClicked();

    // ── RFID slots ──────────────────────────────────────────────────────────
    void onRegisterRfidClicked();   // button handler – starts listening
    void onRfidDataReady();         // called when serial data arrives
};

#endif // USERS_H
