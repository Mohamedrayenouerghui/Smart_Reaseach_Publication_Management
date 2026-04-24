#include "users.h"
#include "ui_users.h"
#include "connection.h"
#include "FaceCaptureDialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QMetaType>
#include <QDebug>
#include <QMenu>
#include <QVariant>
#include <QRegularExpression>

#include <QPrinter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QDate>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>

// QtCharts
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

// ═════════════════════════════════════════════════════════════════════════════
//  Constructor / Destructor
// ═════════════════════════════════════════════════════════════════════════════

Users::Users(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Users)
{
    ui->setupUi(this);

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    Connection &conn = Connection::createInstance();
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur Connexion",
                              "Impossible de se connecter à Oracle :\n" +
                                  QSqlDatabase::database().lastError().text());
        return;
    }

    setupTable();
    setupConnections();
    setupMenus();
    loadUserToTable();

    ui->usersTabWidget->setCurrentIndex(0);
    // ── ADD THIS TO CONNECT ARDUINO ON STARTUP ────────────────────────────
    arduino = new Arduino();
    if (arduino->connect_arduino() == 0) {
        qDebug() << "[SYSTEM] Arduino connected continuously for access control.";
        connect(arduino->getserial(), &QSerialPort::readyRead,
                this, &Users::onRfidDataReady);
    } else {
        qDebug() << "[SYSTEM] Arduino not found on startup.";
    }
    // ──────────────────────────────────────────────────────────────────────
}

Users::~Users()
{
    // Close Arduino serial port cleanly
    if (arduino) {
        arduino->close_arduino();
        delete arduino;
    }
    delete ui;
}

// ═════════════════════════════════════════════════════════════════════════════
//  setupTable  –  Column 7 = RFID UID, Column 8 = Actions
// ═════════════════════════════════════════════════════════════════════════════

void Users::setupTable()
{
    // 9 columns: ID | Nom | Prénom | Email | MDP | Photo | Role | RFID UID | Actions
    QStringList headers = {"ID_User", "Nom", "Prénom", "Email", "MDP",
                           "Photo", "Role", "RFID UID", "Actions"};
    ui->usersTable->setColumnCount(headers.size());
    ui->usersTable->setHorizontalHeaderLabels(headers);

    ui->usersTable->setColumnWidth(0, 80);
    ui->usersTable->setColumnWidth(1, 110);
    ui->usersTable->setColumnWidth(2, 110);
    ui->usersTable->setColumnWidth(3, 200);
    ui->usersTable->setColumnWidth(4, 90);
    ui->usersTable->setColumnWidth(5, 130);
    ui->usersTable->setColumnWidth(6, 110);
    ui->usersTable->setColumnWidth(7, 130);   // RFID UID
    ui->usersTable->setColumnWidth(8, 260);   // Actions
    ui->usersTable->verticalHeader()->setDefaultSectionSize(90);
    ui->usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// ═════════════════════════════════════════════════════════════════════════════
//  setupConnections
// ═════════════════════════════════════════════════════════════════════════════

void Users::setupConnections()
{
    connect(ui->addButtonUser,      &QToolButton::clicked,  this, &Users::showAddUserForm);
    connect(ui->submitButtonUser,   &QPushButton::clicked,  this, &Users::ajouter);
    connect(ui->cancelButtonUser,   &QPushButton::clicked,  this, &Users::clearForm);
    connect(ui->uploadPhotoButton,  &QPushButton::clicked,  this, &Users::uploadPhoto);
    connect(ui->clearButtonUser,    &QToolButton::clicked,  ui->searchInputUser, &QLineEdit::clear);
    connect(ui->searchButtonUser,   &QToolButton::clicked,  this, &Users::searchUsers);
    connect(ui->registerFaceButton, &QPushButton::clicked,  this, &Users::onRegisterFaceClicked);

    // ── RFID ─────────────────────────────────────────────────────────────────
    connect(ui->registerRfidButton, &QPushButton::clicked,  this, &Users::onRegisterRfidClicked);
}

// ═════════════════════════════════════════════════════════════════════════════
//  getFormData  –  reads all inputs including rfidUid (already stored)
// ═════════════════════════════════════════════════════════════════════════════

void Users::getFormData()
{
    nom      = ui->nomInput->text().trimmed();
    prenom   = ui->prenomInput->text().trimmed();
    email    = ui->emailInput->text().trimmed();
    password = ui->mdpInput->text();
    role     = ui->roleCombo->currentText();
    // rfidUid is populated by onRfidDataReady() – no UI widget to read
}

// ═════════════════════════════════════════════════════════════════════════════
//  ajouter  –  INSERT with RFID_UID
// ═════════════════════════════════════════════════════════════════════════════

bool Users::ajouter()
{
    getFormData();
    QString roleValue = ui->roleCombo->currentText();

    // ── Validation ────────────────────────────────────────────────────────────
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        password.isEmpty() || roleValue == "Sélectionnez un rôle...") {
        QMessageBox::warning(this, "Validation",
                             "Tous les champs suivants sont obligatoires :\n"
                             "- Nom\n- Prénom\n- Email\n- Mot de passe\n- Rôle");
        return false;
    }

    if (nom.length() < 2 || nom.length() > 50) {
        QMessageBox::warning(this, "Validation Nom",
                             "Le nom doit contenir entre 2 et 50 caractères.");
        ui->nomInput->setFocus(); return false;
    }
    if (!nom.contains(QRegularExpression("^[a-zA-ZÀ-ÿ\\s\\-']+$"))) {
        QMessageBox::warning(this, "Validation Nom",
                             "Le nom ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        ui->nomInput->setFocus(); return false;
    }

    if (prenom.length() < 2 || prenom.length() > 50) {
        QMessageBox::warning(this, "Validation Prénom",
                             "Le prénom doit contenir entre 2 et 50 caractères.");
        ui->prenomInput->setFocus(); return false;
    }
    if (!prenom.contains(QRegularExpression("^[a-zA-ZÀ-ÿ\\s\\-']+$"))) {
        QMessageBox::warning(this, "Validation Prénom",
                             "Le prénom ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        ui->prenomInput->setFocus(); return false;
    }

    QRegularExpression emailRegex(R"((^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$))");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Validation Email",
                             "Veuillez entrer une adresse email valide.");
        ui->emailInput->setFocus(); return false;
    }

    if (password.length() < 6 || password.length() > 100) {
        QMessageBox::warning(this, "Validation Mot de passe",
                             "Le mot de passe doit contenir entre 6 et 100 caractères.");
        ui->mdpInput->setFocus(); return false;
    }
    if (!password.contains(QRegularExpression("[a-zA-Z]")) ||
        !password.contains(QRegularExpression("[0-9]"))) {
        QMessageBox::warning(this, "Validation Mot de passe",
                             "Le mot de passe doit contenir au moins une lettre et un chiffre.");
        ui->mdpInput->setFocus(); return false;
    }

    // ── Generate ID ───────────────────────────────────────────────────────────
    QSqlQuery query;
    query.exec("SELECT NVL(MAX(ID_USER), 0) + 1 FROM USERS");
    if (query.next()) {
        id = query.value(0).toInt();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de générer l'ID");
        return false;
    }

    // ── INSERT  (includes RFID_UID) ────────────────────────────────────────────
    query.prepare(
        "INSERT INTO USERS (ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE, RFID_UID) "
        "VALUES (:id, :nom, :prenom, :email, :mdp, :photo, :role, :rfid)"
        );

    query.bindValue(":id",     id);
    query.bindValue(":nom",    nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email",  email);
    query.bindValue(":mdp",    password);
    query.bindValue(":photo",  photoPath.isEmpty()
                                  ? QVariant(QMetaType(QMetaType::QString))
                                  : QVariant(photoPath));
    query.bindValue(":role",   roleValue);
    query.bindValue(":rfid",   rfidUid.isEmpty()
                                 ? QVariant(QMetaType(QMetaType::QString))
                                 : QVariant(rfidUid));

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Échec Insertion", query.lastError().text());
        return false;
    }

    QMessageBox::information(this, "Succès", "Utilisateur ajouté avec succès !");
    loadUserToTable();
    clearForm();
    hideAddUserForm();
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
//  clearForm
// ═════════════════════════════════════════════════════════════════════════════

void Users::clearForm()
{
    ui->nomInput->clear();
    ui->prenomInput->clear();
    ui->emailInput->clear();
    ui->mdpInput->clear();
    ui->roleCombo->setCurrentIndex(0);
    ui->photoFileNameLabel->setText("Aucun fichier sélectionné");
    photoPath.clear();

    // Reset RFID state
    rfidUid.clear();
    ui->rfidStatusLabel->setText("🔷 Carte : non enregistrée");
    ui->rfidStatusLabel->setStyleSheet(
        "font-size:12px; color:#64748B; background:transparent;");
    ui->registerRfidButton->setEnabled(true);

    if (isEditMode) {
        resetToAddMode();
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  RFID  –  onRegisterRfidClicked()
//  Opens the Arduino serial connection and hooks readyRead so the next card
//  scan is captured and stored in rfidUid.
// ═════════════════════════════════════════════════════════════════════════════

void Users::onRegisterRfidClicked()
{
    // If Arduino disconnected for some reason, try to reconnect
    if (!arduino || !arduino->getserial()->isOpen()) {
        if (arduino->connect_arduino() != 0) {
            QMessageBox::warning(this, "Arduino introuvable",
                                 "Impossible de se connecter à l'Arduino.");
            return;
        }
        connect(arduino->getserial(), &QSerialPort::readyRead,
                this, &Users::onRfidDataReady);
    }

    // Set the flag! The next read in onRfidDataReady will be used for UI.
    isRegistrationMode = true;

    // Update UI
    ui->registerRfidButton->setEnabled(false);
    ui->rfidStatusLabel->setText("⏳ Approchez votre carte RFID du lecteur…");
    ui->rfidStatusLabel->setStyleSheet(
        "font-size:12px; color:#F59E0B; font-weight:600; background:transparent;");
}

//═════════════════════════════════════════════════════════════════════════════
//  checkUidInDatabase()
//  Returns true if the given RFID UID exists in the USERS table.
//  Optionally fills *outUserId with the matching ID_USER.
// ═════════════════════════════════════════════════════════════════════════════
bool Users::checkUidInDatabase(const QString &uid, int *outUserId)
{
    if (uid.isEmpty()) return false;

    QSqlQuery query;
    query.prepare("SELECT ID_USER FROM USERS WHERE RFID_UID = :uid");
    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "[RFID] DB query failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        if (outUserId)
            *outUserId = query.value(0).toInt();
        return true;
    }

    return false;   // UID not found
}

// ═════════════════════════════════════════════════════════════════════════════
//  RFID  –  onRfidDataReady()
//  Called each time the Arduino sends data.  We look for the line
//  "UID (DEC): XXX.XX.XX.XX" and extract the UID from it.
// ═════════════════════════════════════════════════════════════════════════════

// ═════════════════════════════════════════════════════════════════════════════
//  onRfidDataReady()  –  with database UID validation
// ═════════════════════════════════════════════════════════════════════════════
void Users::onRfidDataReady()
{
    if (!arduino || !arduino->getserial()->isOpen()) return;

    while (arduino->getserial()->canReadLine()) {
        QString line = QString::fromUtf8(arduino->getserial()->readLine()).trimmed();

        // Ignore noise, only look for the UID line sent by the Arduino code
        if (!line.contains("UID (DEC):")) continue;

        // 1. Extract the raw UID
        QString scannedUid = line.section(':', 1).trimmed();
        qDebug() << "[RFID] Scanned UID:" << scannedUid;

        // 2. Check Database
        int matchedUserId = -1;
        bool uidExists = checkUidInDatabase(scannedUid, &matchedUserId);

        // ──────────────────────────────────────────────────────────────────
        // SCENARIO A: ADMINISTRATOR IS REGISTERING A NEW USER
        // ──────────────────────────────────────────────────────────────────
        if (isRegistrationMode) {
            if (uidExists) {
                // Allow re-assigning to the *same* user being edited
                if (isEditMode && matchedUserId == currentEditingId) {
                    rfidUid = scannedUid;
                    ui->rfidStatusLabel->setText(QString("✅ Carte enregistrée : %1").arg(rfidUid));
                    ui->rfidStatusLabel->setStyleSheet("font-size:12px; color:#10B981; font-weight:600; background:transparent;");
                } else {
                    QMessageBox::warning(this, "UID déjà enregistré",
                                         QString("⚠️ Cette carte RFID est déjà assignée à l'utilisateur ID %1.").arg(matchedUserId));
                    ui->rfidStatusLabel->setText("⚠️ Carte déjà utilisée — réessayez");
                    ui->rfidStatusLabel->setStyleSheet("font-size:12px; color:#EF4444; font-weight:600; background:transparent;");
                }
            } else {
                // UID is new and valid
                rfidUid = scannedUid;
                ui->rfidStatusLabel->setText(QString("✅ Carte enregistrée : %1").arg(rfidUid));
                ui->rfidStatusLabel->setStyleSheet("font-size:12px; color:#10B981; font-weight:600; background:transparent;");
            }

            // Reset UI state
            ui->registerRfidButton->setEnabled(true);
            ui->registerRfidButton->setText("🔄 Changer la carte");

            // Turn off registration mode. The next scan goes back to Gate Control.
            isRegistrationMode = false;
        }

        // ──────────────────────────────────────────────────────────────────
        // SCENARIO B: NORMAL GATE ACCESS CONTROL (BACKGROUND OPERATION)
        // ──────────────────────────────────────────────────────────────────
        else {
            if (uidExists) {
                qDebug() << "[ACCESS GRANTED] User ID:" << matchedUserId;
                arduino->write_to_arduino("O"); // Send 'Open' command to Arduino
            } else {
                qDebug() << "[ACCESS DENIED] Unknown card.";
                arduino->write_to_arduino("D"); // Send 'Deny' command to Arduino
            }
        }
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  onRegisterFaceClicked  (unchanged)
// ═════════════════════════════════════════════════════════════════════════════

void Users::onRegisterFaceClicked()
{
    QString email = ui->emailInput->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Email requis",
                             "Veuillez d'abord saisir l'adresse email de l'utilisateur\n"
                             "avant d'enregistrer son visage.");
        ui->emailInput->setFocus();
        return;
    }

    ui->registerFaceButton->setEnabled(false);
    ui->faceStatusLabel->setText("⏳ Ouverture de la caméra…");

    FaceCaptureDialog dlg(email, this);
    if (dlg.exec() == QDialog::Accepted) {
        ui->faceStatusLabel->setText("✅ Visage enregistré avec succès !");
        ui->faceStatusLabel->setStyleSheet(
            "font-size:12px; color:#10B981; font-weight:600; background:transparent;");
    } else {
        ui->faceStatusLabel->setText("⚠️ Enregistrement annulé");
        ui->faceStatusLabel->setStyleSheet(
            "font-size:12px; color:#F59E0B; background:transparent;");
    }

    ui->registerFaceButton->setEnabled(true);
}

// ═════════════════════════════════════════════════════════════════════════════
//  uploadPhoto  (unchanged)
// ═════════════════════════════════════════════════════════════════════════════

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

// ═════════════════════════════════════════════════════════════════════════════
//  Delete / Modify
// ═════════════════════════════════════════════════════════════════════════════

void Users::onDeleteUser()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int userId = btn->property("userId").toInt();

    auto reply = QMessageBox::question(this, "Confirmation",
                                       QString("Voulez-vous vraiment supprimer l'utilisateur ID %1 ?").arg(userId),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    QSqlQuery query;
    query.prepare("DELETE FROM USERS WHERE ID_USER = :id");
    query.bindValue(":id", userId);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Utilisateur supprimé avec succès.");
        loadUserToTable();
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de supprimer l'utilisateur :\n" + query.lastError().text());
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
    showAddUserForm();

    ui->submitButtonUser->setText("Modifier");
    ui->submitButtonUser->disconnect();
    connect(ui->submitButtonUser, &QPushButton::clicked, this, &Users::updateUser);
}

// ═════════════════════════════════════════════════════════════════════════════
//  loadUserDataForEdit  –  includes RFID_UID
// ═════════════════════════════════════════════════════════════════════════════

void Users::loadUserDataForEdit(int userId)
{
    QSqlQuery query;
    query.prepare(
        "SELECT NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE, RFID_UID "
        "FROM USERS WHERE ID_USER = :id");
    query.bindValue(":id", userId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de charger les données de l'utilisateur.");
        return;
    }

    ui->nomInput->setText(query.value(0).toString());
    ui->prenomInput->setText(query.value(1).toString());
    ui->emailInput->setText(query.value(2).toString());
    ui->mdpInput->setText(query.value(3).toString());

    QString photo = query.value(4).toString();
    if (!photo.isEmpty()) {
        photoPath = photo;
        ui->photoFileNameLabel->setText(QFileInfo(photo).fileName());
    } else {
        photoPath.clear();
        ui->photoFileNameLabel->setText("Aucun fichier sélectionné");
    }

    QString roleStr = query.value(5).toString();
    int idx = ui->roleCombo->findText(roleStr);
    if (idx >= 0) ui->roleCombo->setCurrentIndex(idx);
    else           ui->roleCombo->setCurrentText(roleStr);

    // Restore RFID UID
    rfidUid = query.value(6).toString();
    if (!rfidUid.isEmpty()) {
        ui->rfidStatusLabel->setText(QString("✅ Carte : %1").arg(rfidUid));
        ui->rfidStatusLabel->setStyleSheet(
            "font-size:12px; color:#10B981; font-weight:600; background:transparent;");
        ui->registerRfidButton->setText("🔄 Changer la carte");
    } else {
        ui->rfidStatusLabel->setText("🔷 Carte : non enregistrée");
        ui->rfidStatusLabel->setStyleSheet(
            "font-size:12px; color:#64748B; background:transparent;");
        ui->registerRfidButton->setText("🔷 Enregistrer carte RFID");
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  updateUser  –  UPDATE with RFID_UID
// ═════════════════════════════════════════════════════════════════════════════

void Users::updateUser()
{
    getFormData();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Validation",
                             "Nom, Prénom et Email sont obligatoires.");
        return;
    }

    QString finalPassword = password;
    bool keepPassword = finalPassword.isEmpty();

    QSqlQuery query;
    QString sql;

    if (keepPassword) {
        sql = "UPDATE USERS SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
              "PHOTO = :photo, ROLE = :role, RFID_UID = :rfid "
              "WHERE ID_USER = :id";
    } else {
        sql = "UPDATE USERS SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
              "MDP = :mdp, PHOTO = :photo, ROLE = :role, RFID_UID = :rfid "
              "WHERE ID_USER = :id";
    }

    query.prepare(sql);
    query.bindValue(":id",     currentEditingId);
    query.bindValue(":nom",    nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email",  email);
    query.bindValue(":photo",  photoPath.isEmpty()
                                  ? QVariant(QMetaType(QMetaType::QString))
                                  : QVariant(photoPath));
    query.bindValue(":role",   ui->roleCombo->currentText());
    query.bindValue(":rfid",   rfidUid.isEmpty()
                                 ? QVariant(QMetaType(QMetaType::QString))
                                 : QVariant(rfidUid));

    if (!keepPassword) {
        query.bindValue(":mdp", finalPassword);
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL Update:" << query.lastError().text();
        QMessageBox::critical(this, "Échec Modification", query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Utilisateur modifié avec succès !");
    resetToAddMode();
    loadUserToTable();
    hideAddUserForm();
}

// ═════════════════════════════════════════════════════════════════════════════
//  resetToAddMode
// ═════════════════════════════════════════════════════════════════════════════

void Users::resetToAddMode()
{
    isEditMode       = false;
    currentEditingId = -1;
    ui->submitButtonUser->setText("Ajouter Utilisateur");

    ui->submitButtonUser->disconnect();
    connect(ui->submitButtonUser, &QPushButton::clicked, this, &Users::ajouter);

    clearForm();
}

// ═════════════════════════════════════════════════════════════════════════════
//  loadUserToTable  overloads  –  SELECT includes RFID_UID
// ═════════════════════════════════════════════════════════════════════════════

void Users::loadUserToTable()
{
    loadUserToTable("", "", "ID_USER");
}

void Users::loadUserToTable(const QString &searchText, const QString &searchType)
{
    loadUserToTable(searchText, searchType, "ID_USER");
}

void Users::loadUserToTable(const QString &searchText,
                            const QString &searchType,
                            const QString &sortColumn)
{
    ui->usersTable->setRowCount(0);

    // Columns: ID | NOM | PRENOM | EMAIL | MDP | PHOTO | ROLE | RFID_UID
    QString sql = "SELECT ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE, RFID_UID FROM USERS";

    bool hasFilter = !searchText.isEmpty();
    QString filterColumn;

    if (hasFilter) {
        if      (searchType == "Par Nom")   filterColumn = "NOM";
        else if (searchType == "Par Email") filterColumn = "EMAIL";
        else if (searchType == "Par Role")  filterColumn = "ROLE";
        else                                hasFilter    = false;
    }

    if (hasFilter) {
        sql += " WHERE UPPER(" + filterColumn + ") LIKE UPPER(:search)";
    }

    sql += " ORDER BY " + sortColumn;

    QSqlQuery query;
    query.prepare(sql);
    if (hasFilter) {
        query.bindValue(":search", "%" + searchText + "%");
    }

    if (!query.exec()) {
        qDebug() << "Erreur recherche SQL :" << query.lastError().text();
        QMessageBox::warning(this, "Recherche", "Impossible d'exécuter la recherche.");
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->usersTable->insertRow(row);

        ui->usersTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID
        ui->usersTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // NOM
        ui->usersTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // PRENOM
        ui->usersTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // EMAIL
        ui->usersTable->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // MDP
        ui->usersTable->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // PHOTO
        ui->usersTable->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // ROLE

        // RFID UID  – show a friendly indicator when set
        QString uid = query.value(7).toString();
        QTableWidgetItem *rfidItem = new QTableWidgetItem(uid.isEmpty() ? "—" : uid);
        if (!uid.isEmpty()) {
            rfidItem->setForeground(QBrush(QColor("#10B981")));
            rfidItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        }
        ui->usersTable->setItem(row, 7, rfidItem);

        // ── Action buttons ────────────────────────────────────────────────────
        QWidget    *actionWidget = new QWidget();
        QHBoxLayout *layout     = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(4, 2, 4, 2);
        layout->setSpacing(10);

        QPushButton *modifyBtn = new QPushButton("✏️ Modify");
        modifyBtn->setFixedSize(110, 60);
        modifyBtn->setStyleSheet(
            "QPushButton { background-color:#4CAF50; color:white; border:none; "
            "border-radius:10px; font-weight:bold; }"
            "QPushButton:hover { background-color:#45a049; }");

        QPushButton *deleteBtn = new QPushButton("🗑️ Delete");
        deleteBtn->setFixedSize(110, 60);
        deleteBtn->setStyleSheet(
            "QPushButton { background-color:#f44336; color:white; border:none; "
            "border-radius:10px; font-weight:bold; }"
            "QPushButton:hover { background-color:#da190b; }");

        layout->addWidget(modifyBtn);
        layout->addWidget(deleteBtn);

        modifyBtn->setProperty("userId", query.value(0).toInt());
        deleteBtn->setProperty("userId", query.value(0).toInt());

        connect(modifyBtn, &QPushButton::clicked, this, &Users::onModifyUser);
        connect(deleteBtn, &QPushButton::clicked, this, &Users::onDeleteUser);

        ui->usersTable->setCellWidget(row, 8, actionWidget); // column 8 = Actions
        row++;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  searchUsers
// ═════════════════════════════════════════════════════════════════════════════

void Users::searchUsers()
{
    loadUserToTable(ui->searchInputUser->text().trimmed(),
                    ui->searchTypeComboUser->currentText());
}

// ═════════════════════════════════════════════════════════════════════════════
//  showAddUserForm / hideAddUserForm
// ═════════════════════════════════════════════════════════════════════════════

void Users::showAddUserForm() { ui->usersTabWidget->setCurrentIndex(1); }
void Users::hideAddUserForm() { ui->usersTabWidget->setCurrentIndex(0); }

// ═════════════════════════════════════════════════════════════════════════════
//  setupMenus  (unchanged logic, kept intact)
// ═════════════════════════════════════════════════════════════════════════════

void Users::setupMenus()
{
    QMenu *sortMenu = new QMenu(ui->sortButtonUser);
    sortMenu->addAction("Par ID");
    sortMenu->addAction("Par Nom");
    sortMenu->addAction("Par Prénom");
    sortMenu->addAction("Par Email");
    sortMenu->addAction("Par Role");
    ui->sortButtonUser->setMenu(sortMenu);

    QMenu *exportMenu = new QMenu(ui->exportButtonUser);
    exportMenu->addAction("Exporter en PDF");
    ui->exportButtonUser->setMenu(exportMenu);

    connect(ui->statsButtonUser, &QToolButton::clicked, this, &Users::statsUsers);
    connect(sortMenu,            &QMenu::triggered,     this, &Users::sortUsers);
    connect(exportMenu,          &QMenu::triggered,     this, &Users::exportUsers);
}

// ═════════════════════════════════════════════════════════════════════════════
//  sortUsers
// ═════════════════════════════════════════════════════════════════════════════

void Users::sortUsers(QAction *action)
{
    if (!action) return;

    QString sortColumn;
    QString text = action->text();

    if      (text == "Par ID")      sortColumn = "ID_USER";
    else if (text == "Par Nom")     sortColumn = "NOM";
    else if (text == "Par Prénom")  sortColumn = "PRENOM";
    else if (text == "Par Email")   sortColumn = "EMAIL";
    else if (text == "Par Role")    sortColumn = "ROLE";
    else return;

    loadUserToTable(ui->searchInputUser->text().trimmed(),
                    ui->searchTypeComboUser->currentText(),
                    sortColumn);
}

// ═════════════════════════════════════════════════════════════════════════════
//  on_btnTogglePassword_clicked
// ═════════════════════════════════════════════════════════════════════════════

void Users::on_btnTogglePassword_clicked()
{
    if (ui->mdpInput->echoMode() == QLineEdit::Password) {
        ui->mdpInput->setEchoMode(QLineEdit::Normal);
        ui->btnTogglePassword->setText("👁️‍🗨️");
    } else {
        ui->mdpInput->setEchoMode(QLineEdit::Password);
        ui->btnTogglePassword->setText("👁️");
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  exportUsers  (unchanged – only PDF column count/widths stay at 7 data cols)
// ═════════════════════════════════════════════════════════════════════════════

void Users::exportUsers()
{
    QString defaultPath = QDir::homePath() + "/Liste_Utilisateurs_"
                          + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter la liste des utilisateurs en PDF",
                                                    defaultPath,
                                                    "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

    // Fetch all users including RFID_UID
    QSqlQuery query;
    query.prepare(
        "SELECT ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE, RFID_UID "
        "FROM USERS ORDER BY ID_USER ASC");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur Export",
                              "Impossible de charger les données :\n" + query.lastError().text());
        return;
    }

    QList<QStringList> usersData;
    while (query.next()) {
        QStringList row;
        for (int i = 0; i < 8; ++i)
            row << query.value(i).toString();
        usersData << row;
    }

    if (usersData.isEmpty()) {
        QMessageBox::information(this, "Export PDF", "Aucun utilisateur à exporter.");
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);   // landscape fits 8 cols better

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur PDF", "Impossible de créer le document PDF.");
        return;
    }

    int pageWidth  = painter.device()->width();
    int pageHeight = painter.device()->height();

    QColor primary(139, 92, 246);
    QColor textColor(30, 41, 59);
    QColor gridColor(148, 163, 184);
    QColor lightRow(248, 250, 252);

    // Title
    painter.setPen(primary);
    painter.setFont(QFont("Segoe UI", 24, QFont::Bold));
    painter.drawText(0, 60, pageWidth, 50, Qt::AlignCenter, "Liste des Utilisateurs");

    painter.setPen(textColor);
    painter.setFont(QFont("Segoe UI", 11));
    painter.drawText(0, 110, pageWidth, 25, Qt::AlignCenter,
                     QString("Export • Base Oracle • %1")
                         .arg(QDate::currentDate().toString("dddd dd MMMM yyyy")));

    // Table layout – 8 data columns
    int marginLeft = 30;
    int rowHeight  = 38;
    int colWidths[8] = { 45, 85, 85, 150, 80, 100, 85, 110 };
    int colPos[9] = { marginLeft };
    for (int i = 0; i < 8; ++i)
        colPos[i + 1] = colPos[i] + colWidths[i];

    int y = 160;

    QStringList headers = {"ID", "Nom", "Prénom", "Email", "MDP", "Photo", "Rôle", "RFID UID"};

    painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
    painter.setPen(textColor);
    for (int i = 0; i < 8; ++i) {
        QRect rect(colPos[i], y, colWidths[i], rowHeight);
        painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
    }
    painter.setPen(QPen(primary, 3));
    painter.drawLine(colPos[0], y + rowHeight + 3, colPos[8], y + rowHeight + 3);
    y += rowHeight + 12;

    painter.setFont(QFont("Segoe UI", 9));

    for (int rowIndex = 0; rowIndex < usersData.size(); ++rowIndex) {
        if (y + rowHeight > pageHeight - 60) {
            if (!printer.newPage()) break;
            y = 60;
        }

        if (rowIndex % 2 == 1)
            painter.fillRect(marginLeft, y, colPos[8] - marginLeft, rowHeight, lightRow);

        painter.setPen(textColor);
        const QStringList &rowData = usersData[rowIndex];

        for (int i = 0; i < 8; ++i) {
            QString txt = rowData[i];
            if (i == 5) { txt = QFileInfo(txt).fileName(); if (txt.isEmpty()) txt = "—"; }
            if (i == 7 && txt.isEmpty()) txt = "—";

            QRect rect(colPos[i], y, colWidths[i], rowHeight);
            painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, txt);
        }

        painter.setPen(QPen(gridColor, 1));
        painter.drawLine(colPos[0], y + rowHeight, colPos[8], y + rowHeight);
        for (int i = 0; i <= 8; ++i)
            painter.drawLine(colPos[i], y, colPos[i], y + rowHeight);

        y += rowHeight;
    }

    painter.end();

    QFileInfo fi(fileName);
    QMessageBox::information(this, "Export PDF Terminé",
                             QString("✅ PDF créé avec succès !\n\nFichier : %1\nChemin : %2\n"
                                     "Utilisateurs exportés : %3")
                                 .arg(fi.fileName())
                                 .arg(fi.absolutePath())
                                 .arg(usersData.size()));

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

// ═════════════════════════════════════════════════════════════════════════════
//  statsUsers  (unchanged)
// ═════════════════════════════════════════════════════════════════════════════

void Users::statsUsers()
{
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Utilisateurs");
    statsDialog->resize(950, 720);
    statsDialog->setStyleSheet("QDialog { background-color: #F8FAFC; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("📊 Statistiques des Utilisateurs par Rôle");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size:24px; font-weight:bold; color:#8B5CF6; padding:10px;");
    mainLayout->addWidget(titleLabel);

    QSqlQuery query;
    query.prepare("SELECT ROLE, COUNT(*) as count FROM USERS GROUP BY ROLE ORDER BY count DESC");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible de charger les statistiques :\n" + query.lastError().text());
        statsDialog->deleteLater();
        return;
    }

    int totalUsers = 0;
    QList<QPair<QString, int>> data;

    while (query.next()) {
        QString role = query.value(0).toString().trimmed();
        if (role.isEmpty()) role = "Non défini";
        int count = query.value(1).toInt();
        data.append(qMakePair(role, count));
        totalUsers += count;
    }

    if (totalUsers == 0) {
        QLabel *noData = new QLabel("Aucune donnée utilisateur disponible.");
        noData->setAlignment(Qt::AlignCenter);
        noData->setStyleSheet("font-size:18px; color:#64748B; padding:80px;");
        mainLayout->addWidget(noData);
    } else {
        QPieSeries *series = new QPieSeries();

        QList<QColor> colors = {
            QColor(139,92,246), QColor(59,130,246), QColor(16,185,129),
            QColor(245,158,11), QColor(239,68,68),  QColor(236,72,153),
            QColor(99,102,241), QColor(20,184,166)
        };

        int colorIndex = 0;
        for (const auto &item : data) {
            QPieSlice *slice = series->append(item.first, item.second);
            slice->setLabelVisible(true);
            double pct = 100.0 * item.second / totalUsers;
            slice->setLabel(QString("%1: %2 (%3%)")
                                .arg(item.first).arg(item.second)
                                .arg(pct, 0, 'f', 1));
            slice->setBrush(colors[colorIndex++ % colors.size()]);
            slice->setExplodeDistanceFactor(0.05);
            connect(slice, &QPieSlice::hovered, [slice](bool show) {
                slice->setExploded(show);
            });
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        series->setParent(chart);
        chart->setTitle(QString("Répartition par Rôle (%1 utilisateurs)").arg(totalUsers));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->setBackgroundBrush(QBrush(QColor("#FFFFFF")));

        QFont titleFont;
        titleFont.setPixelSize(20);
        titleFont.setBold(true);
        chart->setTitleFont(titleFont);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumHeight(480);
        mainLayout->addWidget(chartView);
    }

    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet(
        "QPushButton { background-color:#8B5CF6; color:white; border:none; "
        "border-radius:8px; padding:12px 40px; font-size:15px; font-weight:bold; }"
        "QPushButton:hover { background-color:#7C3AED; }");
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);
    connect(closeBtn, &QPushButton::clicked, statsDialog, &QDialog::accept);

    statsDialog->exec();
    delete statsDialog;
}
