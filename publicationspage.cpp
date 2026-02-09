#include "publicationspage.h"
#include "ui_publicationspage.h"
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

PublicationsPage::PublicationsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PublicationsPage)
{
    ui->setupUi(this);
    setupTable();
    setupMenus();
    setupConnections();
}

PublicationsPage::~PublicationsPage()
{
    delete ui;
}

void PublicationsPage::setupTable()
{
    // Set column widths for the table
    ui->publicationsTable->setColumnWidth(0, 60);
    ui->publicationsTable->setColumnWidth(1, 400);
    ui->publicationsTable->setColumnWidth(2, 120);
    ui->publicationsTable->setColumnWidth(3, 120);
    ui->publicationsTable->setColumnWidth(4, 130);
    ui->publicationsTable->setColumnWidth(5, 150);
    
    // Set row height
    ui->publicationsTable->verticalHeader()->setDefaultSectionSize(60);
}

void PublicationsPage::setupMenus()
{
    // Sort button menu
    QMenu *sortMenu = new QMenu(ui->sortButton);
    sortMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #F3E8FF; "
        "    color: #8B5CF6; "
        "}"
    );
    sortMenu->addAction("Par Date");
    sortMenu->addAction("Par Titre");
    ui->sortButton->setMenu(sortMenu);
    
    // Export button menu
    QMenu *exportMenu = new QMenu(ui->exportButton);
    exportMenu->setStyleSheet(
        "QMenu { "
        "    background-color: white; "
        "    border: 1.5px solid #E2E8F0; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "} "
        "QMenu::item { "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    color: #475569; "
        "    border-radius: 6px; "
        "} "
        "QMenu::item:selected { "
        "    background-color: #FEF3C7; "
        "    color: #F59E0B; "
        "}"
    );
    exportMenu->addAction("Exporter en PDF");
    exportMenu->addAction("Exporter en Word");
    ui->exportButton->setMenu(exportMenu);
    
    // Connect menu actions
    connect(sortMenu, &QMenu::triggered, this, &PublicationsPage::onSortMenuTriggered);
    connect(exportMenu, &QMenu::triggered, this, &PublicationsPage::onExportMenuTriggered);
}

void PublicationsPage::setupConnections()
{
    // Toolbar buttons
    connect(ui->addButton, &QToolButton::clicked, this, &PublicationsPage::onAddButtonClicked);
    connect(ui->statsButton, &QToolButton::clicked, this, &PublicationsPage::onStatsButtonClicked);
    connect(ui->clearButton, &QToolButton::clicked, this, &PublicationsPage::onClearButtonClicked);
    
    // Form buttons
    connect(ui->uploadButton, &QPushButton::clicked, this, &PublicationsPage::onUploadButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &PublicationsPage::onCancelButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &PublicationsPage::onSubmitButtonClicked);
}

void PublicationsPage::onAddButtonClicked()
{
    ui->publicationsTabWidget->setCurrentIndex(1);
}

void PublicationsPage::onStatsButtonClicked()
{
    QMessageBox::information(this, "Statistiques", "Affichage des statistiques à implémenter");
}

void PublicationsPage::onClearButtonClicked()
{
    ui->searchInput->clear();
}

void PublicationsPage::onSortMenuTriggered(QAction *action)
{
    QMessageBox::information(this, "Trier", "Tri par: " + action->text());
}

void PublicationsPage::onExportMenuTriggered(QAction *action)
{
    QMessageBox::information(this, "Exporter", action->text());
}

void PublicationsPage::onUploadButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Sélectionner un fichier PDF", 
        "", 
        "PDF Files (*.pdf)");
    
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        ui->fileNameLabel->setText(fileInfo.fileName());
    }
}

void PublicationsPage::onCancelButtonClicked()
{
    ui->titreInput->clear();
    ui->domaineCombo->setCurrentIndex(0);
    ui->fileNameLabel->setText("Aucun fichier sélectionné");
    ui->publicationsTabWidget->setCurrentIndex(0);
}

void PublicationsPage::onSubmitButtonClicked()
{
    QString titre = ui->titreInput->text().trimmed();
    QString domaine = ui->domaineCombo->currentText();
    QString fileName = ui->fileNameLabel->text();
    
    // Validation
    if (titre.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un titre pour l'article.");
        return;
    }
    
    if (domaine == "Sélectionnez un domaine...") {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un domaine.");
        return;
    }
    
    if (fileName == "Aucun fichier sélectionné") {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un fichier PDF.");
        return;
    }
    
    // Success message
    QMessageBox::information(this, "Succès", 
        "Article ajouté avec succès!\n\n"
        "Titre: " + titre + "\n"
        "Domaine: " + domaine + "\n"
        "Fichier: " + fileName);
    
    // Clear form and switch back to list
    ui->titreInput->clear();
    ui->domaineCombo->setCurrentIndex(0);
    ui->fileNameLabel->setText("Aucun fichier sélectionné");
    ui->publicationsTabWidget->setCurrentIndex(0);
}
