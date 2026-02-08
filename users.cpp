#include "users.h"
#include "ui_users.h"
#include <QMenu>
#include <QMessageBox>

Users::Users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Users)
{
    ui->setupUi(this);
    setupTable();
    setupMenus();
    setupConnections();
}

Users::~Users() {
    delete ui;
}

void Users::setupTable() {

    // Set column widths for the table
    ui->usersTable->setColumnWidth(0, 150);
    ui->usersTable->setColumnWidth(1, 150);
    ui->usersTable->setColumnWidth(2, 150);
    ui->usersTable->setColumnWidth(3, 200);
    ui->usersTable->setColumnWidth(4, 150);
    ui->usersTable->setColumnWidth(5, 150);
    ui->usersTable->setColumnWidth(5, 150);

    // Set row height
    ui->usersTable->verticalHeader()->setDefaultSectionSize(60);
}

void Users::setupMenus() {
    QMenu *exportMenu = new QMenu(this);
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Excel");
    ui->exportButtonUser->setMenu(exportMenu);

    // Export menu actions
    connect(exportMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Exporter", action->text() + " à implémenter");
    });

    QMenu *sortMenu = new QMenu(this);
    sortMenu->addAction("Par Expertise");
    sortMenu->addAction("Par Role");
    ui->sortButtonUser->setMenu(sortMenu);

    // Sort menu actions
    connect(sortMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Trier", "Tri par: " + action->text() + " à implémenter");
    });



}

void Users::setupConnections() {
    connect(ui->addButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Utilisateur", "Ajouter un utilisateur");
    });
    // Statistics button
    connect(ui->statsButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
    });
    connect(ui->clearButtonUser, &QToolButton::clicked, ui->searchInputUser, &QLineEdit::clear);


}
// ===== CONNECT BUTTON SIGNALS =====
/*
// Add button
connect(usersUi.addButtonUser, &QToolButton::clicked, this, [this]() {
    QMessageBox::information(this, "Ajouter Utilisateur", "Formulaire d'ajout d'utilisateur à implémenter");
});

// Statistics button
connect(usersUi.statsButtonUser, &QToolButton::clicked, this, [this]() {
    QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
});

// Clear button - clears the search input
connect(usersUi.clearButtonUser, &QToolButton::clicked, usersUi.searchInputUser, &QLineEdit::clear);

// Sort menu actions
connect(sortMenu, &QMenu::triggered, this, [this](QAction *action) {
    QMessageBox::information(this, "Trier", "Tri par: " + action->text() + " à implémenter");
});

// Export menu actions
connect(exportMenu, &QMenu::triggered, this, [this](QAction *action) {
    QMessageBox::information(this, "Exporter", action->text() + " à implémenter");
});
*/
