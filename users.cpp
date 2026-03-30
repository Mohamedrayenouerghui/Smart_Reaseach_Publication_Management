#include "users.h"
#include "ui_users.h"
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QMetaType>
#include <QDebug>

Users::Users(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Users)
{
    ui->setupUi(this);

    setupTable();
    setupConnections();
    loadUserToTable();

    // Onglet par défaut = Liste
    ui->usersTabWidget->setCurrentIndex(0);
}

Users::~Users()
{
    delete ui;
}

void Users::setupTable()
{
    QStringList headers = {"ID_User", "Nom", "Prénom", "Email", "MDP", "Photo", "Role", "Actions"};
    ui->usersTable->setColumnCount(headers.size());
    ui->usersTable->setHorizontalHeaderLabels(headers);

    ui->usersTable->setColumnWidth(0, 100);
    ui->usersTable->setColumnWidth(1, 120);
    ui->usersTable->setColumnWidth(2, 120);
    ui->usersTable->setColumnWidth(3, 220);
    ui->usersTable->setColumnWidth(4, 100);
    ui->usersTable->setColumnWidth(5, 150);
    ui->usersTable->setColumnWidth(6, 120);
    ui->usersTable->setColumnWidth(7, 280);
    ui->usersTable->verticalHeader()->setDefaultSectionSize(90);
    ui->usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Users::setupConnections()
{
    connect(ui->addButtonUser, &QToolButton::clicked, this, &Users::showAddUserForm);
    connect(ui->submitButtonUser, &QPushButton::clicked, this, &Users::ajouter);
    connect(ui->cancelButtonUser, &QPushButton::clicked, this, &Users::clearForm);
    connect(ui->uploadPhotoButton, &QPushButton::clicked, this, &Users::uploadPhoto);
    connect(ui->clearButtonUser, &QToolButton::clicked, ui->searchInputUser, &QLineEdit::clear);
}

void Users::getFormData()
{
    nom = ui->nomInput->text().trimmed();
    prenom = ui->prenomInput->text().trimmed();
    email = ui->emailInput->text().trimmed();
    password = ui->mdpInput->text();
    role = ui->roleCombo->currentText();        // conservé en mémoire (colonne ROLE inexistante dans DB)
    // photoPath est géré séparément
}

bool Users::ajouter()
{
    getFormData();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Nom, Prénom, Email et Mot de passe sont obligatoires.");
        return false;
    }

    QSqlQuery query(Connection::createInstance().getDb());

    // Génération ID
    query.exec("SELECT NVL(MAX(ID_USER), 0) + 1 FROM USERS");
    if (query.next()) {
        id = query.value(0).toInt();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de générer l'ID");
        return false;
    }

    // ────────────────────────────────────────────────
    // SOLUTION : on inclut ROLE et on bind la valeur
    // ────────────────────────────────────────────────
    QString roleValue = ui->roleCombo->currentText();  // "Responsable RH", "Admin", etc.

    query.prepare(
        "INSERT INTO USERS (ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE) "
        "VALUES (:id, :nom, :prenom, :email, :mdp, :photo, :role)"
        );

    query.bindValue(":id",    id);
    query.bindValue(":nom",   nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":email", email);
    query.bindValue(":mdp",   password);
    query.bindValue(":photo", photoPath.isEmpty() ? QVariant(QVariant::String) : photoPath);
    query.bindValue(":role",  roleValue);   // ← clé du fix

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Échec Insertion", query.lastError().text());
        return false;
    }

    QMessageBox::information(this, "Succès", "Utilisateur ajouté !");
    loadUserToTable();
    clearForm();
    hideAddUserForm();
    return true;
}

void Users::uploadPhoto()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Choisir une photo", "",
                                                    "Images (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!filePath.isEmpty()) {
        photoPath = filePath;
        ui->photoFileNameLabel->setText(QFileInfo(filePath).fileName());
    }
}

void Users::clearForm()
{
    ui->nomInput->clear();
    ui->prenomInput->clear();
    ui->emailInput->clear();
    ui->mdpInput->clear();
    ui->roleCombo->setCurrentIndex(0);
    ui->photoFileNameLabel->setText("Aucun fichier sélectionné");
    photoPath.clear();

    // If we were in edit mode, reset it
    if (isEditMode) {
        resetToAddMode();
    }
}

void Users::onDeleteUser()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int userId = btn->property("userId").toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              QString("Voulez-vous vraiment supprimer l'utilisateur ID %1 ?").arg(userId),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("DELETE FROM USERS WHERE ID_USER = :id");
    query.bindValue(":id", userId);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Utilisateur supprimé avec succès.");
        loadUserToTable();   // Refresh table
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'utilisateur :\n"
                                                  + query.lastError().text());
    }
}

void Users::onModifyUser()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int userId = btn->property("userId").toInt();
    if (userId <= 0) return;

    currentEditingId = userId;
    isEditMode = true;

    loadUserDataForEdit(userId);
    showAddUserForm();  // Switch to the form tab

    // Change button text and behavior for edit mode
    ui->submitButtonUser->setText("Modifier");
    ui->submitButtonUser->disconnect();  // Remove old connection
    connect(ui->submitButtonUser, &QPushButton::clicked, this, &Users::updateUser);
}

void Users::loadUserDataForEdit(int userId)
{
    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("SELECT NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE FROM USERS WHERE ID_USER = :id");
    query.bindValue(":id", userId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données de l'utilisateur.");
        return;
    }

    // Fill the form with existing data
    ui->nomInput->setText(query.value(0).toString());
    ui->prenomInput->setText(query.value(1).toString());
    ui->emailInput->setText(query.value(2).toString());
    ui->mdpInput->setText(query.value(3).toString());  // Note: In production, never show real password in plain text!

    QString photo = query.value(4).toString();
    if (!photo.isEmpty()) {
        photoPath = photo;
        ui->photoFileNameLabel->setText(QFileInfo(photo).fileName());
    } else {
        photoPath.clear();
        ui->photoFileNameLabel->setText("Aucun fichier sélectionné");
    }

    // Set role in combo box
    QString roleStr = query.value(5).toString();
    int index = ui->roleCombo->findText(roleStr);
    if (index >= 0) {
        ui->roleCombo->setCurrentIndex(index);
    } else {
        ui->roleCombo->setCurrentText(roleStr);
    }
}

void Users::updateUser()
{
    getFormData();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Validation",
                             "Nom, Prénom et Email sont obligatoires.");
        return;
    }

    // Password is optional during update (keep old one if empty)
    QString finalPassword = password;
    if (finalPassword.isEmpty()) {
        // Keep existing password - we'll handle this in the query
        finalPassword = "KEEP_EXISTING";  // Special flag
    }

    QSqlQuery query(Connection::createInstance().getDb());
    QString sql;

    if (finalPassword == "KEEP_EXISTING") {
        sql = "UPDATE USERS SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
              "PHOTO = :photo, ROLE = :role WHERE ID_USER = :id";
    } else {
        sql = "UPDATE USERS SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
              "MDP = :mdp, PHOTO = :photo, ROLE = :role WHERE ID_USER = :id";
    }

    query.prepare(sql);
    query.bindValue(":id",    currentEditingId);
    query.bindValue(":nom",   nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":email", email);
    query.bindValue(":photo", photoPath.isEmpty() ? QVariant(QVariant::String) : photoPath);
    query.bindValue(":role",  ui->roleCombo->currentText());

    if (finalPassword != "KEEP_EXISTING") {
        query.bindValue(":mdp", finalPassword);
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL Update:" << query.lastError().text();
        QMessageBox::critical(this, "Échec Modification", query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Utilisateur modifié avec succès !");

    // Reset to add mode
    resetToAddMode();

    loadUserToTable();        // Refresh the table
    hideAddUserForm();        // Go back to list
}

void Users::resetToAddMode()
{
    isEditMode = false;
    currentEditingId = -1;
    ui->submitButtonUser->setText("Ajouter");

    // Reconnect to the original ajouter() slot
    ui->submitButtonUser->disconnect();
    connect(ui->submitButtonUser, &QPushButton::clicked, this, &Users::ajouter);

    clearForm();
}

void Users::loadUserToTable()
{
    ui->usersTable->setRowCount(0);

    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE FROM USERS ORDER BY ID_USER");
    int row = 0;
    while (query.next()) {
        ui->usersTable->insertRow(row);
        ui->usersTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->usersTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->usersTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->usersTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->usersTable->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // masque le mot de passe
        ui->usersTable->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->usersTable->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));

        // === COLUMN 6: Actions (Delete + Modify buttons) ===
        QWidget *actionWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(4, 2, 4, 2);
        layout->setSpacing(10);
        // Modify Button
        QPushButton *modifyBtn = new QPushButton("✏️ Modify");
        modifyBtn->setFixedSize(120, 60);
        modifyBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; border-radius: 10px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #45a049; }");

        // Delete Button
        QPushButton *deleteBtn = new QPushButton("🗑️ Deleat");
        deleteBtn->setFixedSize(120, 60);
        deleteBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; border-radius: 10px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #da190b; }");

        layout->addWidget(modifyBtn);
        layout->addWidget(deleteBtn);

        // Store the user ID in the buttons for easy access later
        modifyBtn->setProperty("userId", query.value(0).toInt());
        deleteBtn->setProperty("userId", query.value(0).toInt());

        // Connect buttons to slots
        connect(modifyBtn, &QPushButton::clicked, this, &Users::onModifyUser);
        connect(deleteBtn, &QPushButton::clicked, this, &Users::onDeleteUser);

        ui->usersTable->setCellWidget(row, 7, actionWidget);
        row++;
    }
}

// Méthodes demandées dans users.h
void Users::showAddUserForm() { ui->usersTabWidget->setCurrentIndex(1); }
void Users::hideAddUserForm() { ui->usersTabWidget->setCurrentIndex(0); }
void Users::setupMenus() {}          // À compléter plus tard (menus Trier/Exporter)
void Users::setupPrototypeUser() {}  // Non utilisé pour l'instant
