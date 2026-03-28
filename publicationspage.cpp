#include "publicationspage.h"
#include "ui_publicationspage.h"
#include "connection.h"
#include "article.h"
#include "statisticspage.h"
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>

PublicationsPage::PublicationsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PublicationsPage)
    , currentEditingArticleId(-1)
    , currentSortOrder("Date_creation DESC")
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
    
    // Load articles from database
    loadArticlesFromDatabase();
}

void PublicationsPage::loadArticlesFromDatabase()
{
    // Clear existing rows
    ui->publicationsTable->setRowCount(0);
    
    // Use default database connection
    QSqlDatabase db = QSqlDatabase::database();
    
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La connexion à la base de données n'est pas établie.");
        return;
    }
    
    // Get search text
    QString searchText = ui->searchInput->text().trimmed();
    
    // Build query with search and sort
    QString queryStr = "SELECT ID_article, titre, domaine, Etat, Date_creation FROM ARTICLE";
    
    if (!searchText.isEmpty()) {
        queryStr += " WHERE LOWER(titre) LIKE :search OR LOWER(domaine) LIKE :search OR LOWER(Etat) LIKE :search";
    }
    
    queryStr += " ORDER BY " + currentSortOrder;
    
    QSqlQuery query(db);
    query.prepare(queryStr);
    
    if (!searchText.isEmpty()) {
        query.bindValue(":search", "%" + searchText.toLower() + "%");
    }
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des articles:\n" + query.lastError().text());
        return;
    }
    
    int row = 0;
    while (query.next()) {
        ui->publicationsTable->insertRow(row);
        
        int idArticle = query.value(0).toInt();
        
        // ID
        ui->publicationsTable->setItem(row, 0, new QTableWidgetItem(QString::number(idArticle)));
        
        // Titre
        ui->publicationsTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        
        // Domaine
        ui->publicationsTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        
        // État
        ui->publicationsTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        
        // Date (format DD/MM/YYYY)
        QDate date = query.value(4).toDate();
        ui->publicationsTable->setItem(row, 4, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
        
        // Actions - Create buttons widget
        QWidget *actionsWidget = new QWidget();
        QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(2, 2, 2, 2);
        actionsLayout->setSpacing(3);
        
        // Modifier button
        QPushButton *modifierBtn = new QPushButton("Modifier");
        modifierBtn->setFixedSize(60, 28);
        modifierBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #3B82F6;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 3px;"
            "    padding: 2px 4px;"
            "    font-size: 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #2563EB;"
            "}"
        );
        modifierBtn->setCursor(Qt::PointingHandCursor);
        modifierBtn->setProperty("articleId", idArticle);
        connect(modifierBtn, &QPushButton::clicked, this, &PublicationsPage::onModifierButtonClicked);
        
        // Supprimer button
        QPushButton *supprimerBtn = new QPushButton("Supprimer");
        supprimerBtn->setFixedSize(65, 28);
        supprimerBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #EF4444;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 3px;"
            "    padding: 2px 4px;"
            "    font-size: 10px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #DC2626;"
            "}"
        );
        supprimerBtn->setCursor(Qt::PointingHandCursor);
        supprimerBtn->setProperty("articleId", idArticle);
        connect(supprimerBtn, &QPushButton::clicked, this, &PublicationsPage::onSupprimerButtonClicked);
        
        actionsLayout->addWidget(modifierBtn);
        actionsLayout->addWidget(supprimerBtn);
        actionsLayout->addStretch();
        
        ui->publicationsTable->setCellWidget(row, 5, actionsWidget);
        
        row++;
    }
    
    // If no articles found
    if (row == 0 && searchText.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun article trouvé dans la base de données.\nInsérez des articles dans Oracle pour les voir ici.");
    }
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
    
    // Search input
    connect(ui->searchInput, &QLineEdit::textChanged, this, &PublicationsPage::onSearchTextChanged);
    
    // Form buttons
    connect(ui->uploadButton, &QPushButton::clicked, this, &PublicationsPage::onUploadButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &PublicationsPage::onCancelButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &PublicationsPage::onSubmitButtonClicked);
    
    // Verification buttons (in add tab)
    connect(ui->spellCheckButton, &QPushButton::clicked, this, &PublicationsPage::onSpellCheckButtonClicked);
    connect(ui->plagiarismButton, &QPushButton::clicked, this, &PublicationsPage::onPlagiarismButtonClicked);
}

void PublicationsPage::onAddButtonClicked()
{
    ui->publicationsTabWidget->setCurrentIndex(1);
}

void PublicationsPage::onStatsButtonClicked()
{
    StatisticsPage *statsDialog = new StatisticsPage(this);
    statsDialog->exec();
    delete statsDialog;
}

void PublicationsPage::onClearButtonClicked()
{
    ui->searchInput->clear();
    loadArticlesFromDatabase();
}

void PublicationsPage::onSortMenuTriggered(QAction *action)
{
    if (action->text() == "Par Date") {
        currentSortOrder = "Date_creation DESC";
    } else if (action->text() == "Par Titre") {
        currentSortOrder = "titre ASC";
    }
    
    loadArticlesFromDatabase();
}

void PublicationsPage::onExportMenuTriggered(QAction *action)
{
    if (action->text() == "Exporter en PDF") {
        exportTableToPDF();
    } else {
        QMessageBox::information(this, "Exporter", "Export Word sera implémenté prochainement.");
    }
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
    
    // Réinitialiser le mode modification
    currentEditingArticleId = -1;
    ui->submitButton->setText("Soumettre");
    
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
    
    Article article;
    article.setTitre(titre);
    article.setContenu("Contenu du fichier: " + fileName);
    article.setDomaine(domaine);
    article.setEtat("Publié");
    article.setDateCreation(QDate::currentDate());
    
    bool success = false;
    QString successMessage;
    
    // Vérifier si on est en mode modification ou ajout
    if (currentEditingArticleId != -1) {
        // Mode modification
        article.setIdArticle(currentEditingArticleId);
        success = article.modifier();
        successMessage = "Article modifié avec succès!";
    } else {
        // Mode ajout
        success = article.ajouter();
        successMessage = "Article ajouté avec succès!";
    }
    
    if (success) {
        QMessageBox::information(this, "Succès", 
            successMessage + "\n\n"
            "Titre: " + titre + "\n"
            "Domaine: " + domaine + "\n"
            "Fichier: " + fileName);
        
        // Clear form
        ui->titreInput->clear();
        ui->domaineCombo->setCurrentIndex(0);
        ui->fileNameLabel->setText("Aucun fichier sélectionné");
        
        // Réinitialiser le mode
        currentEditingArticleId = -1;
        ui->submitButton->setText("Soumettre");
        
        // Recharger la table
        loadArticlesFromDatabase();
        
        // Retourner à l'onglet liste
        ui->publicationsTabWidget->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", 
            "Erreur lors de l'opération sur la base de données.");
    }
}

void PublicationsPage::onSpellCheckButtonClicked()
{
    QMessageBox::information(this, "Vérification Orthographique", 
        "Vérification orthographique et typographique automatique\n\n"
        "Cette fonctionnalité sera implémentée prochainement.");
}

void PublicationsPage::onPlagiarismButtonClicked()
{
    QMessageBox::information(this, "Vérification de Plagiat", 
        "Vérification de similarité (plagiat)\n\n"
        "Cette fonctionnalité sera implémentée prochainement.");
}

void PublicationsPage::onModifierButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int articleId = button->property("articleId").toInt();
    
    // Charger les données de l'article depuis la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "La connexion à la base de données n'est pas établie.");
        return;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT titre, contenu, domaine, Etat FROM ARTICLE WHERE ID_article = :id");
    query.bindValue(":id", articleId);
    
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger l'article.");
        return;
    }
    
    // Remplir le formulaire avec les données de l'article
    ui->titreInput->setText(query.value(0).toString());
    
    // Sélectionner le domaine dans le combo box
    QString domaine = query.value(2).toString();
    int index = ui->domaineCombo->findText(domaine);
    if (index >= 0) {
        ui->domaineCombo->setCurrentIndex(index);
    }
    
    // Afficher le nom du fichier (contenu)
    QString contenu = query.value(1).toString();
    if (contenu.startsWith("Contenu du fichier: ")) {
        ui->fileNameLabel->setText(contenu.mid(20)); // Extraire le nom du fichier
    } else {
        ui->fileNameLabel->setText("article_" + QString::number(articleId) + ".pdf");
    }
    
    // Stocker l'ID de l'article en cours de modification
    currentEditingArticleId = articleId;
    
    // Changer le texte du bouton submit
    ui->submitButton->setText("Modifier");
    
    // Passer à l'onglet d'ajout/modification
    ui->publicationsTabWidget->setCurrentIndex(1);
}

void PublicationsPage::onSupprimerButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int articleId = button->property("articleId").toInt();
    
    // Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", 
        "Êtes-vous sûr de vouloir supprimer cet article?\n\nCette action est irréversible.",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::No) {
        return;
    }
    
    // Créer un objet Article avec l'ID et supprimer
    Article article;
    article.setIdArticle(articleId);
    
    if (article.supprimer()) {
        QMessageBox::information(this, "Succès", "Article supprimé avec succès!");
        
        // Recharger la table
        loadArticlesFromDatabase();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'article.");
    }
}

void PublicationsPage::onSearchTextChanged()
{
    loadArticlesFromDatabase();
}

void PublicationsPage::exportTableToPDF()
{
    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
        "Exporter en PDF",
        QDir::homePath() + "/publications_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf",
        "PDF Files (*.pdf)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Créer le printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
    
    // Créer le document HTML
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { color: #8B5CF6; text-align: center; margin-bottom: 30px; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th { background-color: #8B5CF6; color: white; padding: 12px; text-align: left; font-weight: bold; }"
                   "td { padding: 10px; border-bottom: 1px solid #E2E8F0; }"
                   "tr:nth-child(even) { background-color: #F9FAFB; }"
                   ".footer { text-align: center; margin-top: 30px; font-size: 10px; color: #64748B; }"
                   "</style></head><body>";
    
    html += "<h1>Liste des Publications</h1>";
    html += "<p style='text-align: center; color: #64748B;'>Généré le " + 
            QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm") + "</p>";
    
    html += "<table>";
    html += "<tr><th>ID</th><th>Titre</th><th>Domaine</th><th>État</th><th>Date</th></tr>";
    
    // Parcourir la table et ajouter les données
    for (int row = 0; row < ui->publicationsTable->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < 5; ++col) { // Seulement les 5 premières colonnes (pas Actions)
            QTableWidgetItem *item = ui->publicationsTable->item(row, col);
            if (item) {
                html += "<td>" + item->text() + "</td>";
            } else {
                html += "<td></td>";
            }
        }
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<div class='footer'>Smart Research Management System - " + 
            QString::number(ui->publicationsTable->rowCount()) + " article(s)</div>";
    html += "</body></html>";
    
    // Créer le document et l'imprimer
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
    
    QMessageBox::information(this, "Succès", 
        QString("Le fichier PDF a été exporté avec succès!\n\n"
                "Emplacement: %1\n"
                "Articles exportés: %2")
            .arg(fileName)
            .arg(ui->publicationsTable->rowCount()));
}
