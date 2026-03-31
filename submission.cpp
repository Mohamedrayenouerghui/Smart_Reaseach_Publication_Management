#include "submission.h"
#include "ui_submission.h"
#include "connection.h"
#include <QDate>
#include <QRandomGenerator>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMenu>
#include <QMap>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPrinter>
#include <QTextDocument>
#include <QPageLayout>
#include <QPageSize>
#include <QFile>
#include <QDialog>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QTextStream>
#include <QDir>

Submission::Submission(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Submission)
    , selectedRow(-1)
    , selectedId(-1)
    , isEditMode(false)
{
    ui->setupUi(this);

    // Set current date
    ui->dateEdit->setDate(QDate::currentDate());

    // Charger les relecteurs dans le ComboBox
    loadRelecteurs();

    setupTable();
    setupMenus();
    setupConnections();
    loadSubmissionsFromDB();
}

void Submission::loadRelecteurs()
{
    // Vider le champ relecteur et le transformer en liste déroulante
    // Note: Dans le .ui, reviewerEdit devrait être un QComboBox, mais pour l'instant
    // on va utiliser le bouton "Attribuer automatiquement" pour afficher une liste

    // Cette fonction sera appelée quand on clique sur "Attribuer automatiquement"
}

Submission::~Submission()
{
    delete ui;
}

bool Submission::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->titleEdit && event->type() == QEvent::MouseButtonPress) {
        onSelectArticle();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void Submission::setupTable()
{
    ui->submissionTable->setColumnCount(7);
    ui->submissionTable->setHorizontalHeaderLabels(
        QStringList() << "Article" << "État" << "Décision Finale" << "Date Limite" << "Relecteur" << "Actions"
        );
    ui->submissionTable->setColumnWidth(0, 220); // Article
    ui->submissionTable->setColumnWidth(1, 110); // État
    ui->submissionTable->setColumnWidth(2, 120); // Décision Finale
    ui->submissionTable->setColumnWidth(3, 130); // Date Limite
    ui->submissionTable->setColumnWidth(4, 180); // Relecteur
    ui->submissionTable->setColumnWidth(5, 220); // Actions
    ui->submissionTable->verticalHeader()->setDefaultSectionSize(70);
    ui->submissionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->submissionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Submission::setupMenus()
{
    // Connecter le changement de sélection du ComboBox de tri
    connect(ui->sortTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        // Effectuer le tri automatiquement quand on change le critère
        onSortTriggered();
    });
}

void Submission::onSortTriggered()
{
    QString sortType = ui->sortTypeCombo->currentText();

    if (sortType == "Par Date") {
        // Tri par date (colonne 2 - Date Limite Révision)
        ui->submissionTable->sortItems(2, Qt::DescendingOrder);
    } else if (sortType == "Par Titre") {
        // Tri par titre/article (colonne 0)
        ui->submissionTable->sortItems(0, Qt::AscendingOrder);
    }
}

void Submission::setupConnections()
{
    connect(ui->addBtn, &QPushButton::clicked, this, &Submission::onAddSubmission);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &Submission::onDeleteSubmission);
    connect(ui->clearBtn, &QPushButton::clicked, this, &Submission::clearForm);
    connect(ui->searchBtn, &QPushButton::clicked, this, &Submission::onSearchClicked);
    connect(ui->exportBtn, &QPushButton::clicked, this, &Submission::onExportClicked);
    connect(ui->statsBtn, &QPushButton::clicked, this, &Submission::onStatisticsClicked);
    connect(ui->autoAssignBtn, &QPushButton::clicked, this, &Submission::onChooseReviewer);
    connect(ui->autoAddBtn, &QPushButton::clicked, this, &Submission::onAutoAssignReviewer);
    connect(ui->submissionTable, &QTableWidget::itemSelectionChanged, this, &Submission::onTableSelectionChanged);
    connect(ui->sortBtn, &QPushButton::clicked, this, &Submission::onSortTriggered);

    // Recherche en temps réel au fur et à mesure de la saisie
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &Submission::onSearchTextChanged);

    // Connecter le champ titleEdit pour qu'il ouvre le menu quand on clique dessus
    ui->titleEdit->installEventFilter(this);
}

void Submission::loadSubmissionsFromDB()
{
    ui->submissionTable->setRowCount(0);

    QSqlQuery query;
    bool hasRelecteur = true;
    query.prepare(
        "SELECT s.ID_soumission, a.titre, s.Etat, s.Decision_finale, "
        "       s.Date_lim_revision, s.relecteur "
        "FROM SOUMISSION s "
        "LEFT JOIN ARTICLE a ON a.ID_soumission = s.ID_soumission "
        "ORDER BY s.Date_envoi DESC"
        );
    if (!query.exec()) {
        hasRelecteur = false;
        query.prepare(
            "SELECT s.ID_soumission, a.titre, s.Etat, s.Decision_finale, "
            "       s.Date_lim_revision "
            "FROM SOUMISSION s "
            "LEFT JOIN ARTICLE a ON a.ID_soumission = s.ID_soumission "
            "ORDER BY s.Date_envoi DESC"
            );
        if (!query.exec()) {
            qDebug() << "loadSubmissionsFromDB error:" << query.lastError().text();
            return;
        }
    }

    int row = 0;
    while (query.next()) {
        int     id        = query.value(0).toInt();
        QString titre     = query.value(1).toString();
        QString etat      = query.value(2).toString();
        QString decision  = query.value(3).toString();
        QDate   date      = query.value(4).toDate();
        QString relecteur = hasRelecteur ? query.value(5).toString() : "";

        ui->submissionTable->insertRow(row);
        ui->submissionTable->setItem(row, 0, new QTableWidgetItem(titre));
        ui->submissionTable->setItem(row, 1, new QTableWidgetItem(etat));
        ui->submissionTable->setItem(row, 2, new QTableWidgetItem(decision));
        ui->submissionTable->setItem(row, 3, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
        ui->submissionTable->setItem(row, 4, new QTableWidgetItem(relecteur));
        addButtonsToRow(row, id);
        row++;
    }
}

void Submission::addButtonsToRow(int row, int id)
{
    QWidget* actionWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(actionWidget);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(6);

    // Modifier Décision button
    QPushButton *decisionBtn = new QPushButton("Décision");
    decisionBtn->setStyleSheet(
        "QPushButton { background-color: #8B5CF6; color: white; border-radius: 4px; "
        "padding: 6px 10px; font-size: 12px; font-weight: 600; min-height: 30px; } "
        "QPushButton:hover { background-color: #7C3AED; }"
        );
    decisionBtn->setProperty("row", row);
    decisionBtn->setProperty("id", id);
    connect(decisionBtn, &QPushButton::clicked, this, [this, id, row]() {
        QMenu menu;
        menu.setStyleSheet(
            "QMenu { background: white; border: 1.5px solid #E2E8F0; border-radius: 8px; padding: 6px; }"
            "QMenu::item { padding: 8px 20px; font-size: 13px; color: #475569; border-radius: 6px; }"
            "QMenu::item:selected { background: #F3E8FF; color: #8B5CF6; }"
            );
        menu.addAction("Accepté");
        menu.addAction("Rejeté");
        QAction *chosen = menu.exec(QCursor::pos());
        if (!chosen) return;

        QSqlQuery q;
        q.prepare("UPDATE SOUMISSION SET Decision_finale = :d, Etat = 'Traité' WHERE ID_soumission = :id");
        q.bindValue(":d",  chosen->text());
        q.bindValue(":id", id);
        if (q.exec()) {
            ui->submissionTable->item(row, 1)->setText("Traité");
            ui->submissionTable->item(row, 2)->setText(chosen->text());
        }
    });

    // Modifier button
    QPushButton *modifyBtn = new QPushButton("Modifier");
    modifyBtn->setStyleSheet(
        "QPushButton { background-color: #3B82F6; color: white; border-radius: 4px; "
        "padding: 6px 10px; font-size: 12px; font-weight: 600; min-height: 30px; } "
        "QPushButton:hover { background-color: #2563EB; }"
        );
    modifyBtn->setProperty("row", row);
    modifyBtn->setProperty("id", id);
    connect(modifyBtn, &QPushButton::clicked, this, &Submission::onEditRowClicked);

    // Supprimer button
    QPushButton *deleteBtn = new QPushButton("Supprimer");
    deleteBtn->setStyleSheet(
        "QPushButton { background-color: #EF4444; color: white; border-radius: 4px; "
        "padding: 6px 10px; font-size: 12px; font-weight: 600; min-height: 30px; } "
        "QPushButton:hover { background-color: #DC2626; }"
        );
    deleteBtn->setProperty("row", row);
    deleteBtn->setProperty("id", id);
    connect(deleteBtn, &QPushButton::clicked, this, &Submission::onDeleteRowClicked);

    layout->addWidget(decisionBtn);
    layout->addWidget(modifyBtn);
    layout->addWidget(deleteBtn);
    actionWidget->setLayout(layout);

    ui->submissionTable->setCellWidget(row, 5, actionWidget);
}

void Submission::onAddSubmission()
{
    if (!validateForm()) {
        return;
    }

    // Récupérer les valeurs du formulaire
    QString article = ui->titleEdit->text().trimmed();
    QString decisionFinale = ui->statusCombo->currentText();
    QDate dateLimite = ui->dateEdit->date();
    QString relecteur = ui->reviewerEdit->text().trimmed();

    // Contrôle 1: Vérifier que l'article est sélectionné
    if (article.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Attention");
        msgBox.setText("Veuillez sélectionner un article.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #D97706; }"
            );
        msgBox.exec();
        return;
    }

    // Contrôle 2: Vérifier que le relecteur est sélectionné
    if (relecteur.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Attention");
        msgBox.setText("Veuillez sélectionner un relecteur.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #D97706; }"
            );
        msgBox.exec();
        return;
    }

    // Contrôle 3: Vérifier que la date limite n'est pas déjà passée
    if (dateLimite < QDate::currentDate()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Date Invalide");
        msgBox.setText("❌ La date limite de révision ne peut pas être une date déjà passée.\n\n"
                       "Date sélectionnée: " + dateLimite.toString("dd/MM/yyyy") + "\n"
                                                             "Date actuelle: " + QDate::currentDate().toString("dd/MM/yyyy"));
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 350px; }"
            "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #DC2626; }"
            );
        msgBox.exec();
        return;
    }

    // Contrôle 4: Vérifier qu'il n'existe pas déjà une soumission avec le même article et le même relecteur
    for (int row = 0; row < ui->submissionTable->rowCount(); ++row) {
        QTableWidgetItem *articleItem = ui->submissionTable->item(row, 0);
        QTableWidgetItem *relecteurItem = ui->submissionTable->item(row, 3);

        if (articleItem && relecteurItem) {
            QString existingArticle = articleItem->text();
            QString existingRelecteur = relecteurItem->text();

            if (existingArticle == article && existingRelecteur == relecteur) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle("Soumission Existante");
                msgBox.setText("⚠️ Article déjà envoyé à ce relecteur!\n\n"
                               "Article: " + article + "\n"
                                           "Relecteur: " + relecteur + "\n\n"
                                             "Veuillez choisir un autre relecteur ou un autre article.");
                msgBox.setStyleSheet(
                    "QMessageBox { background-color: white; }"
                    "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 400px; }"
                    "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                    "QPushButton:hover { background-color: #D97706; }"
                    );
                msgBox.exec();
                return;
            }
        }
    }

    // Ajouter la nouvelle soumission dans la base de données
    // 1. Récupérer l'ID de l'article par son titre
    QSqlQuery articleQuery;
    articleQuery.prepare("SELECT ID_article FROM ARTICLE WHERE titre = :titre AND ROWNUM = 1");
    articleQuery.bindValue(":titre", article);
    if (!articleQuery.exec() || !articleQuery.next()) {
        QMessageBox::critical(this, "Erreur", "Article introuvable dans la base de données.");
        return;
    }
    int idArticle = articleQuery.value(0).toInt();

    if (isEditMode) {
        // UPDATE la soumission existante
        QSqlQuery updateQuery;
        updateQuery.prepare(
            "UPDATE SOUMISSION SET Date_lim_revision = :dateLim, Etat = :etat "
            "WHERE ID_soumission = :id"
            );
        updateQuery.bindValue(":dateLim", dateLimite);
        updateQuery.bindValue(":etat",    ui->etatCombo->currentText());
        updateQuery.bindValue(":id",      selectedId);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Erreur DB",
                                  "Erreur lors de la mise à jour:\n" + updateQuery.lastError().text());
            return;
        }

        // Mettre à jour le lien article
        QSqlQuery linkQuery;
        linkQuery.prepare("UPDATE ARTICLE SET ID_soumission = :idSoum WHERE ID_article = :idArt");
        linkQuery.bindValue(":idSoum", selectedId);
        linkQuery.bindValue(":idArt",  idArticle);
        linkQuery.exec();

        isEditMode = false;
        selectedId = -1;
        ui->addBtn->setText("Ajouter");
        loadSubmissionsFromDB();
        clearForm();
        ui->tabWidget->setCurrentIndex(1);
        QMessageBox::information(this, "Succès", "✅ Soumission mise à jour avec succès!");
        return;
    }

    // 2. Insérer la soumission
    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO SOUMISSION (Date_envoi, Date_lim_revision, Etat, Decision_finale) "
        "VALUES (SYSDATE, :dateLim, :etat, :decision)"
        );
    insertQuery.bindValue(":dateLim",  dateLimite);
    insertQuery.bindValue(":etat",     ui->etatCombo->currentText());
    insertQuery.bindValue(":decision", "En attente");

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Erreur DB",
                              "Erreur lors de l'insertion de la soumission:\n" + insertQuery.lastError().text());
        return;
    }

    // 4. Récupérer l'ID de la soumission créée
    QSqlQuery lastIdQuery;
    lastIdQuery.exec("SELECT MAX(ID_soumission) FROM SOUMISSION");
    int newSoumId = 0;
    if (lastIdQuery.next()) newSoumId = lastIdQuery.value(0).toInt();

    // 5. Lier l'article à cette soumission
    QSqlQuery linkQuery;
    linkQuery.prepare("UPDATE ARTICLE SET ID_soumission = :idSoum WHERE ID_article = :idArt");
    linkQuery.bindValue(":idSoum", newSoumId);
    linkQuery.bindValue(":idArt",  idArticle);
    linkQuery.exec();

    // Recharger le tableau depuis la DB
    loadSubmissionsFromDB();

    // Afficher le message de succès avec bouton "Notifier relecteur"
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Succès");
    msgBox.setText("✅ Soumission ajoutée avec succès!\n\n"
                   "Article: " + article + "\n"
                               "Relecteur: " + relecteur + "\n"
                                 "Date limite: " + dateLimite.toString("dd/MM/yyyy"));

    // Ajouter un bouton personnalisé "Notifier relecteur"
    QPushButton *notifyBtn = msgBox.addButton("Notifier relecteur", QMessageBox::ActionRole);
    QPushButton *okBtn = msgBox.addButton("OK", QMessageBox::AcceptRole);

    // Styliser les boutons
    notifyBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #8B5CF6; "
        "    color: white; "
        "    border-radius: 6px; "
        "    padding: 8px 20px; "
        "    min-width: 120px; "
        "    font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "    background-color: #7C3AED; "
        "}"
        );

    okBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #10B981; "
        "    color: white; "
        "    border-radius: 6px; "
        "    padding: 8px 20px; "
        "    min-width: 80px; "
        "    font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "    background-color: #059669; "
        "}"
        );

    msgBox.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 400px; }"
        );

    msgBox.exec();

    // Vérifier quel bouton a été cliqué
    if (msgBox.clickedButton() == notifyBtn) {
        // Créer une notification dans un fichier
        QString notificationsPath = QDir::tempPath() + "/notifications.txt";
        QFile file(notificationsPath);

        qDebug() << "Creating notification at:" << notificationsPath;

        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);

            // Format: Relecteur|Article|DateLimite|DateCreation
            QString notifLine = relecteur + "|" + article + "|" + dateLimite.toString("dd/MM/yyyy") + "|" + QDate::currentDate().toString("dd/MM/yyyy");
            out << notifLine << "\n";

            file.close();

            qDebug() << "Notification created:" << notifLine;
            qDebug() << "File path:" << notificationsPath;

            QMessageBox successBox;
            successBox.setIcon(QMessageBox::Information);
            successBox.setWindowTitle("Notification Envoyée");
            successBox.setText("🔔 Notification envoyée avec succès!\n\n"
                               "Le relecteur " + relecteur.split(" - ").first() + " sera notifié lors de l'ouverture de l'interface Évaluation.\n\n"
                                                                  "Article: " + article + "\n"
                                           "Date limite: " + dateLimite.toString("dd/MM/yyyy") + "\n\n"
                                                                     "Fichier: " + notificationsPath);
            successBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 13px; padding: 10px; min-width: 450px; }"
                "QPushButton { background-color: #10B981; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; font-weight: 600; }"
                "QPushButton:hover { background-color: #059669; }"
                );
            successBox.exec();
        } else {
            qDebug() << "Failed to create notification file";
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setWindowTitle("Erreur");
            errorBox.setText("❌ Erreur lors de la création de la notification.\n\nChemin: " + notificationsPath);
            errorBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 400px; }"
                "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                "QPushButton:hover { background-color: #DC2626; }"
                );
            errorBox.exec();
        }
    }

    clearForm();
}

void Submission::onEditRowClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int id = btn->property("id").toInt();

    selectedRow = row;
    selectedId = id;
    isEditMode = true;

    populateFormFromRow(row);
    ui->addBtn->setText("Mettre à jour");

    // Rediriger vers l'onglet "Créer une Soumission"
    ui->tabWidget->setCurrentIndex(0);
}

void Submission::onDeleteRowClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int id = btn->property("id").toInt();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Confirmer la suppression");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer cette soumission?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    // Style personnalisé pour le popup
    msgBox.setStyleSheet(
        "QMessageBox { "
        "    background-color: white; "
        "    border-radius: 12px; "
        "} "
        "QMessageBox QLabel { "
        "    color: #1E293B; "
        "    font-size: 14px; "
        "    font-family: 'Segoe UI', sans-serif; "
        "    padding: 10px; "
        "    min-width: 300px; "
        "} "
        "QPushButton { "
        "    background-color: #8B5CF6; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px 20px; "
        "    font-size: 13px; "
        "    font-weight: 600; "
        "    min-width: 80px; "
        "    margin: 5px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #7C3AED; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #6D28D9; "
        "} "
        );

    // Personnaliser les boutons en français
    QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);
    QAbstractButton *noBtn = msgBox.button(QMessageBox::No);

    if (yesBtn) {
        yesBtn->setText("Oui");
        yesBtn->setStyleSheet(
            "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #DC2626; }"
            );
    }
    if (noBtn) {
        noBtn->setText("Non");
        noBtn->setStyleSheet(
            "QPushButton { background-color: #64748B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #475569; }"
            );
    }

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;

        // Supprimer d'abord les articles associés
        query.prepare("DELETE FROM ARTICLE WHERE ID_soumission = :id");
        query.bindValue(":id", id);
        query.exec();

        // Supprimer la soumission
        query.prepare("DELETE FROM SOUMISSION WHERE ID_soumission = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox successBox;
            successBox.setIcon(QMessageBox::Information);
            successBox.setWindowTitle("Succès");
            successBox.setText("Soumission supprimée avec succès!");
            successBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
                "QPushButton { background-color: #10B981; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                "QPushButton:hover { background-color: #059669; }"
                );
            successBox.exec();
            loadSubmissionsFromDB();
        } else {
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setWindowTitle("Erreur");
            errorBox.setText("Erreur lors de la suppression: " + query.lastError().text());
            errorBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
                "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                "QPushButton:hover { background-color: #DC2626; }"
                );
            errorBox.exec();
        }
    }
}

void Submission::onDeleteSubmission()
{
    if (selectedRow < 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Attention");
        msgBox.setText("Veuillez sélectionner une soumission à supprimer.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #D97706; }"
            );
        msgBox.exec();
        return;
    }

    int id = ui->submissionTable->item(selectedRow, 0)->text().toInt();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Confirmer la suppression");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer cette soumission?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    msgBox.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 300px; }"
        "QPushButton { background-color: #8B5CF6; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
        "QPushButton:hover { background-color: #7C3AED; }"
        );

    QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);
    QAbstractButton *noBtn = msgBox.button(QMessageBox::No);

    if (yesBtn) {
        yesBtn->setText("Oui");
        yesBtn->setStyleSheet(
            "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #DC2626; }"
            );
    }
    if (noBtn) {
        noBtn->setText("Non");
        noBtn->setStyleSheet(
            "QPushButton { background-color: #64748B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; margin: 5px; }"
            "QPushButton:hover { background-color: #475569; }"
            );
    }

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;

        // Supprimer d'abord les articles associés
        query.prepare("DELETE FROM ARTICLE WHERE ID_soumission = :id");
        query.bindValue(":id", id);
        query.exec();

        // Supprimer la soumission
        query.prepare("DELETE FROM SOUMISSION WHERE ID_soumission = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox successBox;
            successBox.setIcon(QMessageBox::Information);
            successBox.setWindowTitle("Succès");
            successBox.setText("Soumission supprimée avec succès!");
            successBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
                "QPushButton { background-color: #10B981; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                "QPushButton:hover { background-color: #059669; }"
                );
            successBox.exec();
            clearForm();
            loadSubmissionsFromDB();
        } else {
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setWindowTitle("Erreur");
            errorBox.setText("Erreur lors de la suppression: " + query.lastError().text());
            errorBox.setStyleSheet(
                "QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
                "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
                "QPushButton:hover { background-color: #DC2626; }"
                );
            errorBox.exec();
        }
    }
}

void Submission::onSearchTextChanged(const QString &text)
{
    QString searchText = text.trimmed();

    if (searchText.isEmpty()) {
        loadSubmissionsFromDB();
        return;
    }

    QString searchType = ui->searchTypeCombo->currentText();
    ui->submissionTable->setRowCount(0);

    QSqlQuery query;
    QString sql =
        "SELECT s.ID_soumission, a.titre, s.Etat, s.Decision_finale, s.Date_lim_revision "
        "FROM SOUMISSION s "
        "LEFT JOIN ARTICLE a ON a.ID_soumission = s.ID_soumission ";

    if (searchType == "Par Titre") {
        sql += "WHERE LOWER(a.titre) LIKE :search ";
    } else if (searchType == "Par Statut") {
        sql += "WHERE LOWER(s.Decision_finale) LIKE :search ";
    }
    sql += "ORDER BY s.Date_envoi DESC";

    query.prepare(sql);
    query.bindValue(":search", "%" + searchText.toLower() + "%");

    if (!query.exec()) {
        qDebug() << "Search error:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        int     id        = query.value(0).toInt();
        QString titre     = query.value(1).toString();
        QString etat      = query.value(2).toString();
        QString decision  = query.value(3).toString();
        QDate   date      = query.value(4).toDate();

        ui->submissionTable->insertRow(row);
        ui->submissionTable->setItem(row, 0, new QTableWidgetItem(titre));
        ui->submissionTable->setItem(row, 1, new QTableWidgetItem(etat));
        ui->submissionTable->setItem(row, 2, new QTableWidgetItem(decision));
        ui->submissionTable->setItem(row, 3, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
        ui->submissionTable->setItem(row, 4, new QTableWidgetItem(""));
        addButtonsToRow(row, id);
        row++;
    }
}

void Submission::onSearchClicked()
{
    // Cette fonction est maintenant appelée par le bouton Rechercher
    // mais la recherche en temps réel se fait via onSearchTextChanged
    QString searchText = ui->searchEdit->text().trimmed();

    if (searchText.isEmpty()) {
        loadSubmissionsFromDB();
    } else {
        onSearchTextChanged(searchText);
    }
}

void Submission::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->submissionTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        selectedRow = selectedItems.first()->row();
        populateFormFromRow(selectedRow);
    }
}

void Submission::populateFormFromRow(int row)
{
    if (row < 0 || row >= ui->submissionTable->rowCount()) return;

    // Article (col 0)
    ui->titleEdit->setText(ui->submissionTable->item(row, 0)->text());

    // État (col 1)
    int etatIdx = ui->etatCombo->findText(ui->submissionTable->item(row, 1)->text());
    if (etatIdx >= 0) ui->etatCombo->setCurrentIndex(etatIdx);

    // Décision Finale (col 2) — lecture seule, reflète la dernière valeur
    ui->statusCombo->setCurrentIndex(0); // reset visuel
    QString decision = ui->submissionTable->item(row, 2)->text();
    // On met à jour le texte du widget disabled pour afficher la vraie valeur
    int decIdx = ui->statusCombo->findText(decision);
    if (decIdx >= 0) ui->statusCombo->setCurrentIndex(decIdx);

    // Date limite (col 3)
    QDate date = QDate::fromString(ui->submissionTable->item(row, 3)->text(), "dd/MM/yyyy");
    if (date.isValid()) ui->dateEdit->setDate(date);

    // Relecteur (col 4)
    ui->reviewerEdit->setText(ui->submissionTable->item(row, 4)->text());
}

bool Submission::validateForm()
{
    if (ui->statusCombo->currentText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Attention");
        msgBox.setText("Veuillez sélectionner un état.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #D97706; }"
            );
        msgBox.exec();
        return false;
    }

    return true;
}

void Submission::clearForm()
{
    ui->reviewerEdit->clear();
    ui->reviewerEdit->setProperty("relecteur_id", 0);
    ui->statusCombo->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
    selectedRow = -1;
    selectedId = -1;
    isEditMode = false;
    ui->addBtn->setText("Ajouter");
}

void Submission::onChooseReviewer()
{
    QMenu *relecteurMenu = new QMenu(this);
    relecteurMenu->setStyleSheet(
        "QMenu { background-color: white; border: 1.5px solid #E2E8F0; border-radius: 8px; padding: 8px; } "
        "QMenu::item { padding: 10px 24px; font-size: 13px; color: #475569; border-radius: 6px; } "
        "QMenu::item:selected { background-color: #F3E8FF; color: #8B5CF6; }"
        );

    relecteurMenu->addAction("Mehdi Karaa - Intelligence Artificielle");
    relecteurMenu->addAction("Rayen Ouerghui - Cybersécurité");
    relecteurMenu->addAction("Louay Fkiri - Data Science");
    relecteurMenu->addAction("Leo Messi - Génie Logiciel");

    QAction *selected = relecteurMenu->exec(QCursor::pos());
    if (selected)
        ui->reviewerEdit->setText(selected->text());

    delete relecteurMenu;
}

void Submission::onAutoAssignReviewer()
{
    QString selectedArticle = ui->titleEdit->text().trimmed();

    if (selectedArticle.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord sélectionner un article.");
        return;
    }

    // Liste fixe des relecteurs avec leur domaine
    QMap<QString, QString> relecteurs = {
        {"Mehdi Karaa",   "Intelligence Artificielle"},
        {"Rayen Ouerghui","Cybersécurité"},
        {"Louay Fkiri",   "Data Science"},
        {"Leo Messi",     "Génie Logiciel"}
    };

    // Chercher le domaine de l'article dans la DB
    QSqlQuery q;
    q.prepare("SELECT domaine FROM ARTICLE WHERE titre = :titre AND ROWNUM = 1");
    q.bindValue(":titre", selectedArticle);
    QString articleDomain;
    if (q.exec() && q.next())
        articleDomain = q.value(0).toString();

    // Trouver le relecteur dont le domaine correspond
    QString assignedReviewer;
    QString expertise;
    for (auto it = relecteurs.begin(); it != relecteurs.end(); ++it) {
        if (articleDomain.contains(it.value(), Qt::CaseInsensitive) ||
            it.value().contains(articleDomain, Qt::CaseInsensitive)) {
            assignedReviewer = it.key() + " - " + it.value();
            expertise = it.value();
            break;
        }
    }

    // Fallback: premier relecteur de la liste
    if (assignedReviewer.isEmpty()) {
        assignedReviewer = "Mehdi Karaa - Intelligence Artificielle";
        expertise = "Intelligence Artificielle";
    }

    ui->reviewerEdit->setText(assignedReviewer);

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Attribution Automatique");
    msgBox.setText(QString("✅ Relecteur attribué automatiquement!\n\n"
                           "Article: %1\n"
                           "Domaine: %2\n"
                           "Relecteur: %3")
                       .arg(selectedArticle, articleDomain, assignedReviewer));
    msgBox.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 400px; }"
        "QPushButton { background-color: #10B981; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
        "QPushButton:hover { background-color: #059669; }"
        );
    msgBox.exec();
}

void Submission::onSelectArticle()
{
    QMenu *articleMenu = new QMenu(this);
    articleMenu->setStyleSheet(
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

    // Charger les articles depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT titre, domaine FROM ARTICLE ORDER BY titre ASC");

    if (query.exec()) {
        while (query.next()) {
            QString titre   = query.value(0).toString();
            QString domaine = query.value(1).toString();
            // Afficher "Titre — Domaine" dans le menu
            articleMenu->addAction(titre + " — " + domaine);
        }
    }

    if (articleMenu->isEmpty()) {
        articleMenu->addAction("Aucun article disponible")->setEnabled(false);
    }

    QAction *selectedAction = articleMenu->exec(QCursor::pos());

    if (selectedAction && selectedAction->isEnabled()) {
        // Stocker uniquement le titre dans titleEdit (avant " — ")
        QString fullText = selectedAction->text();
        QString titre = fullText.section(" — ", 0, 0);
        ui->titleEdit->setText(titre);
    }

    delete articleMenu;
}

void Submission::onExportClicked()
{
    // Vérifier s'il y a des données à exporter
    if (ui->submissionTable->rowCount() == 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Attention");
        msgBox.setText("Aucune soumission à exporter.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: #F59E0B; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #D97706; }"
            );
        msgBox.exec();
        return;
    }

    // Ouvrir une boîte de dialogue pour choisir l'emplacement du fichier PDF
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/soumissions_export.pdf";

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Enregistrer la liste en PDF",
        defaultPath,
        "Fichiers PDF (*.pdf)"
        );

    // Si l'utilisateur a annulé, ne rien faire
    if (fileName.isEmpty()) {
        return;
    }

    // S'assurer que le fichier a l'extension .pdf
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Créer le document HTML pour le PDF
    QString html = "<html><head><meta charset='UTF-8'><style>"
                   "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 20px; }"
                   "h1 { color: #8B5CF6; text-align: center; margin-bottom: 30px; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th { background-color: #8B5CF6; color: white; padding: 12px; text-align: left; font-weight: 600; }"
                   "td { padding: 10px; border-bottom: 1px solid #E2E8F0; }"
                   "tr:nth-child(even) { background-color: #F8FAFC; }"
                   ".footer { text-align: center; margin-top: 30px; color: #64748B; font-size: 12px; }"
                   "</style></head><body>";

    html += "<h1>Liste des Soumissions</h1>";
    html += "<p style='text-align: center; color: #64748B;'>Généré le " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";

    html += "<table>";
    html += "<tr><th>Article</th><th>Décision Finale</th><th>Date Limite Révision</th><th>Relecteur</th></tr>";

    // Parcourir toutes les lignes du tableau
    for (int row = 0; row < ui->submissionTable->rowCount(); ++row) {
        html += "<tr>";

        // Article (colonne 0)
        QTableWidgetItem *articleItem = ui->submissionTable->item(row, 0);
        html += "<td>" + (articleItem ? articleItem->text() : "") + "</td>";

        // Décision Finale (colonne 1)
        QTableWidgetItem *decisionItem = ui->submissionTable->item(row, 1);
        html += "<td>" + (decisionItem ? decisionItem->text() : "") + "</td>";

        // Date Limite Révision (colonne 2)
        QTableWidgetItem *dateItem = ui->submissionTable->item(row, 2);
        html += "<td>" + (dateItem ? dateItem->text() : "") + "</td>";

        // Relecteur (colonne 3)
        QTableWidgetItem *relecteurItem = ui->submissionTable->item(row, 3);
        html += "<td>" + (relecteurItem ? relecteurItem->text() : "") + "</td>";

        html += "</tr>";
    }

    html += "</table>";
    html += "<div class='footer'>Document généré par Smart Research Publication Management</div>";
    html += "</body></html>";

    // Créer un QTextDocument et le rendre en PDF
    QTextDocument document;
    document.setHtml(html);

    // Créer le printer et configurer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // Générer le PDF
    document.print(&printer);

    // Vérifier si le fichier a été créé
    QFile file(fileName);
    if (!file.exists()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Erreur lors de la création du fichier PDF.\n\nVérifiez que vous avez les droits d'écriture dans le dossier sélectionné.");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: #1E293B; font-size: 14px; padding: 10px; min-width: 350px; }"
            "QPushButton { background-color: #EF4444; color: white; border-radius: 6px; padding: 8px 20px; min-width: 80px; }"
            "QPushButton:hover { background-color: #DC2626; }"
            );
        msgBox.exec();
        return;
    }

    // Afficher le message de confirmation
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Export réussi");
    msgBox.setText("La liste des soumissions a été exportée avec succès!\n\nEmplacement: " + fileName);
    msgBox.setInformativeText("Nombre de soumissions exportées: " + QString::number(ui->submissionTable->rowCount()));
    msgBox.setStyleSheet(
        "QMessageBox { "
        "    background-color: white; "
        "    border-radius: 12px; "
        "} "
        "QMessageBox QLabel { "
        "    color: #1E293B; "
        "    font-size: 14px; "
        "    font-family: 'Segoe UI', sans-serif; "
        "    padding: 10px; "
        "    min-width: 350px; "
        "} "
        "QPushButton { "
        "    background-color: #10B981; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px 20px; "
        "    font-size: 13px; "
        "    font-weight: 600; "
        "    min-width: 80px; "
        "    margin: 5px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #059669; "
        "} "
        );
    msgBox.exec();
}

void Submission::onStatisticsClicked()
{
    // Créer une boîte de dialogue pour afficher les statistiques
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Soumissions");
    statsDialog->setMinimumSize(1000, 600);
    statsDialog->setStyleSheet("QDialog { background-color: #F5F7FA; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Titre
    QLabel *titleLabel = new QLabel("Statistiques des Soumissions");
    titleLabel->setStyleSheet(
        "font-size: 24px; "
        "font-weight: bold; "
        "color: #8B5CF6; "
        "padding: 10px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Layout horizontal pour les deux graphiques
    QHBoxLayout *chartsLayout = new QHBoxLayout();

    // ============================================
    // GRAPHIQUE 1: Statistiques de Performance
    // ============================================

    // Compter les soumissions par décision
    int accepte = 0;
    int rejete = 0;
    int enAttente = 0;
    int enRevision = 0;

    for (int row = 0; row < ui->submissionTable->rowCount(); ++row) {
        QTableWidgetItem *decisionItem = ui->submissionTable->item(row, 1);
        if (decisionItem) {
            QString decision = decisionItem->text();
            if (decision == "Accepté") {
                accepte++;
            } else if (decision == "Rejeté") {
                rejete++;
            } else if (decision == "En attente") {
                enAttente++;
            } else if (decision == "En révision") {
                enRevision++;
            }
        }
    }

    // Créer le graphique de performance
    QPieSeries *performanceSeries = new QPieSeries();

    if (accepte > 0) {
        QPieSlice *sliceAccepte = performanceSeries->append("Accepté", accepte);
        sliceAccepte->setBrush(QColor("#10B981")); // Vert
        sliceAccepte->setLabelVisible(true);
        sliceAccepte->setLabel(QString("Accepté: %1 (%2%)").arg(accepte).arg(100.0 * accepte / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    if (rejete > 0) {
        QPieSlice *sliceRejete = performanceSeries->append("Rejeté", rejete);
        sliceRejete->setBrush(QColor("#EF4444")); // Rouge
        sliceRejete->setLabelVisible(true);
        sliceRejete->setLabel(QString("Rejeté: %1 (%2%)").arg(rejete).arg(100.0 * rejete / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    if (enAttente > 0) {
        QPieSlice *sliceAttente = performanceSeries->append("En attente", enAttente);
        sliceAttente->setBrush(QColor("#F59E0B")); // Orange
        sliceAttente->setLabelVisible(true);
        sliceAttente->setLabel(QString("En attente: %1 (%2%)").arg(enAttente).arg(100.0 * enAttente / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    if (enRevision > 0) {
        QPieSlice *sliceRevision = performanceSeries->append("En révision", enRevision);
        sliceRevision->setBrush(QColor("#3B82F6")); // Bleu
        sliceRevision->setLabelVisible(true);
        sliceRevision->setLabel(QString("En révision: %1 (%2%)").arg(enRevision).arg(100.0 * enRevision / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    QChart *performanceChart = new QChart();
    performanceChart->addSeries(performanceSeries);
    performanceChart->setTitle("Statistiques de Performance");
    performanceChart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    performanceChart->legend()->setVisible(true);
    performanceChart->legend()->setAlignment(Qt::AlignBottom);
    performanceChart->setAnimationOptions(QChart::SeriesAnimations);
    performanceChart->setBackgroundBrush(QBrush(Qt::white));

    QChartView *performanceChartView = new QChartView(performanceChart);
    performanceChartView->setRenderHint(QPainter::Antialiasing);
    performanceChartView->setMinimumSize(450, 400);

    chartsLayout->addWidget(performanceChartView);

    // ============================================
    // GRAPHIQUE 2: Statistiques d'États
    // ============================================

    int traitees = accepte + rejete;
    int nonTraitees = enAttente + enRevision;

    QPieSeries *etatSeries = new QPieSeries();

    if (traitees > 0) {
        QPieSlice *sliceTraitees = etatSeries->append("Traitées", traitees);
        sliceTraitees->setBrush(QColor("#8B5CF6")); // Violet
        sliceTraitees->setLabelVisible(true);
        sliceTraitees->setLabel(QString("Traitées: %1 (%2%)").arg(traitees).arg(100.0 * traitees / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    if (nonTraitees > 0) {
        QPieSlice *sliceNonTraitees = etatSeries->append("Non traitées", nonTraitees);
        sliceNonTraitees->setBrush(QColor("#64748B")); // Gris
        sliceNonTraitees->setLabelVisible(true);
        sliceNonTraitees->setLabel(QString("Non traitées: %1 (%2%)").arg(nonTraitees).arg(100.0 * nonTraitees / ui->submissionTable->rowCount(), 0, 'f', 1));
    }

    QChart *etatChart = new QChart();
    etatChart->addSeries(etatSeries);
    etatChart->setTitle("Statistiques d'États");
    etatChart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    etatChart->legend()->setVisible(true);
    etatChart->legend()->setAlignment(Qt::AlignBottom);
    etatChart->setAnimationOptions(QChart::SeriesAnimations);
    etatChart->setBackgroundBrush(QBrush(Qt::white));

    QChartView *etatChartView = new QChartView(etatChart);
    etatChartView->setRenderHint(QPainter::Antialiasing);
    etatChartView->setMinimumSize(450, 400);

    chartsLayout->addWidget(etatChartView);

    mainLayout->addLayout(chartsLayout);

    // Bouton Fermer
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #8B5CF6; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 30px; "
        "    font-size: 14px; "
        "    font-weight: 600; "
        "    min-width: 120px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #7C3AED; "
        "}"
        );
    connect(closeBtn, &QPushButton::clicked, statsDialog, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    statsDialog->exec();
    delete statsDialog;
}

void Submission::loadSubmissions()
{
    // Cette méthode est maintenant remplacée par loadSubmissionsFromDB()
    loadSubmissionsFromDB();
}
