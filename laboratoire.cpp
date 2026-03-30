#include "laboratoire.h"
#include "ui_laboratoire.h"
#include <QMessageBox>
#include <QFileDialog>

Laboratoire::Laboratoire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Laboratoire)
{
    ui->setupUi(this);
    setupConnections();
}

Laboratoire::~Laboratoire()
{
    delete ui;
}

void Laboratoire::setupConnections()
{
    connect(ui->ajouterButton, &QPushButton::clicked, this, &Laboratoire::onAjouterClicked);
    connect(ui->modifierButton, &QPushButton::clicked, this, &Laboratoire::onModifierClicked);
    connect(ui->supprimerButton, &QPushButton::clicked, this, &Laboratoire::onSupprimerClicked);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &Laboratoire::onRechercherClicked);
    connect(ui->trierButton, &QPushButton::clicked, this, &Laboratoire::onTrierClicked);
    connect(ui->exporterPDFButton, &QPushButton::clicked, this, &Laboratoire::onExporterPDFClicked);
    connect(ui->exporterWordButton, &QPushButton::clicked, this, &Laboratoire::onExporterWordClicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &Laboratoire::onStatistiquesClicked);
    connect(ui->clearSearchButton, &QPushButton::clicked, this, &Laboratoire::onClearSearchClicked);
}

void Laboratoire::onAjouterClicked()
{
    // Switch to form tab
    ui->tabWidget->setCurrentIndex(1);
}

void Laboratoire::onModifierClicked()
{
    QMessageBox::information(this, "Modifier", "Fonction de modification en cours de développement");
}

void Laboratoire::onSupprimerClicked()
{
    QMessageBox::information(this, "Supprimer", "Fonction de suppression en cours de développement");
}

void Laboratoire::onRechercherClicked()
{
    QString searchText = ui->searchLineEdit->text();
    QString searchType = ui->searchComboBox->currentText();
    QMessageBox::information(this, "Rechercher", 
        QString("Recherche: %1\nType: %2").arg(searchText, searchType));
}

void Laboratoire::onTrierClicked()
{
    QMessageBox::information(this, "Trier", "Fonction de tri en cours de développement");
}

void Laboratoire::onExporterPDFClicked()
{
    QMessageBox::information(this, "Exporter PDF", "Export PDF en cours de développement");
}

void Laboratoire::onExporterWordClicked()
{
    QMessageBox::information(this, "Exporter Word", "Export Word en cours de développement");
}

void Laboratoire::onStatistiquesClicked()
{
    QMessageBox::information(this, "Statistiques", "Statistiques en cours de développement");
}

void Laboratoire::onClearSearchClicked()
{
    ui->searchLineEdit->clear();
}
