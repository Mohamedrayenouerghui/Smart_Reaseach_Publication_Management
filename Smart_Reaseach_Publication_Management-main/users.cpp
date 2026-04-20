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
// === QtCharts includes (REQUIRED for statsUsers) ===
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

Users::Users(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Users)
{
    ui->setupUi(this);

    // Nettoyage propre des anciennes connexions
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Connexion Oracle
    Connection &conn = Connection::createInstance();
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur Connexion",
                              "Impossible de se connecter à Oracle :\n" +
                                  QSqlDatabase::database().lastError().text());
        return;
    }

    setupTable();
    setupConnections();
    setupMenus();        // ← Important pour le bouton Trier
    loadUserToTable();

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
    // === NOUVEAU : Bouton Recherche ===
    connect(ui->searchButtonUser, &QToolButton::clicked, this, &Users::searchUsers);
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
    QString roleValue = ui->roleCombo->currentText();


    // ==================== CONTRÔLES DE SAISIE ====================

    // 1. Vérification des champs obligatoires
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || password.isEmpty() || roleValue=="Sélectionnez un rôle...") {
        QMessageBox::warning(this, "Validation",
                             "Tous les champs suivants sont obligatoires :\n"
                             "- Nom\n- Prénom\n- Email\n- Mot de passe");
        return false;
    }

    // 2. Contrôle du Nom
    if (nom.length() < 2 || nom.length() > 50) {
        QMessageBox::warning(this, "Validation Nom",
                             "Le nom doit contenir entre 2 et 50 caractères.");
        ui->nomInput->setFocus();
        return false;
    }
    if (!nom.contains(QRegularExpression("^[a-zA-ZÀ-ÿ\\s\\-']+$"))) {
        QMessageBox::warning(this, "Validation Nom",
                             "Le nom ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        ui->nomInput->setFocus();
        return false;
    }

    // 3. Contrôle du Prénom
    if (prenom.length() < 2 || prenom.length() > 50) {
        QMessageBox::warning(this, "Validation Prénom",
                             "Le prénom doit contenir entre 2 et 50 caractères.");
        ui->prenomInput->setFocus();
        return false;
    }
    if (!prenom.contains(QRegularExpression("^[a-zA-ZÀ-ÿ\\s\\-']+$"))) {
        QMessageBox::warning(this, "Validation Prénom",
                             "Le prénom ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        ui->prenomInput->setFocus();
        return false;
    }

    // 4. Contrôle de l'Email (format basique mais efficace)
    QRegularExpression emailRegex(R"((^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$))");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Validation Email",
                             "Veuillez entrer une adresse email valide (ex: exemple@domaine.com).");
        ui->emailInput->setFocus();
        return false;
    }

    // 5. Contrôle du Mot de passe
    if (password.length() < 6) {
        QMessageBox::warning(this, "Validation Mot de passe",
                             "Le mot de passe doit contenir au moins 6 caractères.");
        ui->mdpInput->setFocus();
        return false;
    }
    if (password.length() > 100) {
        QMessageBox::warning(this, "Validation Mot de passe",
                             "Le mot de passe est trop long (maximum 100 caractères).");
        ui->mdpInput->setFocus();
        return false;
    }

    // Optionnel : Exiger au moins une lettre et un chiffre
    if (!password.contains(QRegularExpression("[a-zA-Z]")) ||
        !password.contains(QRegularExpression("[0-9]"))) {
        QMessageBox::warning(this, "Validation Mot de passe",
                             "Le mot de passe doit contenir au moins une lettre et un chiffre.");
        ui->mdpInput->setFocus();
        return false;
    }

    // ==================== INSERTION EN BASE ====================

    QSqlQuery query;

    // Génération ID
    query.exec("SELECT NVL(MAX(ID_USER), 0) + 1 FROM USERS");
    if (query.next()) {
        id = query.value(0).toInt();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de générer l'ID");
        return false;
    }

    //
    query.prepare(
        "INSERT INTO USERS (ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE) "
        "VALUES (:id, :nom, :prenom, :email, :mdp, :photo, :role)"
        );

    query.bindValue(":id",    id);
    query.bindValue(":nom",   nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":email", email);
    query.bindValue(":mdp",   password);
    query.bindValue(":photo", photoPath.isEmpty()
                                  ? QVariant(QMetaType(QMetaType::QString))
                                  : QVariant(photoPath));
    query.bindValue(":role",  roleValue);

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

    QSqlQuery query;
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
    QSqlQuery query;
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

    QSqlQuery query;
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
    query.bindValue(":photo", photoPath.isEmpty()
                                  ? QVariant(QMetaType(QMetaType::QString))
                                  : QVariant(photoPath));
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

// Surcharge sans paramètre (appelée par le constructeur, ajouter, supprimer, etc.)
void Users::loadUserToTable()
{
    loadUserToTable("", "", "ID_USER");
}

// Surcharge avec recherche (appelée par searchUsers())
void Users::loadUserToTable(const QString &searchText, const QString &searchType)
{
    loadUserToTable(searchText, searchType, "ID_USER");
}

// Fonction principale avec tri
void Users::loadUserToTable(const QString &searchText, const QString &searchType, const QString &sortColumn)
{
    ui->usersTable->setRowCount(0);

    QString sql = "SELECT ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE FROM USERS";

    bool hasFilter = !searchText.isEmpty();
    QString filterColumn;

    if (hasFilter) {
        if (searchType == "Par Nom") {
            filterColumn = "NOM";
        } else if (searchType == "Par Email") {
            filterColumn = "EMAIL";
        } else if (searchType == "Par Role") {
            filterColumn = "ROLE";
        } else {
            hasFilter = false;
        }
    }

    if (hasFilter) {
        sql += " WHERE UPPER(" + filterColumn + ") LIKE UPPER(:search)";
    }

    sql += " ORDER BY " + sortColumn;   // ← TRI DYNAMIQUE

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

        ui->usersTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->usersTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->usersTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->usersTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->usersTable->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // MDP
        ui->usersTable->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // PHOTO
        ui->usersTable->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // ROLE

        // === Actions (code inchangé) ===
        QWidget *actionWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(4, 2, 4, 2);
        layout->setSpacing(10);

        QPushButton *modifyBtn = new QPushButton("✏️ Modify");
        modifyBtn->setFixedSize(120, 60);
        modifyBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; border-radius: 10px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #45a049; }");

        QPushButton *deleteBtn = new QPushButton("🗑️ Delete");
        deleteBtn->setFixedSize(120, 60);
        deleteBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; border-radius: 10px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #da190b; }");

        layout->addWidget(modifyBtn);
        layout->addWidget(deleteBtn);

        modifyBtn->setProperty("userId", query.value(0).toInt());
        deleteBtn->setProperty("userId", query.value(0).toInt());

        connect(modifyBtn, &QPushButton::clicked, this, &Users::onModifyUser);
        connect(deleteBtn, &QPushButton::clicked, this, &Users::onDeleteUser);

        ui->usersTable->setCellWidget(row, 7, actionWidget);
        row++;
    }
}

void Users::searchUsers()
{
    QString searchText = ui->searchInputUser->text().trimmed();
    QString searchType = ui->searchTypeComboUser->currentText();

    loadUserToTable(searchText, searchType);
}

// Méthodes demandées dans users.h
void Users::showAddUserForm() { ui->usersTabWidget->setCurrentIndex(1); }
void Users::hideAddUserForm() { ui->usersTabWidget->setCurrentIndex(0); }

void Users::setupMenus()
{
    // Sort menu (existing)
    QMenu *sortMenu = new QMenu(ui->sortButtonUser);
    // ... your existing style and actions for sortMenu ...

    sortMenu->addAction("Par ID");
    sortMenu->addAction("Par Nom");
    sortMenu->addAction("Par Prénom");
    sortMenu->addAction("Par Email");
    sortMenu->addAction("Par Role");

    ui->sortButtonUser->setMenu(sortMenu);

    // Export menu (existing)
    QMenu *exportMenu = new QMenu(ui->exportButtonUser);
    // ... your existing style ...

    exportMenu->addAction("Exporter en PDF");
    ui->exportButtonUser->setMenu(exportMenu);

    // Stats menu - Simple button instead of menu if there's only one option
    connect(ui->statsButtonUser, &QToolButton::clicked, this, &Users::statsUsers);

    // Connect menus
    connect(sortMenu, &QMenu::triggered, this, &Users::sortUsers);
    connect(exportMenu, &QMenu::triggered, this, &Users::exportUsers);
}

void Users::statsUsers()
{
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Utilisateurs");
    statsDialog->resize(950, 720);
    statsDialog->setStyleSheet("QDialog { background-color: #F8FAFC; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    QLabel *titleLabel = new QLabel("📊 Statistiques des Utilisateurs par Rôle");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B5CF6; padding: 10px;");
    mainLayout->addWidget(titleLabel);

    QSqlQuery query;
    query.prepare("SELECT ROLE, COUNT(*) as count FROM USERS GROUP BY ROLE ORDER BY count DESC");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les statistiques :\n" + query.lastError().text());
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
        QLabel *noData = new QLabel("Aucune donnée utilisateur disponible pour le moment.");
        noData->setAlignment(Qt::AlignCenter);
        noData->setStyleSheet("font-size: 18px; color: #64748B; padding: 80px;");
        mainLayout->addWidget(noData);
    }
    else {
        // === CORRECTION 1 : Leak de 'series' supprimé ===
        // Création du series UNIQUEMENT dans le bloc else + parentage correct
        QPieSeries *series = new QPieSeries();

        // === CORRECTION 2 : QColor avec valeurs RGB (plus rapide) ===
        QList<QColor> colors = {
            QColor(139, 92, 246),   // #8B5CF6
            QColor(59, 130, 246),   // #3B82F6
            QColor(16, 185, 129),   // #10B981
            QColor(245, 158, 11),   // #F59E0B
            QColor(239, 68, 68),    // #EF4444
            QColor(236, 72, 153),   // #EC4899
            QColor(99, 102, 241),   // #6366F1
            QColor(20, 184, 166)    // #14B8A6
        };

        int colorIndex = 0;

        for (const auto &item : data) {
            QPieSlice *slice = series->append(item.first, item.second);
            slice->setLabelVisible(true);

            // === CORRECTION 3 : multi-arg .arg() au lieu du chaînage ===
            double percentage = 100.0 * item.second / totalUsers;
            slice->setLabel(QString("%1: %2 (%3%)")
                                .arg(item.first)
                                .arg(item.second)
                                .arg(percentage, 0, 'f', 1));

            slice->setBrush(colors[colorIndex % colors.size()]);
            colorIndex++;

            slice->setExplodeDistanceFactor(0.05);
            connect(slice, &QPieSlice::hovered, [slice](bool show) {
                slice->setExploded(show);
            });
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        series->setParent(chart);          // Propriété transférée → plus de leak

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

    // Close button
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet("QPushButton { background-color: #8B5CF6; color: white; "
                            "border: none; border-radius: 8px; padding: 12px 40px; "
                            "font-size: 15px; font-weight: bold; } "
                            "QPushButton:hover { background-color: #7C3AED; }");

    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);
    connect(closeBtn, &QPushButton::clicked, statsDialog, &QDialog::accept);

    statsDialog->exec();
    delete statsDialog;   // Nettoyage complet (chart + series via parentage Qt)
}

void Users::sortUsers(QAction *action)
{
    if (!action) return;

    QString sortColumn;
    QString text = action->text();

    if (text == "Par ID")          sortColumn = "ID_USER";
    else if (text == "Par Nom")    sortColumn = "NOM";
    else if (text == "Par Prénom") sortColumn = "PRENOM";
    else if (text == "Par Email")  sortColumn = "EMAIL";
    else if (text == "Par Role")   sortColumn = "ROLE";
    else return;

    // On garde le filtre de recherche actif (s'il y en a un)
    QString searchText = ui->searchInputUser->text().trimmed();
    QString searchType = ui->searchTypeComboUser->currentText();

    loadUserToTable(searchText, searchType, sortColumn);
}
void Users::exportUsers()
{
    // 1. Choix du fichier
    QString defaultPath = QDir::homePath() + "/Liste_Utilisateurs_"
                          + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter la liste des utilisateurs en PDF",
                                                    defaultPath,
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // 2. Récupération des données
    QSqlQuery query;
    query.prepare("SELECT ID_USER, NOM, PRENOM, EMAIL, MDP, PHOTO, ROLE "
                  "FROM USERS ORDER BY ID_USER ASC");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur Export",
                              "Impossible de charger les données :\n" + query.lastError().text());
        return;
    }

    QList<QStringList> usersData;
    while (query.next()) {
        QStringList row;
        for (int i = 0; i < 7; ++i)
            row << query.value(i).toString();
        usersData << row;
    }

    if (usersData.isEmpty()) {
        QMessageBox::information(this, "Export PDF", "Aucun utilisateur à exporter.");
        return;
    }

    // ==================== CRÉATION DU PDF ====================
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);

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
    QColor lightRow(248, 250, 252);   // fond très clair pour alternance

    // ==================== TITRE ====================
    painter.setPen(primary);
    painter.setFont(QFont("Segoe UI", 28, QFont::Bold));
    painter.drawText(0, 80, pageWidth, 60, Qt::AlignCenter, "Liste des Utilisateurs");

    painter.setPen(textColor);
    painter.setFont(QFont("Segoe UI", 12));
    QString dateStr = QDate::currentDate().toString("dddd dd MMMM yyyy");
    painter.drawText(0, 140, pageWidth, 30, Qt::AlignCenter,
                     QString("Export complet • Base Oracle • %1").arg(dateStr));

    // ==================== TABLEAU ====================
    int marginLeft = 40;
    int tableWidth = pageWidth - 80;
    int rowHeight  = 42;

    int colWidths[7] = { 55, 95, 95, 170, 85, 110, 90 };
    int colPos[8] = { marginLeft };
    for (int i = 0; i < 7; ++i)
        colPos[i + 1] = colPos[i] + colWidths[i];

    int y = 200;

    QStringList headers = {"ID User", "Nom", "Prénom", "Email", "Mot de passe", "Photo", "Rôle"};

    // En-tête
    painter.setFont(QFont("Segoe UI", 13, QFont::Bold));
    painter.setPen(textColor);
    for (int i = 0; i < 7; ++i) {
        QRect rect(colPos[i], y, colWidths[i], rowHeight);
        painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
    }

    painter.setPen(QPen(primary, 4));
    painter.drawLine(colPos[0], y + rowHeight + 4, colPos[7], y + rowHeight + 4);

    y += rowHeight + 15;

    // Données avec correction de couleur + alternance de lignes
    painter.setFont(QFont("Segoe UI", 10));

    for (int rowIndex = 0; rowIndex < usersData.size(); ++rowIndex) {
        const QStringList& rowData = usersData[rowIndex];

        // Nouvelle page si nécessaire
        if (y + rowHeight > pageHeight - 80) {
            if (!printer.newPage()) break;
            y = 80;
        }

        // Alternance de couleur de fond (très léger)
        if (rowIndex % 2 == 1) {
            painter.fillRect(marginLeft, y, tableWidth, rowHeight, lightRow);
        }

        // IMPORTANT : on remet la couleur du texte à chaque ligne
        painter.setPen(textColor);

        // Texte des colonnes
        for (int i = 0; i < 7; ++i) {
            QString txt = rowData[i];
            if (i == 5) {  // Photo
                txt = QFileInfo(txt).fileName();
                if (txt.isEmpty()) txt = "—";
            }

            QRect rect(colPos[i], y, colWidths[i], rowHeight);
            painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, txt);
        }

        // Lignes du tableau
        painter.setPen(QPen(gridColor, 1));
        painter.drawLine(colPos[0], y + rowHeight, colPos[7], y + rowHeight);   // horizontale

        for (int i = 0; i <= 7; ++i) {
            painter.drawLine(colPos[i], y, colPos[i], y + rowHeight);          // verticales
        }

        y += rowHeight;
    }

    painter.end();

    // ==================== MESSAGE DE SUCCÈS ====================
    QFileInfo fi(fileName);
    QString msg = QString(
                      "✅ PDF créé avec succès !\n\n"
                      "Nom du fichier : %1\n"
                      "Chemin : %2\n\n"
                      "Utilisateurs exportés : %3")
                      .arg(fi.fileName())
                      .arg(fi.absolutePath())
                      .arg(usersData.size());

    QMessageBox::information(this, "Export PDF Terminé", msg);

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void Users::on_btnTogglePassword_clicked()
{
    // Check the current echo mode of the password input [cite: 200]
    if (ui->mdpInput->echoMode() == QLineEdit::Password) {
        // Show the actual text
        ui->mdpInput->setEchoMode(QLineEdit::Normal);
        // Optional: Update the icon/text to indicate "hide"
        ui->btnTogglePassword->setText("👁️‍🗨️");
    } else {
        // Hide the text with dots again [cite: 200]
        ui->mdpInput->setEchoMode(QLineEdit::Password);
        ui->btnTogglePassword->setText("👁️");
    }
}
