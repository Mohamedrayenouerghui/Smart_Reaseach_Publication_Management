#include "conference.h"
#include "ui_conference.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QTableWidgetItem>

Conference::Conference(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Conference)
{
    ui->setupUi(this);
    setupTable();
    setupConnections();
    loadConferences();
}

Conference::~Conference()
{
    delete ui;
}

// ─────────────────────────── SETUP ───────────────────────────
void Conference::setupTable()
{
    ui->conferencesTable->setColumnCount(8);
    QStringList headers = {"ID", "Nom", "Lieu", "Date Debut", "Date Fin",
                           "Frais (DT)", "Etat", "Theme"};
    ui->conferencesTable->setHorizontalHeaderLabels(headers);
    ui->conferencesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->conferencesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->conferencesTable->verticalHeader()->setVisible(false);
    ui->conferencesTable->horizontalHeader()->setStretchLastSection(true);
}

void Conference::setupConnections()
{
    connect(ui->ajouterListeButton, &QPushButton::clicked, this, &Conference::onAjouterClicked);
    connect(ui->ajouterButton,      &QPushButton::clicked, this, &Conference::onSubmitFormClicked);
    connect(ui->resetFormButton,    &QPushButton::clicked, this, &Conference::onResetFormClicked);
    connect(ui->modifierButton,     &QPushButton::clicked, this, &Conference::onModifierClicked);
    connect(ui->supprimerButton,    &QPushButton::clicked, this, &Conference::onSupprimerClicked);
    connect(ui->rechercherButton,   &QPushButton::clicked, this, &Conference::onRechercherClicked);
    connect(ui->trierButton,        &QPushButton::clicked, this, &Conference::onTrierClicked);
    connect(ui->exporterPDFButton,  &QPushButton::clicked, this, &Conference::onExporterPDFClicked);
    connect(ui->clearSearchButton,  &QPushButton::clicked, this, &Conference::onClearSearchClicked);
}

// ─────────────────────────── LOAD ────────────────────────────
void Conference::loadConferences(const QString &whereClause)
{
    QSqlQuery query(Connection::createInstance().getDb());
    QString sql = "SELECT ID_conference, nom, lieu, Date_debut, Date_fin, "
                  "frais_inscri, etat, theme FROM CONFERENCE";
    if (!whereClause.isEmpty())
        sql += " WHERE " + whereClause;
    sql += " ORDER BY Date_debut DESC";

    if (!query.exec(sql)) {
        qDebug() << "loadConferences error:" << query.lastError().text();
        return;
    }

    ui->conferencesTable->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->conferencesTable->insertRow(row);
        ui->conferencesTable->setItem(row, 0, new QTableWidgetItem(query.value("ID_conference").toString()));
        ui->conferencesTable->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
        ui->conferencesTable->setItem(row, 2, new QTableWidgetItem(query.value("lieu").toString()));
        ui->conferencesTable->setItem(row, 3, new QTableWidgetItem(query.value("Date_debut").toDate().toString("dd/MM/yyyy")));
        ui->conferencesTable->setItem(row, 4, new QTableWidgetItem(query.value("Date_fin").toDate().toString("dd/MM/yyyy")));
        ui->conferencesTable->setItem(row, 5, new QTableWidgetItem(query.value("frais_inscri").toString()));
        ui->conferencesTable->setItem(row, 6, new QTableWidgetItem(query.value("etat").toString()));
        ui->conferencesTable->setItem(row, 7, new QTableWidgetItem(query.value("theme").toString()));

        // Color row based on etat
        QString etat = query.value("etat").toString();
        QColor rowColor;
        if (etat == "Ouverte")       rowColor = QColor("#F0FFF4");
        else if (etat == "Fermee")   rowColor = QColor("#FFF5F5");
        else if (etat == "En cours") rowColor = QColor("#FEFCBF");
        else                         rowColor = QColor("#F7F7F7");

        for (int col = 0; col < 8; col++) {
            if (ui->conferencesTable->item(row, col))
                ui->conferencesTable->item(row, col)->setBackground(rowColor);
        }
        row++;
    }
    ui->conferencesTable->resizeColumnsToContents();
}

// ─────────────────────────── ADD ─────────────────────────────
void Conference::onAjouterClicked()
{
    // Reset form dates to today
    ui->dateDebutEdit->setDate(QDate::currentDate());
    ui->dateFinEdit->setDate(QDate::currentDate().addDays(3));
    ui->tabWidget->setCurrentIndex(1);
}

void Conference::onSubmitFormClicked()
{
    // ── Validation ──────────────────────────────────────────
    QString nom   = ui->nomLineEdit->text().trimmed();
    QString lieu  = ui->lieuLineEdit->text().trimmed();

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Veuillez entrer le nom de la conference.");
        ui->nomLineEdit->setFocus();
        return;
    }
    if (lieu.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Veuillez entrer le lieu de la conference.");
        ui->lieuLineEdit->setFocus();
        return;
    }
    QDate dateDebut = ui->dateDebutEdit->date();
    QDate dateFin   = ui->dateFinEdit->date();
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Validation", "La date de fin doit etre apres la date de debut.");
        return;
    }

    // ── INSERT or UPDATE ─────────────────────────────────────
    QSqlQuery query(Connection::createInstance().getDb());

    if (isEditMode && editingConferenceId > 0) {
        // UPDATE
        query.prepare(
            "UPDATE CONFERENCE SET nom=:nom, lieu=:lieu, "
            "Date_debut=TO_DATE(:deb,'DD/MM/YYYY'), Date_fin=TO_DATE(:fin,'DD/MM/YYYY'), "
            "frais_inscri=:frais, etat=:etat, theme=:theme "
            "WHERE ID_conference=:id"
        );
        query.bindValue(":id", editingConferenceId);
    } else {
        // INSERT
        query.prepare(
            "INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme) "
            "VALUES (:nom, :lieu, TO_DATE(:deb,'DD/MM/YYYY'), TO_DATE(:fin,'DD/MM/YYYY'), "
            "        :frais, :etat, :theme)"
        );
    }
    query.bindValue(":nom",   nom);
    query.bindValue(":lieu",  lieu);
    query.bindValue(":deb",   dateDebut.toString("dd/MM/yyyy"));
    query.bindValue(":fin",   dateFin.toString("dd/MM/yyyy"));
    query.bindValue(":frais", ui->fraisSpinBox->value());
    query.bindValue(":etat",  ui->etatComboBox->currentText());
    query.bindValue(":theme", ui->themeComboBox->currentText());

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur BD",
            "Impossible d'ajouter la conference :\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes",
        isEditMode ? "Conference modifiee avec succes !" : "Conference ajoutee avec succes !");

    // Reset state
    isEditMode = false;
    editingConferenceId = -1;
    ui->ajouterButton->setText("Ajouter la Conference");

    onResetFormClicked();
    ui->tabWidget->setCurrentIndex(0);
    loadConferences();
}

void Conference::onResetFormClicked()
{
    ui->nomLineEdit->clear();
    ui->lieuLineEdit->clear();
    ui->fraisSpinBox->setValue(0);
    ui->etatComboBox->setCurrentIndex(0);
    ui->themeComboBox->setCurrentIndex(0);
    ui->dateDebutEdit->setDate(QDate::currentDate());
    ui->dateFinEdit->setDate(QDate::currentDate().addDays(3));
}

// ─────────────────────────── DELETE ──────────────────────────
void Conference::onSupprimerClicked()
{
    int currentRow = ui->conferencesTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection", "Veuillez selectionner une conference a supprimer.");
        return;
    }

    QString nom = ui->conferencesTable->item(currentRow, 1)->text();
    int id      = ui->conferencesTable->item(currentRow, 0)->text().toInt();

    int rep = QMessageBox::question(this, "Confirmation",
        QString("Supprimer la conference \"%1\" ?").arg(nom),
        QMessageBox::Yes | QMessageBox::No);
    if (rep != QMessageBox::Yes) return;

    QSqlQuery query(Connection::createInstance().getDb());
    query.prepare("DELETE FROM CONFERENCE WHERE ID_conference = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur",
            "Impossible de supprimer :\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes", "Conference supprimee.");
    loadConferences();
}

// ─────────────────────────── MODIFY ──────────────────────────
void Conference::onModifierClicked()
{
    int currentRow = ui->conferencesTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection", "Veuillez selectionner une conference a modifier.");
        return;
    }

    // Pre-fill form with selected row data
    ui->nomLineEdit->setText(ui->conferencesTable->item(currentRow, 1)->text());
    ui->lieuLineEdit->setText(ui->conferencesTable->item(currentRow, 2)->text());
    ui->dateDebutEdit->setDate(QDate::fromString(ui->conferencesTable->item(currentRow, 3)->text(), "dd/MM/yyyy"));
    ui->dateFinEdit->setDate(QDate::fromString(ui->conferencesTable->item(currentRow, 4)->text(), "dd/MM/yyyy"));
    ui->fraisSpinBox->setValue(ui->conferencesTable->item(currentRow, 5)->text().toDouble());

    int etatIdx = ui->etatComboBox->findText(ui->conferencesTable->item(currentRow, 6)->text());
    if (etatIdx >= 0) ui->etatComboBox->setCurrentIndex(etatIdx);

    int themeIdx = ui->themeComboBox->findText(ui->conferencesTable->item(currentRow, 7)->text());
    if (themeIdx >= 0) ui->themeComboBox->setCurrentIndex(themeIdx);

    // Store ID for update
    editingConferenceId = ui->conferencesTable->item(currentRow, 0)->text().toInt();
    isEditMode = true;

    // Change button text
    ui->ajouterButton->setText("Enregistrer les modifications");
    ui->tabWidget->setCurrentIndex(1);
}

// ─────────────────────────── SEARCH ──────────────────────────
void Conference::onRechercherClicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();
    if (searchText.isEmpty()) {
        loadConferences();
        return;
    }

    QString type = ui->searchComboBox->currentText();
    QString column;
    if (type.contains("Nom"))       column = "nom";
    else if (type.contains("Lieu")) column = "lieu";
    else                            column = "theme";

    loadConferences(QString("%1 LIKE '%%2%'").arg(column, searchText));
}

void Conference::onClearSearchClicked()
{
    ui->searchLineEdit->clear();
    loadConferences();
}

// ─────────────────────────── SORT / EXPORT ───────────────────
void Conference::onTrierClicked()
{
    ui->conferencesTable->sortItems(3); // Sort by date debut
}

void Conference::onExporterPDFClicked()
{
    QMessageBox::information(this, "Export PDF", "Fonctionnalite d'export PDF a implementer.");
}
