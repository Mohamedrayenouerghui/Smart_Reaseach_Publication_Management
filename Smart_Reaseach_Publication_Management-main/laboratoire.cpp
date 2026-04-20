#include "laboratoire.h"
#include "ui_laboratoire.h"
#include "laboratoire_entity.h"
#include "historique_entity.h"
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QListWidget>
#include <QTextEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

Laboratoire::Laboratoire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Laboratoire)
{
    ui->setupUi(this);
    setupValidationLabels();
    setupConnections();
    setupTableWidget();

    QSqlDatabase &db = Connection::createInstance().getDb();
    if (!db.isOpen()) {
        qWarning() << "Database not connected in Laboratoire module";
        QMessageBox::warning(this, "Avertissement Base de Données",
                             "La base de données n'est pas connectée.\n\n"
                             "Les fonctionnalités CRUD ne seront pas disponibles.\n"
                             "Veuillez vérifier la configuration de la base de données.");
    } else {
        qInfo() << "Laboratoire module: Database connected, loading data...";
        loadAllLaboratoires();
    }
}

Laboratoire::~Laboratoire()
{
    delete ui;
}

void Laboratoire::setupConnections()
{
    connect(ui->ajouterButton,            &QPushButton::clicked, this, &Laboratoire::onAjouterClicked);
    connect(ui->historiqueButton,         &QPushButton::clicked, this, &Laboratoire::onHistoriqueClicked);
    connect(ui->modifierButton,           &QPushButton::clicked, this, &Laboratoire::onModifierClicked);
    connect(ui->supprimerButton,          &QPushButton::clicked, this, &Laboratoire::onSupprimerClicked);
    connect(ui->rechercherButton,         &QPushButton::clicked, this, &Laboratoire::onRechercherClicked);
    connect(ui->trierButton,              &QPushButton::clicked, this, &Laboratoire::onTrierClicked);
    connect(ui->exporterPDFButton,        &QPushButton::clicked, this, &Laboratoire::onExporterPDFClicked);
    connect(ui->notificationButton,       &QPushButton::clicked, this, &Laboratoire::onNotificationClicked);
    connect(ui->statistiquesButton,       &QPushButton::clicked, this, &Laboratoire::onStatistiquesClicked);
    connect(ui->clearSearchButton,        &QPushButton::clicked, this, &Laboratoire::onClearSearchClicked);
    connect(ui->supprimerHistoriqueButton,&QPushButton::clicked, this, &Laboratoire::onSupprimerHistoriqueClicked);
    connect(ui->exporterHistoriqueButton, &QPushButton::clicked, this, &Laboratoire::onExporterHistoriqueClicked);
    connect(ui->validerAjoutButton,       &QPushButton::clicked, this, &Laboratoire::onValiderAjoutClicked);
    connect(ui->annulerButton,            &QPushButton::clicked, this, &Laboratoire::onAnnulerClicked);

    connect(ui->laboratoiresTable, &QTableWidget::itemSelectionChanged,
            this, &Laboratoire::onTableSelectionChanged);

    connect(ui->nomLineEdit,          &QLineEdit::textChanged, this, &Laboratoire::validateNomInput);
    connect(ui->localisationLineEdit, &QLineEdit::textChanged, this, &Laboratoire::validateLocalisationInput);

    populateHistoriqueTable();
}

void Laboratoire::setupTableWidget()
{
    ui->laboratoiresTable->setColumnCount(8);
    ui->laboratoiresTable->setHorizontalHeaderLabels(
        {"ID", "Nom", "Budget", "Date Création", "Domaine", "Localisation", "Chercheurs", "Classement"});
    ui->laboratoiresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->laboratoiresTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->laboratoiresTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->laboratoiresTable->horizontalHeader()->setStretchLastSection(true);
    ui->laboratoiresTable->setAlternatingRowColors(true);
    ui->laboratoiresTable->setColumnHidden(0, true);
}

void Laboratoire::loadAllLaboratoires()
{
    QList<LaboratoireEntity> labos = LaboratoireEntity::getAll();
    displayLaboratoires(labos);
}

void Laboratoire::displayLaboratoires(const QList<LaboratoireEntity> &labos)
{
    ui->laboratoiresTable->setRowCount(0);
    for (const LaboratoireEntity &labo : labos) {
        int row = ui->laboratoiresTable->rowCount();
        ui->laboratoiresTable->insertRow(row);
        ui->laboratoiresTable->setItem(row, 0, new QTableWidgetItem(QString::number(labo.getIdLabo())));
        ui->laboratoiresTable->setItem(row, 1, new QTableWidgetItem(labo.getNom()));
        ui->laboratoiresTable->setItem(row, 2, new QTableWidgetItem(QString::number(labo.getBudget(), 'f', 2) + " €"));
        ui->laboratoiresTable->setItem(row, 3, new QTableWidgetItem(labo.getDateCreation().toString("dd/MM/yyyy")));
        ui->laboratoiresTable->setItem(row, 4, new QTableWidgetItem(labo.getDomaine()));
        ui->laboratoiresTable->setItem(row, 5, new QTableWidgetItem(labo.getLocalisation()));
        ui->laboratoiresTable->setItem(row, 6, new QTableWidgetItem(QString::number(labo.getNombreChercheurs())));
        ui->laboratoiresTable->setItem(row, 7, new QTableWidgetItem(labo.getClassement()));
    }
    ui->laboratoiresTable->resizeColumnsToContents();
    updateStatisticsLabel();
}

void Laboratoire::clearForm()
{
    ui->nomLineEdit->clear();
    ui->budgetSpinBox->setValue(0);
    ui->dateCreationEdit->setDate(QDate::currentDate());
    ui->domaineComboBox->setCurrentIndex(0);
    ui->localisationLineEdit->clear();
    ui->nbEmployesSpinBox->setValue(0);
    ui->classementComboBox->setCurrentIndex(0);
    m_currentLaboId = 0;
}

void Laboratoire::loadLaboIntoForm(int id)
{
    LaboratoireEntity labo = LaboratoireEntity::getById(id);
    if (labo.getIdLabo() == 0) {
        QMessageBox::warning(this, "Erreur", "Laboratoire introuvable");
        return;
    }
    ui->nomLineEdit->setText(labo.getNom());
    ui->budgetSpinBox->setValue(labo.getBudget());
    ui->dateCreationEdit->setDate(labo.getDateCreation());

    int domaineIndex = ui->domaineComboBox->findText(labo.getDomaine());
    if (domaineIndex >= 0) ui->domaineComboBox->setCurrentIndex(domaineIndex);

    ui->localisationLineEdit->setText(labo.getLocalisation());
    ui->nbEmployesSpinBox->setValue(labo.getNombreChercheurs());

    int classementIndex = ui->classementComboBox->findText(labo.getClassement());
    if (classementIndex >= 0) ui->classementComboBox->setCurrentIndex(classementIndex);

    m_currentLaboId = id;
}

void Laboratoire::updateStatisticsLabel()
{
    int total = ui->laboratoiresTable->rowCount();
    qDebug() << QString("Total: %1 laboratoire(s)").arg(total);
}

// ============================================
// CRUD OPERATIONS
// ============================================

void Laboratoire::onAjouterClicked()
{
    clearForm();
    ui->tabWidget->setCurrentIndex(1);
    ui->validerAjoutButton->setText("Ajouter");
}

void Laboratoire::onValiderAjoutClicked()
{
    QString nomText = ui->nomLineEdit->text().trimmed();
    if (nomText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du laboratoire est requis");
        ui->nomLineEdit->setFocus();
        return;
    }

    QString localText = ui->localisationLineEdit->text().trimmed();
    if (localText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "La localisation est requise");
        ui->localisationLineEdit->setFocus();
        return;
    }

    QString domaine = ui->domaineComboBox->currentText().trimmed();
    if (domaine.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le domaine est requis");
        ui->domaineComboBox->setFocus();
        return;
    }

    LaboratoireEntity oldLabo;
    LaboratoireEntity labo;

    if (m_currentLaboId > 0) {
        oldLabo = LaboratoireEntity::getById(m_currentLaboId);
        labo = oldLabo;
        if (labo.getIdLabo() == 0) {
            QMessageBox::critical(this, "Erreur", "Laboratoire introuvable");
            return;
        }
    }

    labo.setNom(ui->nomLineEdit->text().trimmed());
    labo.setBudget(ui->budgetSpinBox->value());
    labo.setDateCreation(ui->dateCreationEdit->date());
    labo.setDomaine(domaine);
    labo.setLocalisation(ui->localisationLineEdit->text().trimmed());
    labo.setNombreChercheurs(ui->nbEmployesSpinBox->value());
    labo.setClassement(ui->classementComboBox->currentText());
    labo.setHistorique("");

    if (!labo.isValid()) {
        QMessageBox::warning(this, "Erreur de validation", labo.getValidationError());
        return;
    }

    QSqlDatabase &db = Connection::createInstance().getDb();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur de connexion", "La base de données n'est pas connectée.");
        return;
    }

    if (labo.save()) {
        QString message = (m_currentLaboId > 0) ? "Laboratoire modifié avec succès!" : "Laboratoire ajouté avec succès!";

        if (m_currentLaboId > 0) {
            QStringList changes;
            if (oldLabo.getNom() != labo.getNom())
                changes << QString("Nom: '%1' → '%2'").arg(oldLabo.getNom(), labo.getNom());
            if (oldLabo.getBudget() != labo.getBudget())
                changes << QString("Budget: %1 € → %2 €").arg(oldLabo.getBudget(), 0, 'f', 2).arg(labo.getBudget(), 0, 'f', 2);
            if (oldLabo.getDateCreation() != labo.getDateCreation())
                changes << QString("Date création: %1 → %2")
                               .arg(oldLabo.getDateCreation().toString("dd/MM/yyyy"))
                               .arg(labo.getDateCreation().toString("dd/MM/yyyy"));
            if (oldLabo.getDomaine() != labo.getDomaine())
                changes << QString("Domaine: '%1' → '%2'").arg(oldLabo.getDomaine(), labo.getDomaine());
            if (oldLabo.getLocalisation() != labo.getLocalisation())
                changes << QString("Localisation: '%1' → '%2'").arg(oldLabo.getLocalisation(), labo.getLocalisation());
            if (oldLabo.getNombreChercheurs() != labo.getNombreChercheurs())
                changes << QString("Nombre chercheurs: %1 → %2").arg(oldLabo.getNombreChercheurs()).arg(labo.getNombreChercheurs());
            if (oldLabo.getClassement() != labo.getClassement())
                changes << QString("Classement: '%1' → '%2'").arg(oldLabo.getClassement(), labo.getClassement());

            QString details = changes.isEmpty() ? "Aucune modification détectée" : changes.join("; ");
            HistoriqueEntity::logModification(labo.getNom(), details, "admin");
        } else {
            HistoriqueEntity::logAjout(labo.getNom(), "admin");
        }

        QMessageBox::information(this, "Succès", message);
        loadAllLaboratoires();
        populateHistoriqueTable();
        ui->tabWidget->setCurrentIndex(0);
        clearForm();
    } else {
        QString errorMsg = "Échec de l'enregistrement du laboratoire";
        errorMsg += "\n\nErreur: " + labo.getValidationError();
        QMessageBox::critical(this, "Erreur", errorMsg);
    }
}

void Laboratoire::onModifierClicked()
{
    int currentRow = ui->laboratoiresTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un laboratoire à modifier");
        return;
    }
    int id = ui->laboratoiresTable->item(currentRow, 0)->text().toInt();
    loadLaboIntoForm(id);
    ui->tabWidget->setCurrentIndex(1);
    ui->validerAjoutButton->setText("Modifier");
}

void Laboratoire::onSupprimerClicked()
{
    int currentRow = ui->laboratoiresTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un laboratoire à supprimer");
        return;
    }

    QString nom = ui->laboratoiresTable->item(currentRow, 1)->text();
    int id      = ui->laboratoiresTable->item(currentRow, 0)->text().toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmer la suppression");
    msgBox.setText(QString("Êtes-vous sûr de vouloir supprimer le laboratoire '%1'?").arg(nom));
    msgBox.setInformativeText("Cette action est irréversible.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::NoButton);

    QPushButton *okButton = new QPushButton("OK", &msgBox);
    okButton->setStyleSheet("QPushButton { background-color:#dc3545; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; }"
                            "QPushButton:hover { background-color:#c82333; }");
    QPushButton *cancelButton = new QPushButton("Annuler", &msgBox);
    cancelButton->setStyleSheet("QPushButton { background-color:#6c757d; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; }"
                                "QPushButton:hover { background-color:#5a6268; }");

    msgBox.addButton(okButton,     QMessageBox::AcceptRole);
    msgBox.addButton(cancelButton, QMessageBox::RejectRole);
    msgBox.setDefaultButton(cancelButton);
    msgBox.exec();

    if (msgBox.clickedButton() == okButton) {
        LaboratoireEntity labo = LaboratoireEntity::getById(id);
        if (labo.remove()) {
            HistoriqueEntity::logSuppression(nom, "admin");
            QMessageBox::information(this, "Succès", "Laboratoire supprimé avec succès");
            loadAllLaboratoires();
            populateHistoriqueTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du laboratoire");
        }
    }
}

// ============================================
// SEARCH AND FILTER
// ============================================

void Laboratoire::onRechercherClicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();
    if (searchText.isEmpty()) {
        loadAllLaboratoires();
        return;
    }

    QString searchType = ui->searchComboBox->currentText();
    QList<LaboratoireEntity> results;

    if (searchType == "Tous les champs" || searchType.isEmpty()) {
        results = LaboratoireEntity::search(searchText);
    } else if (searchType == "Domaine") {
        results = LaboratoireEntity::getByDomaine(searchText);
    } else {
        results = LaboratoireEntity::search(searchText);
    }

    displayLaboratoires(results);
    if (results.isEmpty()) {
        QMessageBox::information(this, "Recherche",
                                 QString("Aucun résultat trouvé pour '%1'").arg(searchText));
    }
}

void Laboratoire::onClearSearchClicked()
{
    ui->searchLineEdit->clear();
    ui->searchComboBox->setCurrentIndex(0);
    loadAllLaboratoires();
}

// ============================================
// SORTING
// ============================================

void Laboratoire::onTrierClicked()
{
    QStringList sortOptions = {
        "Nom (A-Z)", "Nom (Z-A)",
        "Budget (Croissant)", "Budget (Décroissant)",
        "Date création (Plus récent)", "Date création (Plus ancien)",
        "Nombre chercheurs (Croissant)", "Nombre chercheurs (Décroissant)"
    };

    QDialog sortDialog(this);
    sortDialog.setWindowTitle("Trier les laboratoires");
    sortDialog.setModal(true);
    sortDialog.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&sortDialog);
    QLabel *label = new QLabel("Choisir le critère de tri:", &sortDialog);
    label->setStyleSheet("font-weight: bold; font-size: 12px; margin-bottom: 10px;");
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&sortDialog);
    listWidget->addItems(sortOptions);
    listWidget->setCurrentRow(0);
    listWidget->setStyleSheet("QListWidget { border:1px solid #ccc; border-radius:4px; padding:5px; background-color:white; }"
                              "QListWidget::item { padding:8px; border-bottom:1px solid #eee; }"
                              "QListWidget::item:selected { background-color:#007bff; color:white; }");
    layout->addWidget(listWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK", &sortDialog);
    okButton->setStyleSheet("QPushButton { background-color:#007bff; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; min-width:80px; }"
                            "QPushButton:hover { background-color:#0056b3; }");
    QPushButton *cancelButton = new QPushButton("Annuler", &sortDialog);
    cancelButton->setStyleSheet("QPushButton { background-color:#6c757d; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; min-width:80px; }"
                                "QPushButton:hover { background-color:#5a6268; }");
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton,     &QPushButton::clicked, &sortDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &sortDialog, &QDialog::reject);

    if (sortDialog.exec() != QDialog::Accepted) return;

    QString choice = listWidget->currentItem()->text();
    if (choice.isEmpty()) return;

    int column = -1;
    Qt::SortOrder order = Qt::AscendingOrder;

    if (choice.contains("Nom")) {
        column = 1;
        order  = choice.contains("Z-A") ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else if (choice.contains("Budget")) {
        column = 2;
        order  = choice.contains("Décroissant") ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else if (choice.contains("Date")) {
        column = 3;
        order  = choice.contains("ancien") ? Qt::AscendingOrder : Qt::DescendingOrder;
    } else if (choice.contains("chercheurs")) {
        column = 6;
        order  = choice.contains("Décroissant") ? Qt::DescendingOrder : Qt::AscendingOrder;
    }

    if (column >= 0) ui->laboratoiresTable->sortItems(column, order);
}

// ============================================
// STATISTICS
// ============================================

void Laboratoire::onStatistiquesClicked()
{
    QList<LaboratoireEntity> labos = LaboratoireEntity::getAll();
    if (labos.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune donnée disponible");
        return;
    }

    int total = labos.size();
    double totalBudget = 0;
    int totalChercheurs = 0;
    QMap<QString, int> domaineCount;
    QMap<QString, int> classementCount;

    for (const LaboratoireEntity &labo : labos) {
        totalBudget     += labo.getBudget();
        totalChercheurs += labo.getNombreChercheurs();
        domaineCount[labo.getDomaine()]++;
        classementCount[labo.getClassement()]++;
    }

    double avgBudget     = totalBudget / total;
    double avgChercheurs = static_cast<double>(totalChercheurs) / total;

    QString stats = "📊 STATISTIQUES DES LABORATOIRES\n\n";
    stats += QString("Total de laboratoires: %1\n\n").arg(total);
    stats += QString("💰 Budget total: %1 €\n").arg(totalBudget, 0, 'f', 2);
    stats += QString("💰 Budget moyen: %1 €\n\n").arg(avgBudget, 0, 'f', 2);
    stats += QString("👥 Total chercheurs: %1\n").arg(totalChercheurs);
    stats += QString("👥 Moyenne chercheurs: %1\n\n").arg(avgChercheurs, 0, 'f', 1);
    stats += "📚 Répartition par domaine:\n";
    for (auto it = domaineCount.begin(); it != domaineCount.end(); ++it)
        stats += QString("  • %1: %2 (%3%)\n").arg(it.key()).arg(it.value()).arg(it.value() * 100.0 / total, 0, 'f', 1);
    stats += "\n🏆 Répartition par classement:\n";
    for (auto it = classementCount.begin(); it != classementCount.end(); ++it)
        stats += QString("  • %1: %2 (%3%)\n").arg(it.key()).arg(it.value()).arg(it.value() * 100.0 / total, 0, 'f', 1);

    QDialog statsDialog(this);
    statsDialog.setWindowTitle("Statistiques");
    statsDialog.setModal(true);
    statsDialog.resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(&statsDialog);
    QTextEdit *textEdit = new QTextEdit(&statsDialog);
    textEdit->setPlainText(stats);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("QTextEdit { border:1px solid #ccc; border-radius:4px; padding:10px; background-color:#f8f9fa; font-size:12px; }");
    layout->addWidget(textEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK", &statsDialog);
    okButton->setStyleSheet("QPushButton { background-color:#007bff; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; min-width:80px; }"
                            "QPushButton:hover { background-color:#0056b3; }");
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &statsDialog, &QDialog::accept);
    statsDialog.exec();
}

// ============================================
// EXPORT
// ============================================

void Laboratoire::onExporterPDFClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Exporter en PDF",
        "laboratoires_" + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf",
        "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);

    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { color: #2c3e50; text-align: center; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th { background-color: #3498db; color: white; padding: 10px; text-align: left; }"
                   "td { padding: 8px; border-bottom: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   ".footer { margin-top: 20px; text-align: center; font-size: 10px; color: #7f8c8d; }"
                   "</style></head><body>";
    html += "<h1>Liste des Laboratoires</h1>";
    html += "<p>Date d'export: " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table><thead><tr>";
    html += "<th>Nom</th><th>Budget</th><th>Date Création</th><th>Domaine</th>";
    html += "<th>Localisation</th><th>Chercheurs</th><th>Classement</th>";
    html += "</tr></thead><tbody>";

    for (int row = 0; row < ui->laboratoiresTable->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 1; col < ui->laboratoiresTable->columnCount(); ++col) {
            QTableWidgetItem *item = ui->laboratoiresTable->item(row, col);
            html += "<td>" + (item ? item->text() : "") + "</td>";
        }
        html += "</tr>";
    }

    html += "</tbody></table>";
    html += "<div class='footer'>Généré par Smart Research Management System</div>";
    html += "</body></html>";

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Succès",
                             "Les données ont été exportées avec succès vers:\n" + fileName);
}

// ============================================
// OTHER FUNCTIONS
// ============================================

void Laboratoire::onHistoriqueClicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void Laboratoire::onAnnulerClicked()
{
    int currentTab = ui->tabWidget->currentIndex();
    if (currentTab == 1) {
        clearForm();
    }
    // On historique tab, do nothing
}

void Laboratoire::onNotificationClicked()
{
    QList<LaboratoireEntity> labos = LaboratoireEntity::getAll();
    QStringList alerts;
    int alertCount = 0;

    for (const LaboratoireEntity &labo : labos) {
        double budget = labo.getBudget();
        if (budget > 0) {
            double threshold   = 100000.0;
            double usagePercent = (budget / threshold) * 100.0;
            if (usagePercent >= 80.0) {
                alertCount++;
                alerts << QString("⚠️ %1: Budget élevé (%2 €)").arg(labo.getNom()).arg(budget, 0, 'f', 2);
            }
        }
    }

    QString message = "🔔 NOTIFICATIONS - Alertes Budget\n\n";
    if (alertCount > 0) {
        message += QString("⚠️ %1 laboratoire(s) avec budget élevé (≥80%% du seuil):\n\n").arg(alertCount);
        message += alerts.join("\n");
        message += "\n\n💡 Conseil: Vérifiez les dépenses de ces laboratoires.";
    } else {
        message += "✅ Aucune alerte budget\n\nTous les laboratoires sont dans les limites normales.";
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Notifications Budget");
    msgBox.setText(message);
    msgBox.setIcon(alertCount > 0 ? QMessageBox::Warning : QMessageBox::Information);
    msgBox.exec();
}

void Laboratoire::onTableSelectionChanged()
{
    bool hasSelection = ui->laboratoiresTable->currentRow() >= 0;
    ui->modifierButton->setEnabled(hasSelection);
    ui->supprimerButton->setEnabled(hasSelection);
}

void Laboratoire::onSupprimerHistoriqueClicked()
{
    int currentRow = ui->historiqueTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une entrée d'historique à supprimer");
        return;
    }

    QString date    = ui->historiqueTable->item(currentRow, 0)->text();
    QString nomLabo = ui->historiqueTable->item(currentRow, 1)->text();
    QString typeOp  = ui->historiqueTable->item(currentRow, 2)->text();
    int historyId   = ui->historiqueTable->item(currentRow, 0)->data(Qt::UserRole).toInt();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmer la suppression");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer cette entrée d'historique?");
    msgBox.setInformativeText(QString("Date: %1\nLaboratoire: %2\nType: %3\n\nCette action est irréversible.").arg(date, nomLabo, typeOp));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::NoButton);

    QPushButton *okButton = new QPushButton("OK", &msgBox);
    okButton->setStyleSheet("QPushButton { background-color:#dc3545; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; }"
                            "QPushButton:hover { background-color:#c82333; }");
    QPushButton *cancelButton = new QPushButton("Annuler", &msgBox);
    cancelButton->setStyleSheet("QPushButton { background-color:#6c757d; color:white; border:none; padding:8px 16px; border-radius:4px; font-weight:bold; }"
                                "QPushButton:hover { background-color:#5a6268; }");
    msgBox.addButton(okButton,     QMessageBox::AcceptRole);
    msgBox.addButton(cancelButton, QMessageBox::RejectRole);
    msgBox.setDefaultButton(cancelButton);
    msgBox.exec();

    if (msgBox.clickedButton() == okButton) {
        QSqlDatabase &db = Connection::createInstance().getDb();
        QSqlQuery query(db);
        query.prepare("DELETE FROM HISTORIQUE WHERE ID_historique = ?");
        query.addBindValue(historyId);
        if (query.exec()) {
            if (query.numRowsAffected() > 0) {
                QMessageBox::information(this, "Succès", "Entrée d'historique supprimée avec succès");
            } else {
                QMessageBox::warning(this, "Attention", "Aucune entrée n'a été supprimée.");
            }
            populateHistoriqueTable();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  QString("Échec de la suppression\n\nErreur: %1").arg(query.lastError().text()));
        }
    }
}

void Laboratoire::onExporterHistoriqueClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Exporter Historique", "historique_laboratoires.pdf", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QString html = "<html><head><style>"
                   "body { font-family: Arial; }"
                   "h1 { color: #2c3e50; }"
                   "table { width: 100%; border-collapse: collapse; }"
                   "th { background-color: #3498db; color: white; padding: 8px; }"
                   "td { padding: 6px; border-bottom: 1px solid #ddd; }"
                   "</style></head><body>";
    html += "<h1>Historique des Modifications</h1>";
    html += "<table><tr><th>Date</th><th>Laboratoire</th><th>Type</th><th>Utilisateur</th><th>Détails</th></tr>";

    for (int row = 0; row < ui->historiqueTable->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->historiqueTable->columnCount(); ++col) {
            QTableWidgetItem *item = ui->historiqueTable->item(row, col);
            html += "<td>" + (item ? item->text() : "") + "</td>";
        }
        html += "</tr>";
    }
    html += "</table></body></html>";

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Succès", "Historique exporté avec succès");
}

void Laboratoire::populateHistoriqueTable()
{
    ui->historiqueTable->setRowCount(0);

    QList<HistoriqueEntity> allHistorique = HistoriqueEntity::getAll();
    QList<HistoriqueEntity> historiqueData;

    for (const HistoriqueEntity &entry : allHistorique) {
        if (entry.getType() == HistoriqueEntity::MODIFICATION) {
            historiqueData.append(entry);
        }
    }

    ui->historiqueTable->setRowCount(historiqueData.size());

    for (int row = 0; row < historiqueData.size(); ++row) {
        const HistoriqueEntity &entry = historiqueData[row];

        QTableWidgetItem *dateItem = new QTableWidgetItem(entry.getDate().toString("dd/MM/yyyy hh:mm"));
        ui->historiqueTable->setItem(row, 0, dateItem);
        ui->historiqueTable->setItem(row, 1, new QTableWidgetItem(entry.getLaboratoireNom()));
        ui->historiqueTable->setItem(row, 2, new QTableWidgetItem(entry.getTypeString()));
        ui->historiqueTable->setItem(row, 3, new QTableWidgetItem(entry.getUtilisateur()));
        ui->historiqueTable->setItem(row, 4, new QTableWidgetItem(entry.getDetails()));

        dateItem->setData(Qt::UserRole, entry.getId());
    }

    ui->historiqueTable->resizeColumnsToContents();
    qInfo() << "Historique table populated with" << historiqueData.size() << "modification entries";
}

void Laboratoire::setupValidationLabels()
{
    nomWarningLabel = new QLabel("⚠ Le nom ne doit pas contenir de chiffres !", this);
    nomWarningLabel->setStyleSheet("color: red; font-size: 12px;");
    nomWarningLabel->setVisible(false);

    localisationWarningLabel = new QLabel("⚠ La localisation ne doit pas contenir de chiffres !", this);
    localisationWarningLabel->setStyleSheet("color: red; font-size: 12px;");
    localisationWarningLabel->setVisible(false);

    QFormLayout *formLayout = nullptr;
    QList<QFormLayout*> layouts = ui->scrollAreaWidgetContents->findChildren<QFormLayout*>();
    if (!layouts.isEmpty()) formLayout = layouts.first();

    if (formLayout) {
        int nomRow = -1, locRow = -1;
        for (int i = 0; i < formLayout->rowCount(); ++i) {
            QLayoutItem *item = formLayout->itemAt(i, QFormLayout::FieldRole);
            if (item && item->widget() == ui->nomLineEdit)          nomRow = i;
            if (item && item->widget() == ui->localisationLineEdit) locRow = i;
        }
        if (locRow >= 0) formLayout->insertRow(locRow + 1, QString(""), localisationWarningLabel);
        if (nomRow >= 0) formLayout->insertRow(nomRow + 1, QString(""), nomWarningLabel);
    }
}

void Laboratoire::validateNomInput(const QString &text)
{
    bool hasInvalidChars = text.contains(QRegularExpression("[^a-zA-ZÀ-ÿ\\s]"));
    if (hasInvalidChars) {
        QString cleaned = text;
        cleaned.remove(QRegularExpression("[^a-zA-ZÀ-ÿ\\s]"));
        ui->nomLineEdit->blockSignals(true);
        ui->nomLineEdit->setText(cleaned);
        ui->nomLineEdit->blockSignals(false);
        if (nomWarningLabel) nomWarningLabel->setVisible(true);
        ui->nomLineEdit->setStyleSheet("QLineEdit { border: 2px solid #dc3545; padding: 10px 12px; border-radius: 6px; background-color: white; }");
    } else {
        if (nomWarningLabel) nomWarningLabel->setVisible(false);
        ui->nomLineEdit->setStyleSheet("");
    }
}

void Laboratoire::validateLocalisationInput(const QString &text)
{
    bool hasInvalidChars = text.contains(QRegularExpression("[^a-zA-ZÀ-ÿ\\s]"));
    if (hasInvalidChars) {
        QString cleaned = text;
        cleaned.remove(QRegularExpression("[^a-zA-ZÀ-ÿ\\s]"));
        ui->localisationLineEdit->blockSignals(true);
        ui->localisationLineEdit->setText(cleaned);
        ui->localisationLineEdit->blockSignals(false);
        if (localisationWarningLabel) localisationWarningLabel->setVisible(true);
        ui->localisationLineEdit->setStyleSheet("QLineEdit { border: 2px solid #dc3545; padding: 10px 12px; border-radius: 6px; background-color: white; }");
    } else {
        if (localisationWarningLabel) localisationWarningLabel->setVisible(false);
        ui->localisationLineEdit->setStyleSheet("");
    }
}
