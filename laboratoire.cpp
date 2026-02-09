#include "laboratoire.h"
#include "ui_laboratoire.h"
#include <QMenu>
#include <QMessageBox>
#include <QAction>
#include <QHBoxLayout>
#include <QPushButton>

Laboratoire::Laboratoire(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Laboratoire)
{
    ui->setupUi(this);
    setupTable();
    setupMenus();
    setupConnections();
}

Laboratoire::~Laboratoire()
{
    delete ui;
}

void Laboratoire::setupTable()
{
    // Set column widths for the table
    ui->laboratoireTable->setColumnWidth(0, 60);     // ID
    ui->laboratoireTable->setColumnWidth(1, 100);    // Nom
    ui->laboratoireTable->setColumnWidth(2, 90);     // Budget
    ui->laboratoireTable->setColumnWidth(3, 100);    // Date
    ui->laboratoireTable->setColumnWidth(4, 110);    // Budget/Domaine
    ui->laboratoireTable->setColumnWidth(5, 100);    // Lieu
    ui->laboratoireTable->setColumnWidth(6, 100);    // nb_employe
    ui->laboratoireTable->setColumnWidth(7, 100);    // Actions

    // Set row height
    ui->laboratoireTable->verticalHeader()->setDefaultSectionSize(60);
}



void Laboratoire::setupMenus()
{
    // Sort button menu
    QMenu *sortMenu = new QMenu(this);
    // Options principales de tri pour les laboratoires
    sortMenu->addAction("Par Budget");
    sortMenu->addAction("Par Domaine");
    ui->sortButtonLab->setMenu(sortMenu);
    ui->sortButtonLab->setPopupMode(QToolButton::InstantPopup);
    
    connect(sortMenu, &QMenu::triggered, this, &Laboratoire::onSortMenuTriggered);

    // Export button menu
    QMenu *exportMenu = new QMenu(this);
    // Même principe que Publications : export PDF / Word
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Word");
    ui->exportButtonLab->setMenu(exportMenu);
    ui->exportButtonLab->setPopupMode(QToolButton::InstantPopup);
    
    connect(exportMenu, &QMenu::triggered, this, &Laboratoire::onExportMenuTriggered);
}

void Laboratoire::setupConnections()
{
    // Add button
    connect(ui->addButtonLab, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Ajouter un Laboratoire", 
                                "Ajouter un nouveau laboratoire");
    });

    // Search button
    connect(ui->searchButtonLab, &QToolButton::clicked, this, [this]() {
        QString searchText = ui->searchInputLab->text();
        if (searchText.isEmpty()) {
            QMessageBox::warning(this, "Recherche", "Veuillez entrer un terme de recherche");
            return;
        }
        QMessageBox::information(this, "Recherche", 
                                "Recherche pour: " + searchText);
    });

    // Statistics button
    connect(ui->statsButtonLab, &QToolButton::clicked, this, [this]() {
        QMessageBox::information(this, "Statistiques", 
                                "Affichage des statistiques des laboratoires");
    });

    // Clear button
    connect(ui->clearButtonLab, &QToolButton::clicked, this, [this]() {
        ui->searchInputLab->clear();
        ui->searchTypeComboLab->setCurrentIndex(0);
    });
}

void Laboratoire::onSortMenuTriggered(QAction *action)
{
    // Comme dans Users / Publications : simple message pour l’instant
    QMessageBox::information(
        this,
        "Trier",
        "Tri par: " + action->text() + " à implémenter"
    );
}

void Laboratoire::onExportMenuTriggered(QAction *action)
{
    // Export PDF / Word à implémenter
    QMessageBox::information(
        this,
        "Exporter",
        action->text() + " à implémenter"
    );
}

