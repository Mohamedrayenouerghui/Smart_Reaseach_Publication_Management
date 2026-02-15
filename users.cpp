#include "users.h"
#include "ui_users.h"
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>


Users::Users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Users)
{
    ui->setupUi(this);
    setupTable();
    setupMenus();
    setupConnections();
    setupPrototypeUser();
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
    ui->usersTable->setColumnWidth(5, 125);
    ui->usersTable->setColumnWidth(6, 125);
    ui->usersTable->setColumnWidth(7, 160);

    // Set row height
    ui->usersTable->verticalHeader()->setDefaultSectionSize(100);
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
    sortMenu->addAction("Par Role");
    sortMenu->addAction("Par Date d'inscription");
    ui->sortButtonUser->setMenu(sortMenu);

    // Sort menu actions
    connect(sortMenu, &QMenu::triggered, this, [this](QAction *action) {
        QMessageBox::information(this, "Trier", "Tri par: " + action->text() + " à implémenter");
    });



}



void Users::setupConnections() {
    connect(ui->addButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Utilisateur", "Ajouter un utilisateur à implémenter");
    });
    // Statistics button
    connect(ui->statsButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
    });
    connect(ui->clearButtonUser, &QToolButton::clicked, ui->searchInputUser, &QLineEdit::clear);
    //upload user photo
    connect(ui->uploadPhotoButton, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Photo Utilisateur", "Ajouter une photo d'utilisateur à implémenter");
    });
    connect(ui->submitButtonUser, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Utilisateur", "Ajouter un utilisateur à implémenter");
    });


}

void Users::setupPrototypeUser() {
    int row = ui->usersTable->rowCount();
    ui->usersTable->insertRow(row);

    // Populate standard data
    ui->usersTable->setItem(row, 0, new QTableWidgetItem("1"));
    ui->usersTable->setItem(row, 1, new QTableWidgetItem("Azzabi"));
    ui->usersTable->setItem(row, 2, new QTableWidgetItem("Alaedinne"));
    ui->usersTable->setItem(row, 3, new QTableWidgetItem("Ala@example.com"));
    ui->usersTable->setItem(row, 4, new QTableWidgetItem("****"));
    ui->usersTable->setItem(row, 5, new QTableWidgetItem("Admin"));

    // 1. Photo Section (Placeholder)
    ui->usersTable->setItem(row, 6, new QTableWidgetItem("photo.png"));

    // Action Buttons Section (Column 7)
    QWidget* actionWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(actionWidget);

    QPushButton *modifyBtn = new QPushButton("Modify");
    QPushButton *deleteBtn = new QPushButton("Delete");

    // Styling
    modifyBtn->setStyleSheet("background-color: #3B82F6; color: white; border-radius: 4px; padding: 5px;");
    deleteBtn->setStyleSheet("background-color: #EF4444; color: white; border-radius: 4px; padding: 5px;");

    // Connections
    connect(modifyBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Modifier", "Fonctionnalité de modification à implémenter");
    });

    connect(deleteBtn, &QPushButton::clicked, this, [this, row]() {
        // Example: remove the row when delete is clicked
        //ui->usersTable->removeRow(row);
        QMessageBox::critical(this, "Supprimer", "Utilisateur supprimé");
    });

    layout->addWidget(modifyBtn);
    layout->addWidget(deleteBtn);

    layout->setContentsMargins(5, 2, 5, 2);
    layout->setSpacing(10);
    actionWidget->setLayout(layout);

    // Apply to the "Actions" column
    ui->usersTable->setCellWidget(row, 7, actionWidget);
}
