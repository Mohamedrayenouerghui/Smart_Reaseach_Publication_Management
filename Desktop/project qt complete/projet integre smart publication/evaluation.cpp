#include "evaluation.h"
#include "ui_evaluation.h"
#include "connection.h"
#include <QDate>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTimer>

Evaluation::Evaluation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Evaluation)
    , selectedRow(-1)
    , selectedId(-1)
    , isEditMode(false)
    , unreadNotifications(0)
{
    ui->setupUi(this);
    
    setupTable();
    setupConnections();
    loadEvaluations();
    
    // NE PLUS vérifier les notifications automatiquement
    // Les notifications sont affichées uniquement lors de la connexion
}

Evaluation::~Evaluation()
{
    delete ui;
}

void Evaluation::setCurrentReviewer(const QString &reviewerName)
{
    currentReviewer = reviewerName;
    // NE PLUS appeler checkNotifications ici
    // Les notifications sont affichées uniquement lors de la connexion
}

void Evaluation::setupTable()
{
    ui->evaluationTable->setColumnCount(6);
    ui->evaluationTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Article" << "Note" << "Statut" << "Date" << "Actions"
    );
    
    ui->evaluationTable->setColumnWidth(0, 60);
    ui->evaluationTable->setColumnWidth(1, 300);
    ui->evaluationTable->setColumnWidth(2, 80);
    ui->evaluationTable->setColumnWidth(3, 120);
    ui->evaluationTable->setColumnWidth(4, 120);
    ui->evaluationTable->setColumnWidth(5, 200);
    
    ui->evaluationTable->verticalHeader()->setDefaultSectionSize(70);
    ui->evaluationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->evaluationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Evaluation::setupConnections()
{
    connect(ui->addBtn, &QPushButton::clicked, this, &Evaluation::onAddEvaluation);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &Evaluation::onDeleteEvaluation);
    connect(ui->clearBtn, &QPushButton::clicked, this, &Evaluation::clearForm);
    connect(ui->searchBtn, &QPushButton::clicked, this, &Evaluation::onSearchClicked);
    connect(ui->evaluationTable, &QTableWidget::itemSelectionChanged, this, &Evaluation::onTableSelectionChanged);
}

void Evaluation::checkNotifications()
{
    // Lire les notifications depuis le fichier
    QString notificationsPath = QDir::tempPath() + "/notifications.txt";
    QFile file(notificationsPath);
    
    qDebug() << "Checking notifications at:" << notificationsPath;
    qDebug() << "File exists:" << file.exists();
    
    if (!file.exists()) {
        qDebug() << "No notifications file found";
        return; // Pas de notifications
    }
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList notifications;
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                notifications.append(line);
                qDebug() << "Found notification:" << line;
            }
        }
        file.close();
        
        qDebug() << "Total notifications:" << notifications.count();
        
        if (!notifications.isEmpty()) {
            showNotificationDialog(notifications);
            
            // Supprimer le fichier après lecture
            file.remove();
            qDebug() << "Notifications file deleted";
        }
    } else {
        qDebug() << "Failed to open notifications file";
    }
}

void Evaluation::showNotificationDialog()
{
    // Cette fonction n'est plus utilisée, remplacée par showNotificationDialog(QStringList)
}

void Evaluation::showNotificationDialog(const QStringList &notifications)
{
    QDialog *notifDialog = new QDialog(this);
    notifDialog->setWindowTitle("Nouvelles Notifications");
    notifDialog->setMinimumSize(700, 500);
    notifDialog->setStyleSheet("QDialog { background-color: #F5F7FA; }");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(notifDialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Titre
    QLabel *titleLabel = new QLabel(QString("🔔 Vous avez %1 nouvelle(s) notification(s)").arg(notifications.count()));
    titleLabel->setStyleSheet(
        "font-size: 20px; "
        "font-weight: bold; "
        "color: #8B5CF6; "
        "padding: 10px;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Zone de défilement pour les notifications
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: transparent; }");
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->setSpacing(15);
    
    // Afficher chaque notification
    for (const QString &notif : notifications) {
        QStringList parts = notif.split("|");
        if (parts.size() >= 4) {
            QString relecteur = parts[0];
            QString article = parts[1];
            QString dateLimite = parts[2];
            QString dateCreation = parts[3];
            
            // Extraire le nom et prénom du relecteur (avant le tiret)
            QString nomPrenom = relecteur.split(" - ").first();
            
            // Créer un widget pour chaque notification
            QWidget *notifWidget = new QWidget();
            notifWidget->setStyleSheet(
                "QWidget { "
                "    background-color: white; "
                "    border-radius: 12px; "
                "    border-left: 5px solid #8B5CF6; "
                "    padding: 20px; "
                "}"
            );
            
            QVBoxLayout *notifLayout = new QVBoxLayout(notifWidget);
            notifLayout->setSpacing(10);
            
            // Titre de la notification
            QLabel *titleNotif = new QLabel("📝 Nouvelle Évaluation à Faire");
            titleNotif->setStyleSheet(
                "font-size: 16px; "
                "color: #8B5CF6; "
                "font-weight: bold; "
                "margin-bottom: 5px;"
            );
            
            // Relecteur
            QLabel *relecteurLabel = new QLabel("👤 Relecteur: " + nomPrenom);
            relecteurLabel->setStyleSheet(
                "font-size: 14px; "
                "color: #1E293B; "
                "font-weight: 600;"
            );
            
            // Article
            QLabel *articleLabel = new QLabel("📄 Article: " + article);
            articleLabel->setStyleSheet(
                "font-size: 14px; "
                "color: #475569; "
                "margin-top: 5px;"
            );
            articleLabel->setWordWrap(true);
            
            // Date limite
            QLabel *dateLimiteLabel = new QLabel("⏰ Date limite d'évaluation: " + dateLimite);
            dateLimiteLabel->setStyleSheet(
                "font-size: 14px; "
                "color: #EF4444; "
                "font-weight: 600; "
                "margin-top: 5px;"
            );
            
            // Date de création
            QLabel *dateCreationLabel = new QLabel("📅 Notification créée le: " + dateCreation);
            dateCreationLabel->setStyleSheet(
                "font-size: 12px; "
                "color: #64748B; "
                "margin-top: 5px;"
            );
            
            notifLayout->addWidget(titleNotif);
            notifLayout->addWidget(relecteurLabel);
            notifLayout->addWidget(articleLabel);
            notifLayout->addWidget(dateLimiteLabel);
            notifLayout->addWidget(dateCreationLabel);
            
            scrollLayout->addWidget(notifWidget);
        }
    }
    
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
    
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
    connect(closeBtn, &QPushButton::clicked, notifDialog, &QDialog::accept);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    notifDialog->exec();
    delete notifDialog;
}

void Evaluation::markNotificationAsRead(int)
{
    // Fonction vide - pas de base de données
}

void Evaluation::loadEvaluations()
{
    ui->evaluationTable->setRowCount(0);
    
    QSqlQuery query;
    query.prepare("SELECT e.ID_evaluation, a.titre, e.note, e.statut, e.Date_evaluation "
                  "FROM EVALUATION e "
                  "LEFT JOIN SOUMISSION s ON e.ID_soumission = s.ID_soumission "
                  "LEFT JOIN ARTICLE a ON s.ID_soumission = a.ID_soumission "
                  "ORDER BY e.Date_evaluation DESC");
    
    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            ui->evaluationTable->insertRow(row);
            
            int id = query.value(0).toInt();
            ui->evaluationTable->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
            ui->evaluationTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->evaluationTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->evaluationTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            
            QDate date = query.value(4).toDate();
            ui->evaluationTable->setItem(row, 4, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
            
            addButtonsToRow(row, id);
            row++;
        }
    }
}

void Evaluation::addButtonsToRow(int row, int id)
{
    QWidget* actionWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(actionWidget);
    
    QPushButton *modifyBtn = new QPushButton("Modifier");
    QPushButton *deleteBtn = new QPushButton("Supprimer");
    
    modifyBtn->setStyleSheet(
        "QPushButton { background-color: #3B82F6; color: white; border-radius: 4px; "
        "padding: 8px 12px; font-size: 13px; font-weight: 600; min-height: 35px; } "
        "QPushButton:hover { background-color: #2563EB; }"
    );
    
    deleteBtn->setStyleSheet(
        "QPushButton { background-color: #EF4444; color: white; border-radius: 4px; "
        "padding: 8px 12px; font-size: 13px; font-weight: 600; min-height: 35px; } "
        "QPushButton:hover { background-color: #DC2626; }"
    );
    
    modifyBtn->setProperty("row", row);
    modifyBtn->setProperty("id", id);
    
    deleteBtn->setProperty("row", row);
    deleteBtn->setProperty("id", id);
    connect(deleteBtn, &QPushButton::clicked, this, &Evaluation::onDeleteEvaluation);
    
    layout->addWidget(modifyBtn);
    layout->addWidget(deleteBtn);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);
    
    ui->evaluationTable->setCellWidget(row, 5, actionWidget);
}

void Evaluation::onAddEvaluation()
{
    if (!validateForm()) {
        return;
    }
    
    QMessageBox::information(this, "Succès", "Évaluation ajoutée avec succès!");
    clearForm();
    loadEvaluations();
}

void Evaluation::onDeleteEvaluation()
{
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une évaluation à supprimer.");
        return;
    }
    
    QMessageBox::information(this, "Succès", "Évaluation supprimée avec succès!");
    loadEvaluations();
}

void Evaluation::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->evaluationTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        selectedRow = selectedItems.first()->row();
        populateFormFromRow(selectedRow);
    }
}

void Evaluation::onSearchClicked()
{
    loadEvaluations();
}

void Evaluation::populateFormFromRow(int row)
{
    // Implémenter si nécessaire
}

bool Evaluation::validateForm()
{
    return true;
}

void Evaluation::clearForm()
{
    selectedRow = -1;
    selectedId = -1;
    isEditMode = false;
}
