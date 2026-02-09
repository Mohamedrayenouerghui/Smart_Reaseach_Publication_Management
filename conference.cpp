#include "conference.h"
#include "ui_conference.h"
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>

Conference::Conference(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Conference)
{
    ui->setupUi(this);
    setupTable();
    setupMenus();
    setupConnections();
}

Conference::~Conference() {
    delete ui;
}

void Conference::setupTable() {
    // Set column widths for the table
    ui->conferenceTable->setColumnWidth(0, 100);    // ID_conférence
    ui->conferenceTable->setColumnWidth(1, 150);    // Nom
    ui->conferenceTable->setColumnWidth(2, 150);    // Lieu
    ui->conferenceTable->setColumnWidth(3, 120);    // Date_début
    ui->conferenceTable->setColumnWidth(4, 120);    // Date_fin
    ui->conferenceTable->setColumnWidth(5, 130);    // Frais_inscription
    ui->conferenceTable->setColumnWidth(6, 100);    // État
    ui->conferenceTable->setColumnWidth(7, 120);    // Thème

    // Set row height
    ui->conferenceTable->verticalHeader()->setDefaultSectionSize(80);
}

void Conference::setupMenus() {
    // Sort menu
    QMenu *sortMenu = new QMenu(this);
    sortMenu->addAction("Par Nom");
    sortMenu->addAction("Par Date");
    sortMenu->addAction("Par État");
    ui->sortButtonConference->setMenu(sortMenu);

    // Sort menu actions
    connect(sortMenu, &QMenu::triggered, this, &Conference::onSortMenuTriggered);

    // Export menu
    QMenu *exportMenu = new QMenu(this);
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Excel");
    ui->exportButtonConference->setMenu(exportMenu);

    // Export menu actions
    connect(exportMenu, &QMenu::triggered, this, &Conference::onExportMenuTriggered);
}

void Conference::setupConnections() {
    connect(ui->addButtonConference, &QToolButton::clicked, this, &Conference::onAddButtonClicked);
    connect(ui->statsButtonConference, &QToolButton::clicked, this, &Conference::onStatsButtonClicked);
    connect(ui->clearButtonConference, &QToolButton::clicked, ui->searchInputConference, &QLineEdit::clear);
}

void Conference::onAddButtonClicked() {
    QMessageBox::information(this, "Ajouter Conférence", "Fonctionnalité d'ajout de conférence à implémenter");
}

void Conference::onStatsButtonClicked() {
    QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
}

void Conference::onClearButtonClicked() {
    ui->searchInputConference->clear();
}

void Conference::onSortMenuTriggered(QAction *action) {
    QMessageBox::information(this, "Trier", "Tri par: " + action->text() + " à implémenter");
}

void Conference::onExportMenuTriggered(QAction *action) {
    QMessageBox::information(this, "Exporter", action->text() + " à implémenter");
}
