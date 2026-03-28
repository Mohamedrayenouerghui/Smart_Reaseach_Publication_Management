#include "publicationspage.h"
#include "ui_publicationspage.h"
#include "connection.h"
#include "article.h"
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
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QScrollArea>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

PublicationsPage::PublicationsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PublicationsPage)
    , currentEditingArticleId(-1)
    , currentSortOrder("Date_creation DESC")
    , networkManager(new QNetworkAccessManager(this))
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
    // Créer le dialog directement ici
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Publications");
    statsDialog->resize(1200, 900);
    statsDialog->setStyleSheet("QDialog { background-color: #F8FAFC; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    // Titre
    QLabel *titleLabel = new QLabel("Statistiques des Publications");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B5CF6; padding: 15px;");
    mainLayout->addWidget(titleLabel);

    // --- Graphique 1: Par Domaine ---
    QLabel *label1 = new QLabel("Répartition par Domaine");
    label1->setStyleSheet("font-size: 16px; font-weight: bold; color: #1E293B; padding: 5px;");
    mainLayout->addWidget(label1);

    {
        QSqlQuery query;
        query.prepare("SELECT domaine, COUNT(*) FROM ARTICLE GROUP BY domaine ORDER BY COUNT(*) DESC");
        query.exec();

        QPieSeries *series = new QPieSeries();
        QStringList colors = {"#8B5CF6", "#3B82F6", "#10B981", "#F59E0B", "#EF4444", "#EC4899", "#6366F1"};
        int i = 0, total = 0;
        QList<QPair<QString,int>> data;

        while (query.next()) {
            data.append(qMakePair(query.value(0).toString(), query.value(1).toInt()));
            total += query.value(1).toInt();
        }

        for (auto &item : data) {
            QPieSlice *slice = series->append(item.first, item.second);
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1: %2 (%3%)")
                .arg(item.first).arg(item.second)
                .arg(total > 0 ? 100.0 * item.second / total : 0, 0, 'f', 1));
            slice->setBrush(QColor(colors[i % colors.size()]));
            slice->setExplodeDistanceFactor(0.05);
            connect(slice, &QPieSlice::hovered, [slice](bool show){ slice->setExploded(show); });
            i++;
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(QString("Par Domaine (%1 articles)").arg(total));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setAlignment(Qt::AlignRight);
        QFont f; f.setPixelSize(18); f.setBold(true);
        chart->setTitleFont(f);
        chart->setBackgroundBrush(QBrush(Qt::white));

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);
        view->setMinimumHeight(350);
        mainLayout->addWidget(view);
    }

    // --- Graphique 2: Par Date ---
    QLabel *label2 = new QLabel("Répartition par Date de Création");
    label2->setStyleSheet("font-size: 16px; font-weight: bold; color: #1E293B; padding: 5px;");
    mainLayout->addWidget(label2);

    {
        QSqlQuery query;
        query.prepare("SELECT Date_creation FROM ARTICLE ORDER BY Date_creation");
        query.exec();

        QMap<QString, int> periodCount;
        while (query.next()) {
            QDate date = query.value(0).toDate();
            if (date.isValid())
                periodCount[date.toString("MMM yyyy")]++;
        }

        QPieSeries *series = new QPieSeries();
        QStringList colors = {"#8B5CF6", "#3B82F6", "#10B981", "#F59E0B", "#EF4444", "#EC4899", "#14B8A6"};
        int i = 0, total = 0;
        for (auto v : periodCount) total += v;

        for (auto it = periodCount.begin(); it != periodCount.end(); ++it) {
            QPieSlice *slice = series->append(it.key(), it.value());
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1: %2 (%3%)")
                .arg(it.key()).arg(it.value())
                .arg(total > 0 ? 100.0 * it.value() / total : 0, 0, 'f', 1));
            slice->setBrush(QColor(colors[i % colors.size()]));
            slice->setExplodeDistanceFactor(0.05);
            connect(slice, &QPieSlice::hovered, [slice](bool show){ slice->setExploded(show); });
            i++;
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(QString("Par Date de Création (%1 articles)").arg(total));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setAlignment(Qt::AlignRight);
        QFont f; f.setPixelSize(18); f.setBold(true);
        chart->setTitleFont(f);
        chart->setBackgroundBrush(QBrush(Qt::white));

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);
        view->setMinimumHeight(350);
        mainLayout->addWidget(view);
    }

    // Bouton Fermer
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setFixedHeight(40);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: #8B5CF6; color: white; border: none;"
        "border-radius: 6px; padding: 10px 30px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #7C3AED; }");
    connect(closeBtn, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);

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
        "Sélectionner un fichier article", 
        "", 
        "Fichiers texte (*.txt);;Tous les fichiers (*.*)");
    
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        ui->fileNameLabel->setText(fileInfo.fileName());
        // Stocker le chemin complet pour la lecture
        ui->fileNameLabel->setProperty("fullPath", fileName);
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
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un fichier TXT.");
        return;
    }
    
    Article article;
    article.setTitre(titre);
    
    // Lire le vrai contenu du fichier .txt pour le stocker en DB
    QString fullPath = ui->fileNameLabel->property("fullPath").toString();
    QString contenuToStore = fileName; // fallback: juste le nom
    if (!fullPath.isEmpty()) {
        QFile file(fullPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8);
            contenuToStore = stream.readAll().trimmed();
            file.close();
        }
    }
    
    article.setContenu(contenuToStore);
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
    QString titre = ui->titreInput->text().trimmed();
    QString pdfFileName = ui->fileNameLabel->text();
    bool hasFile = (pdfFileName != "Aucun fichier sélectionné" && !pdfFileName.isEmpty());

    QString textToCheck = titre;

    // Lire le fichier .txt directement avec QFile
    if (hasFile) {
        QString fullPath = ui->fileNameLabel->property("fullPath").toString();

        if (fullPath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Chemin du fichier introuvable. Re-uploadez le fichier.");
            return;
        }

        QFileInfo fileInfo(fullPath);
        QString ext = fileInfo.suffix().toLower();

        if (ext == "txt") {
            QFile file(fullPath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier:\n" + fullPath);
                return;
            }
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8);
            QString fileContent = stream.readAll().trimmed();
            file.close();

            if (!fileContent.isEmpty()) {
                // Limiter à 5000 caractères (limite API gratuite)
                if (fileContent.length() > 5000)
                    fileContent = fileContent.left(5000);
                // Combiner titre + contenu fichier
                if (!titre.isEmpty())
                    textToCheck = titre + "\n\n" + fileContent;
                else
                    textToCheck = fileContent;
            }
        } else {
            QMessageBox::information(this, "Format non supporté",
                "Seuls les fichiers .txt sont supportés.\n"
                "La vérification continuera avec le titre uniquement.");
        }
    }

    if (textToCheck.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun texte à vérifier. Entrez un titre ou uploadez un fichier .txt.");
        return;
    }

    // Envoyer à LanguageTool API
    ui->spellCheckButton->setEnabled(false);
    ui->spellCheckButton->setText("Vérification...");

    QUrl url("https://api.languagetool.org/v2/check");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("text", textToCheck);
    params.addQueryItem("language", "fr");
    params.addQueryItem("enabledOnly", "false");

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply *reply = networkManager->post(request, postData);
    reply->setProperty("originalText", textToCheck);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSpellCheckReplyFinished(reply);
    });
}

void PublicationsPage::onSpellCheckReplyFinished(QNetworkReply *reply)
{
    // Réactiver le bouton
    ui->spellCheckButton->setEnabled(true);
    ui->spellCheckButton->setText("Vérif. Orthographe");
    
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Erreur Réseau",
            "Impossible de contacter l'API LanguageTool.\n"
            "Vérifiez votre connexion internet.\n\n"
            "Erreur: " + reply->errorString());
        reply->deleteLater();
        return;
    }
    
    // Parser la réponse JSON
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject root = doc.object();
    QJsonArray matches = root["matches"].toArray();
    
    QString originalText = reply->property("originalText").toString();
    reply->deleteLater();
    
    showSpellCheckResults(matches, originalText);
}

void PublicationsPage::showSpellCheckResults(const QJsonArray &matches, const QString &originalText)
{
    QDialog *d = new QDialog(this);
    d->setWindowTitle("Vérification Orthographique");
    d->resize(620, matches.isEmpty() ? 220 : 480);
    d->setStyleSheet("QDialog { background-color: #FFFFFF; }");

    QVBoxLayout *layout = new QVBoxLayout(d);
    layout->setSpacing(10);
    layout->setContentsMargins(24, 24, 24, 20);

    // --- En-tête ---
    QLabel *title = new QLabel("📝  Vérification Orthographique");
    title->setStyleSheet("font-size: 17px; font-weight: bold; color: #1E293B;");
    layout->addWidget(title);

    // Texte analysé (tronqué si trop long)
    QString preview = originalText.length() > 80
        ? originalText.left(80) + "..." : originalText;
    QLabel *sub = new QLabel("Texte analysé : " + preview);
    sub->setStyleSheet("font-size: 11px; color: #94A3B8;");
    sub->setWordWrap(true);
    layout->addWidget(sub);

    // Séparateur
    QFrame *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("background-color: #E2E8F0; border: none; max-height: 1px;");
    layout->addWidget(sep);

    if (matches.isEmpty()) {
        // --- Aucune erreur ---
        QLabel *ok = new QLabel("✅   Aucune erreur détectée — le texte est correct !");
        ok->setAlignment(Qt::AlignCenter);
        ok->setStyleSheet(
            "font-size: 15px; font-weight: bold; color: #059669;"
            "background: #ECFDF5; border-radius: 10px; padding: 20px;");
        layout->addWidget(ok);

    } else {
        // --- Résumé badge ---
        QLabel *badge = new QLabel(QString("  %1 erreur(s) trouvée(s)").arg(matches.size()));
        badge->setStyleSheet(
            "font-size: 13px; font-weight: bold; color: #B45309;"
            "background: #FEF3C7; border-radius: 6px; padding: 6px 12px;");
        layout->addWidget(badge);

        // --- Zone scrollable ---
        QScrollArea *scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setStyleSheet("background: transparent;");

        QWidget *container = new QWidget();
        container->setStyleSheet("background: transparent;");
        QVBoxLayout *errLayout = new QVBoxLayout(container);
        errLayout->setSpacing(8);
        errLayout->setContentsMargins(0, 4, 0, 4);

        for (int i = 0; i < matches.size(); i++) {
            QJsonObject m = matches[i].toObject();
            QString message  = m["message"].toString();
            QString category = m["rule"].toObject()["category"].toObject()["name"].toString();
            QString ctx      = m["context"].toObject()["text"].toString();
            int offset       = m["context"].toObject()["offset"].toInt();
            int length       = m["context"].toObject()["length"].toInt();

            // Suggestions (max 3)
            QStringList suggs;
            for (auto r : m["replacements"].toArray()) {
                if (suggs.size() >= 3) break;
                suggs << r.toObject()["value"].toString();
            }

            // Carte erreur
            QWidget *card = new QWidget();
            card->setStyleSheet(
                "background: #FFF7F7; border: 1px solid #FECACA;"
                "border-left: 4px solid #EF4444; border-radius: 8px;");
            QVBoxLayout *cl = new QVBoxLayout(card);
            cl->setContentsMargins(12, 10, 12, 10);
            cl->setSpacing(5);

            // Ligne 1: numéro + catégorie
            QLabel *hdr = new QLabel(QString("#%1  ·  %2").arg(i+1).arg(category));
            hdr->setStyleSheet("font-size: 11px; font-weight: bold; color: #EF4444;");
            cl->addWidget(hdr);

            // Ligne 2: message
            QLabel *msg = new QLabel(message);
            msg->setStyleSheet("font-size: 13px; color: #1E293B;");
            msg->setWordWrap(true);
            cl->addWidget(msg);

            // Ligne 3: contexte surligné
            if (!ctx.isEmpty() && offset >= 0 && length > 0 && offset + length <= ctx.length()) {
                QString highlighted = ctx.left(offset).toHtmlEscaped()
                    + "<span style='background:#FEE2E2; color:#DC2626; font-weight:bold;'>"
                    + ctx.mid(offset, length).toHtmlEscaped()
                    + "</span>"
                    + ctx.mid(offset + length).toHtmlEscaped();
                QLabel *ctxLbl = new QLabel("…" + highlighted + "…");
                ctxLbl->setTextFormat(Qt::RichText);
                ctxLbl->setStyleSheet("font-size: 11px; color: #64748B;");
                ctxLbl->setWordWrap(true);
                cl->addWidget(ctxLbl);
            }

            // Ligne 4: suggestions
            if (!suggs.isEmpty()) {
                QLabel *sugLbl = new QLabel("💡 " + suggs.join("   ·   "));
                sugLbl->setStyleSheet(
                    "font-size: 12px; color: #065F46; font-weight: bold;"
                    "background: #D1FAE5; border-radius: 4px; padding: 3px 8px;");
                cl->addWidget(sugLbl);
            }

            errLayout->addWidget(card);
        }

        errLayout->addStretch();
        scroll->setWidget(container);
        layout->addWidget(scroll);
    }

    // --- Bouton Fermer ---
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setFixedSize(110, 36);
    closeBtn->setStyleSheet(
        "QPushButton { background: #8B5CF6; color: white; border: none;"
        "border-radius: 8px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background: #7C3AED; }");
    connect(closeBtn, &QPushButton::clicked, d, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);

    d->exec();
    delete d;
}

// Build n-grams from text (sequences of n consecutive words)
QSet<QString> PublicationsPage::buildNgrams(const QString &text, int n)
{
    QSet<QString> ngrams;
    
    // Nettoyer et tokeniser en mots
    QString cleaned = text.toLower();
    cleaned.remove(QRegularExpression("[^a-zàâäéèêëîïôùûüç\\s]"));
    QStringList words = cleaned.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    
    // Filtrer les mots vides (stop words français)
    QSet<QString> stopWords = {"le","la","les","de","du","des","un","une","et","en",
                                "à","au","aux","ce","se","sa","son","ses","je","tu",
                                "il","elle","nous","vous","ils","elles","que","qui",
                                "ne","pas","plus","par","sur","dans","avec","pour"};
    QStringList filtered;
    for (const QString &w : words)
        if (!stopWords.contains(w) && w.length() > 2)
            filtered << w;
    
    // Construire les n-grams
    for (int i = 0; i <= filtered.size() - n; i++) {
        QString ngram = filtered.mid(i, n).join(" ");
        ngrams.insert(ngram);
    }
    return ngrams;
}

// Jaccard similarity sur les n-grams
double PublicationsPage::ngramSimilarity(const QSet<QString> &a, const QSet<QString> &b)
{
    if (a.isEmpty() && b.isEmpty()) return 0.0;
    QSet<QString> intersection = a & b;
    QSet<QString> unionSet     = a | b;
    return unionSet.isEmpty() ? 0.0 : (double)intersection.size() / unionSet.size();
}

void PublicationsPage::onPlagiarismButtonClicked()
{
    // 1. Récupérer le texte du fichier uploadé
    QString fullPath = ui->fileNameLabel->property("fullPath").toString();
    QString newText;

    if (fullPath.isEmpty() || ui->fileNameLabel->text() == "Aucun fichier sélectionné") {
        // Utiliser le titre si pas de fichier
        newText = ui->titreInput->text().trimmed();
        if (newText.isEmpty()) {
            QMessageBox::warning(this, "Erreur",
                "Veuillez uploader un fichier .txt ou entrer un titre avant de vérifier.");
            return;
        }
    } else {
        QFile file(fullPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erreur", "Impossible de lire le fichier.");
            return;
        }
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        newText = stream.readAll().trimmed();
        file.close();
    }

    if (newText.length() < 30) {
        QMessageBox::warning(this, "Texte trop court",
            "Le texte doit contenir au moins 30 caractères pour une analyse fiable.");
        return;
    }

    // 2. Construire bigrams ET trigrams du nouveau texte (combinaison plus précise)
    QSet<QString> newBigrams  = buildNgrams(newText, 2);
    QSet<QString> newTrigrams = buildNgrams(newText, 3);
    QSet<QString> newNgrams   = newBigrams + newTrigrams;

    // 3. Comparer avec tous les articles de la DB
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "Erreur", "Connexion base de données non établie.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID_article, titre, contenu FROM ARTICLE ORDER BY ID_article");
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la lecture des articles.");
        return;
    }

    // Stocker les résultats
    struct Result { int id; QString titre; double score; };
    QList<Result> results;

    while (query.next()) {
        int    id      = query.value(0).toInt();
        QString titre  = query.value(1).toString();
        QString contenu = query.value(2).toString();

        // Combiner titre + contenu pour la comparaison
        QString dbText = titre + " " + contenu;
        QSet<QString> dbNgrams = buildNgrams(dbText, 2) + buildNgrams(dbText, 3);

        double score = ngramSimilarity(newNgrams, dbNgrams) * 100.0;
        if (score > 0.5) // Ignorer les similarités négligeables
            results.append({id, titre, score});
    }

    // Trier par score décroissant
    std::sort(results.begin(), results.end(),
              [](const Result &a, const Result &b){ return a.score > b.score; });

    // 4. Afficher les résultats
    QDialog *d = new QDialog(this);
    d->setWindowTitle("Vérification de Similarité (Plagiat)");
    d->resize(620, results.isEmpty() ? 220 : 500);
    d->setStyleSheet("QDialog { background: #FFFFFF; }");

    QVBoxLayout *layout = new QVBoxLayout(d);
    layout->setContentsMargins(24, 24, 24, 20);
    layout->setSpacing(10);

    // En-tête
    QLabel *title = new QLabel("🔍  Vérification de Similarité");
    title->setStyleSheet("font-size: 17px; font-weight: bold; color: #1E293B;");
    layout->addWidget(title);

    QString preview = newText.length() > 80 ? newText.left(80) + "..." : newText;
    QLabel *sub = new QLabel("Article analysé : " + preview);
    sub->setStyleSheet("font-size: 11px; color: #94A3B8;");
    sub->setWordWrap(true);
    layout->addWidget(sub);

    QFrame *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("background: #E2E8F0; border: none; max-height: 1px;");
    layout->addWidget(sep);

    // Score global (max similarity trouvée)
    double maxScore = results.isEmpty() ? 0.0 : results.first().score;
    QString verdict;
    QString verdictColor;
    QString verdictBg;

    if (maxScore < 30.0) {
        verdict = "✅  Original — Aucune similarité significative détectée";
        verdictColor = "#065F46"; verdictBg = "#ECFDF5";
    } else if (maxScore < 50.0) {
        verdict = "⚠️  Similarité Faible — Quelques passages similaires";
        verdictColor = "#92400E"; verdictBg = "#FEF3C7";
    } else if (maxScore < 70.0) {
        verdict = "🔶  Similarité Modérée — Vérification recommandée";
        verdictColor = "#9A3412"; verdictBg = "#FFF7ED";
    } else {
        verdict = "🚨  Plagiat Probable — Forte similarité détectée!";
        verdictColor = "#7F1D1D"; verdictBg = "#FEF2F2";
    }

    QLabel *verdictLbl = new QLabel(verdict);
    verdictLbl->setAlignment(Qt::AlignCenter);
    verdictLbl->setStyleSheet(QString(
        "font-size: 14px; font-weight: bold; color: %1;"
        "background: %2; border-radius: 8px; padding: 12px;")
        .arg(verdictColor, verdictBg));
    layout->addWidget(verdictLbl);

    if (!results.isEmpty()) {
        QLabel *listTitle = new QLabel(
            QString("Articles similaires trouvés (%1):").arg(results.size()));
        listTitle->setStyleSheet("font-size: 12px; font-weight: bold; color: #475569;");
        layout->addWidget(listTitle);

        QScrollArea *scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);

        QWidget *container = new QWidget();
        QVBoxLayout *cl = new QVBoxLayout(container);
        cl->setSpacing(6);
        cl->setContentsMargins(0, 0, 0, 0);

        for (const Result &r : results) {
            // Couleur de la barre selon le score
            QString barColor = r.score < 20 ? "#10B981"
                             : r.score < 40 ? "#F59E0B"
                             : r.score < 65 ? "#F97316" : "#EF4444";

            QWidget *card = new QWidget();
            card->setStyleSheet(
                "background: #F8FAFC; border: 1px solid #E2E8F0;"
                "border-left: 4px solid " + barColor + "; border-radius: 6px;");

            QHBoxLayout *hl = new QHBoxLayout(card);
            hl->setContentsMargins(12, 10, 12, 10);

            // Titre de l'article
            QLabel *artTitle = new QLabel(
                QString("ID %1  —  %2").arg(r.id).arg(r.titre));
            artTitle->setStyleSheet("font-size: 13px; color: #1E293B;");
            artTitle->setWordWrap(true);
            hl->addWidget(artTitle, 1);

            // Score badge
            QLabel *scoreLbl = new QLabel(
                QString("%1%").arg(r.score, 0, 'f', 1));
            scoreLbl->setFixedWidth(55);
            scoreLbl->setAlignment(Qt::AlignCenter);
            scoreLbl->setStyleSheet(QString(
                "font-size: 14px; font-weight: bold; color: %1;"
                "background: white; border: 2px solid %1;"
                "border-radius: 6px; padding: 4px;").arg(barColor));
            hl->addWidget(scoreLbl);

            cl->addWidget(card);
        }

        cl->addStretch();
        scroll->setWidget(container);
        layout->addWidget(scroll);
    }

    // Bouton Fermer
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setFixedSize(110, 36);
    closeBtn->setStyleSheet(
        "QPushButton { background: #8B5CF6; color: white; border: none;"
        "border-radius: 8px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background: #7C3AED; }");
    connect(closeBtn, &QPushButton::clicked, d, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);

    d->exec();
    delete d;
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
    
    // Afficher le nom du fichier
    ui->fileNameLabel->setText("article_" + QString::number(articleId) + ".txt");
    
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
