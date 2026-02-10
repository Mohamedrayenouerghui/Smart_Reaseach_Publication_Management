#include "conference.h"
#include "ui_conference.h"
#include <QMessageBox>
#include <QFileDialog>

Conference::Conference(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Conference)
{
    ui->setupUi(this);
    setupConnections();
}

Conference::~Conference()
{
    delete ui;
}

void Conference::setupConnections()
{
    connect(ui->ajouterButton, &QPushButton::clicked, this, &Conference::onAjouterClicked);
    connect(ui->modifierButton, &QPushButton::clicked, this, &Conference::onModifierClicked);
    connect(ui->supprimerButton, &QPushButton::clicked, this, &Conference::onSupprimerClicked);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &Conference::onRechercherClicked);
    connect(ui->trierButton, &QPushButton::clicked, this, &Conference::onTrierClicked);
    connect(ui->exporterPDFButton, &QPushButton::clicked, this, &Conference::onExporterPDFClicked);
    connect(ui->exporterWordButton, &QPushButton::clicked, this, &Conference::onExporterWordClicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &Conference::onStatistiquesClicked);
    connect(ui->clearSearchButton, &QPushButton::clicked, this, &Conference::onClearSearchClicked);
}

void Conference::onAjouterClicked()
{
    // Switch to form tab
    ui->tabWidget->setCurrentIndex(1);
}

void Conference::onModifierClicked()
{
    QMessageBox::information(this, "Modifier", "Fonction de modification en cours de développement");
}

void Conference::onSupprimerClicked()
{
    QMessageBox::information(this, "Supprimer", "Fonction de suppression en cours de développement");
}

void Conference::onRechercherClicked()
{
    QString searchText = ui->searchLineEdit->text();
    QString searchType = ui->searchComboBox->currentText();
    QMessageBox::information(this, "Rechercher", 
        QString("Recherche: %1\nType: %2").arg(searchText, searchType));
}

void Conference::onTrierClicked()
{
    QMessageBox::information(this, "Trier", "Fonction de tri en cours de développement");
}

void Conference::onExporterPDFClicked()
{
    QMessageBox::information(this, "Exporter PDF", "Export PDF en cours de développement");
}

void Conference::onExporterWordClicked()
{
    QMessageBox::information(this, "Exporter Word", "Export Word en cours de développement");
}

void Conference::onStatistiquesClicked()
{
    QMessageBox::information(this, "Statistiques", "Statistiques en cours de développement");
}

void Conference::onClearSearchClicked()
{
    ui->searchLineEdit->clear();
}
