#include "laboratoire.h"
#include "ui_laboratoire.h"
#include "arduinomonitor.h"  // ← NOUVEAU : Arduino 2
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>

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
    // ← NOUVEAU : Arduino 2 - Moniteur de Température
    connect(ui->temperatureMonitorButton, &QPushButton::clicked, this, &Laboratoire::onTemperatureMonitorClicked);
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

// ═══════════════════════════════════════════════════════════════════════════
//  NOUVEAU : Arduino 2 - Moniteur de Température (COM5)
// ═══════════════════════════════════════════════════════════════════════════
void Laboratoire::onTemperatureMonitorClicked()
{
    qDebug() << "[Laboratoire] Ouverture du Moniteur de Température Arduino";
    
    // Créer le dialog ArduinoMonitor
    QDialog *monitorDialog = new QDialog(this);
    monitorDialog->setWindowTitle("🌡️ Moniteur de Température Arduino");
    monitorDialog->setMinimumSize(900, 650);
    monitorDialog->setStyleSheet("QDialog { background-color: #F5F7FA; }");
    
    // Créer le layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(monitorDialog);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Créer l'instance ArduinoMonitor
    ArduinoMonitor *monitor = new ArduinoMonitor(monitorDialog);
    mainLayout->addWidget(monitor);
    
    // Afficher le dialog en mode modal
    monitorDialog->setModal(true);
    monitorDialog->exec();
    
    // Nettoyer après fermeture
    delete monitorDialog;
    
    qDebug() << "[Laboratoire] Moniteur de Température fermé";
}
