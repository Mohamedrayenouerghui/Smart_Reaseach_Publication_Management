#include "conference.h"
#include "ui_conference.h"
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDate>
#include <QTableWidgetItem>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

Conference::Conference(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Conference)
    , selectedRowId(-1)
{
    ui->setupUi(this);
    
    // Initialiser le gestionnaire de conférences
    conferenceManager = new ConferenceManager(this);
    
    // Connexion à la base de données SQLite
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/research_db.sqlite";
    
    if (!conferenceManager->connectToSqlite(dbPath)) {
        QMessageBox::warning(this, "Avertissement", 
            "Impossible de se connecter à la base de données: " + conferenceManager->getLastError());
    }
    
    setupTable();
    setupMenus();
    setupConnections();
    loadConferencesToTable();
}

Conference::~Conference() {
    delete ui;
}

void Conference::setupTable() {
    // Configurer les en-têtes du tableau
    ui->conferenceTable->setColumnCount(8);
    ui->conferenceTable->setHorizontalHeaderLabels({
        "ID", "Nom", "Lieu", "Date Début", "Date Fin", "Frais", "État", "Thème"
    });

    // Définir les largeurs des colonnes
    ui->conferenceTable->setColumnWidth(0, 60);     // ID
    ui->conferenceTable->setColumnWidth(1, 150);    // Nom
    ui->conferenceTable->setColumnWidth(2, 150);    // Lieu
    ui->conferenceTable->setColumnWidth(3, 120);    // Date_début
    ui->conferenceTable->setColumnWidth(4, 120);    // Date_fin
    ui->conferenceTable->setColumnWidth(5, 100);    // Frais_inscription
    ui->conferenceTable->setColumnWidth(6, 100);    // État
    ui->conferenceTable->setColumnWidth(7, 120);    // Thème

    // Définir la hauteur des lignes
    ui->conferenceTable->verticalHeader()->setDefaultSectionSize(50);
    
    // Permettre la double-cliquez pour éditer
    connect(ui->conferenceTable, &QTableWidget::cellDoubleClicked, 
            this, &Conference::onTableRowDoubleClicked);
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
    connect(ui->submitButton, &QPushButton::clicked, this, &Conference::onSubmitButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &Conference::onCancelButtonClicked);
    
    // Ajouter les boutons de recherche et suppression
    if (ui->searchButtonConference) {
        connect(ui->searchButtonConference, &QToolButton::clicked, this, &Conference::onSearchButtonClicked);
    }
}

void Conference::onAddButtonClicked() {
    clearFormFields();
    selectedRowId = -1;
    currentEditingConference = ConferenceData();
    ui->conferenceTabWidget->setCurrentIndex(1);
}

void Conference::onStatsButtonClicked() {
    if (!conferenceManager->isConnected()) {
        QMessageBox::warning(this, "Erreur", "Non connecté à la base de données");
        return;
    }

    int count = conferenceManager->getConferenceCount();
    double avgFees = conferenceManager->getAverageFees();
    
    QString stats = QString("Nombre total de conférences: %1\n"
                           "Frais d'inscription moyen: %2 DH")
                           .arg(count)
                           .arg(avgFees, 0, 'f', 2);
    
    QMessageBox::information(this, "Statistiques", stats);
}

void Conference::onClearButtonClicked() {
    ui->searchInputConference->clear();
}

void Conference::onSortMenuTriggered(QAction *action) {
    if (!conferenceManager->isConnected()) {
        QMessageBox::warning(this, "Erreur", "Non connecté à la base de données");
        return;
    }

    QList<ConferenceData> conferences;
    
    if (action->text() == "Par Nom") {
        conferences = conferenceManager->sortConferencesByName();
    } else if (action->text() == "Par Date") {
        conferences = conferenceManager->sortConferencesByDate();
    } else if (action->text() == "Par État") {
        conferences = conferenceManager->sortConferencesByState();
    }

    ui->conferenceTable->setRowCount(conferences.count());
    
    for (int i = 0; i < conferences.count(); ++i) {
        const auto &conf = conferences[i];
        ui->conferenceTable->setItem(i, 0, new QTableWidgetItem(QString::number(conf.id)));
        ui->conferenceTable->setItem(i, 1, new QTableWidgetItem(conf.nom));
        ui->conferenceTable->setItem(i, 2, new QTableWidgetItem(conf.lieu));
        ui->conferenceTable->setItem(i, 3, new QTableWidgetItem(conf.dateDebut.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 4, new QTableWidgetItem(conf.dateFin.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 5, new QTableWidgetItem(QString::number(conf.fraisInscri, 'f', 2)));
        ui->conferenceTable->setItem(i, 6, new QTableWidgetItem(conf.etat));
        ui->conferenceTable->setItem(i, 7, new QTableWidgetItem(conf.theme));
    }
}

void Conference::onExportMenuTriggered(QAction *action) {
    QMessageBox::information(this, "Exporter", action->text() + " à implémenter");
}

void Conference::onSubmitButtonClicked() {
    if (!conferenceManager->isConnected()) {
        QMessageBox::warning(this, "Erreur", "Non connecté à la base de données");
        return;
    }

    ConferenceData conference;
    conference.nom = ui->nomInput->text();
    conference.lieu = ui->lieuInput->text();
    conference.theme = ui->themeInput->text();
    conference.fraisInscri = ui->fraisInput->value();
    conference.etat = ui->etatCombo->currentText();
    conference.dateDebut = ui->dateDebutInput->date();
    conference.dateFin = ui->dateFinInput->date();
    conference.idArticle = 0;

    // Validation
    if (conference.nom.isEmpty() || conference.lieu.isEmpty() || conference.theme.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Veuillez remplir tous les champs obligatoires");
        return;
    }

    bool success = false;
    if (selectedRowId == -1) {
        // Ajouter nouvelle conférence
        success = conferenceManager->addConference(conference);
        if (success) {
            QMessageBox::information(this, "Succès", "Conférence ajoutée avec succès!");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + conferenceManager->getLastError());
        }
    } else {
        // Mettre à jour la conférence
        conference.id = selectedRowId;
        success = conferenceManager->updateConference(conference);
        if (success) {
            QMessageBox::information(this, "Succès", "Conférence mise à jour avec succès!");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour: " + conferenceManager->getLastError());
        }
    }

    if (success) {
        onCancelButtonClicked();
        refreshTable();
    }
}

void Conference::onCancelButtonClicked() {
    clearFormFields();
    ui->conferenceTabWidget->setCurrentIndex(0);
    selectedRowId = -1;
}

void Conference::onSearchButtonClicked() {
    if (!conferenceManager->isConnected()) {
        QMessageBox::warning(this, "Erreur", "Non connecté à la base de données");
        return;
    }

    QString searchText = ui->searchInputConference->text();
    QList<ConferenceData> results = conferenceManager->searchConferencesByName(searchText);

    ui->conferenceTable->setRowCount(results.count());
    
    for (int i = 0; i < results.count(); ++i) {
        const auto &conf = results[i];
        ui->conferenceTable->setItem(i, 0, new QTableWidgetItem(QString::number(conf.id)));
        ui->conferenceTable->setItem(i, 1, new QTableWidgetItem(conf.nom));
        ui->conferenceTable->setItem(i, 2, new QTableWidgetItem(conf.lieu));
        ui->conferenceTable->setItem(i, 3, new QTableWidgetItem(conf.dateDebut.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 4, new QTableWidgetItem(conf.dateFin.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 5, new QTableWidgetItem(QString::number(conf.fraisInscri, 'f', 2)));
        ui->conferenceTable->setItem(i, 6, new QTableWidgetItem(conf.etat));
        ui->conferenceTable->setItem(i, 7, new QTableWidgetItem(conf.theme));
    }

    if (results.isEmpty()) {
        QMessageBox::information(this, "Résultat", "Aucune conférence trouvée");
    }
}

void Conference::onTableRowDoubleClicked(int row, int column) {
    Q_UNUSED(column);
    
    if (row < 0 || row >= ui->conferenceTable->rowCount()) {
        return;
    }

    QTableWidgetItem *idItem = ui->conferenceTable->item(row, 0);
    if (!idItem) return;

    int conferenceId = idItem->text().toInt();
    ConferenceData conference = conferenceManager->getConferenceById(conferenceId);

    if (conference.id != -1) {
        displayConferenceInForm(conference);
        selectedRowId = conferenceId;
        ui->conferenceTabWidget->setCurrentIndex(1);
    }
}

void Conference::onDeleteButtonClicked() {
    if (selectedRowId == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une conférence à supprimer");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
        "Êtes-vous sûr de vouloir supprimer cette conférence?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (conferenceManager->deleteConference(selectedRowId)) {
            QMessageBox::information(this, "Succès", "Conférence supprimée avec succès!");
            refreshTable();
            onCancelButtonClicked();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + conferenceManager->getLastError());
        }
    }
}

void Conference::onEditButtonClicked() {
    if (selectedRowId == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une conférence à éditer");
        return;
    }

    ui->conferenceTabWidget->setCurrentIndex(1);
}

void Conference::loadConferencesToTable() {
    if (!conferenceManager->isConnected()) {
        QMessageBox::warning(this, "Erreur", "Non connecté à la base de données");
        return;
    }

    QList<ConferenceData> conferences = conferenceManager->getAllConferences();
    
    ui->conferenceTable->setRowCount(conferences.count());
    
    for (int i = 0; i < conferences.count(); ++i) {
        const auto &conf = conferences[i];
        ui->conferenceTable->setItem(i, 0, new QTableWidgetItem(QString::number(conf.id)));
        ui->conferenceTable->setItem(i, 1, new QTableWidgetItem(conf.nom));
        ui->conferenceTable->setItem(i, 2, new QTableWidgetItem(conf.lieu));
        ui->conferenceTable->setItem(i, 3, new QTableWidgetItem(conf.dateDebut.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 4, new QTableWidgetItem(conf.dateFin.toString("dd/MM/yyyy")));
        ui->conferenceTable->setItem(i, 5, new QTableWidgetItem(QString::number(conf.fraisInscri, 'f', 2)));
        ui->conferenceTable->setItem(i, 6, new QTableWidgetItem(conf.etat));
        ui->conferenceTable->setItem(i, 7, new QTableWidgetItem(conf.theme));
    }
}

void Conference::refreshTable() {
    loadConferencesToTable();
}

void Conference::clearFormFields() {
    ui->nomInput->clear();
    ui->lieuInput->clear();
    ui->themeInput->clear();
    ui->fraisInput->setValue(0);
    ui->etatCombo->setCurrentIndex(0);
    ui->dateDebutInput->setDate(QDate::currentDate());
    ui->dateFinInput->setDate(QDate::currentDate());
}

void Conference::displayConferenceInForm(const ConferenceData &conference) {
    ui->nomInput->setText(conference.nom);
    ui->lieuInput->setText(conference.lieu);
    ui->themeInput->setText(conference.theme);
    ui->fraisInput->setValue(conference.fraisInscri);
    ui->etatCombo->setCurrentText(conference.etat);
    ui->dateDebutInput->setDate(conference.dateDebut);
    ui->dateFinInput->setDate(conference.dateFin);
}
