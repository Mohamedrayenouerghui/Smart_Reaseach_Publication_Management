#include "headers/reviewerwindow.h"
#include "ui_reviewerwindow.h"
#include "headers/database.h"
#include <QMessageBox>
#include <QInputDialog>

ReviewerWindow::ReviewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReviewerWindow),
    reviewerModel(new QStandardItemModel(this)),
    evaluationModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setupModels();
    setupConnections();
    loadReviewers();
}

ReviewerWindow::~ReviewerWindow()
{
    delete ui;
}

void ReviewerWindow::setupModels()
{
    // Setup reviewer list model
    reviewerModel->setHorizontalHeaderLabels({"ID", "Nom", "Email"});
    ui->listViewReviewers->setModel(reviewerModel);
    
    // Setup evaluation table model
    evaluationModel->setHorizontalHeaderLabels({"ID", "Soumission", "Note", "Date", "Statut"});
    ui->tableViewEvaluations->setModel(evaluationModel);
    ui->tableViewEvaluations->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewEvaluations->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ReviewerWindow::setupConnections()
{
    connect(ui->listViewReviewers, &QListView::clicked, this, &ReviewerWindow::onReviewerSelected);
    connect(ui->btnAddReviewer, &QPushButton::clicked, this, &ReviewerWindow::onAddReviewer);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &ReviewerWindow::onRefresh);
}

void ReviewerWindow::loadReviewers()
{
    reviewerModel->removeRows(0, reviewerModel->rowCount());
    
    QVector<Reviewer> reviewers = Database::instance().getAllReviewers();
    
    for (const Reviewer& reviewer : reviewers) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(reviewer.getId()));
        row << new QStandardItem(reviewer.getNom());
        row << new QStandardItem(reviewer.getEmail());
        
        reviewerModel->appendRow(row);
    }
}

void ReviewerWindow::loadReviewerEvaluations(int reviewerId)
{
    evaluationModel->removeRows(0, evaluationModel->rowCount());
    
    QVector<Evaluation> evaluations = Database::instance().getEvaluationsByReviewer(reviewerId);
    
    ui->lblReviewerStats->setText(QString("Total évaluations: %1").arg(evaluations.size()));
    
    for (const Evaluation& eval : evaluations) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(eval.getId()));
        row << new QStandardItem(QString::number(eval.getSubmissionId()));
        row << new QStandardItem(QString::number(eval.getNote()));
        row << new QStandardItem(eval.getDateEvaluation().toString("dd/MM/yyyy"));
        row << new QStandardItem(eval.getStatutString());
        
        evaluationModel->appendRow(row);
    }
    
    ui->tableViewEvaluations->resizeColumnsToContents();
}

void ReviewerWindow::onReviewerSelected(const QModelIndex& index)
{
    if (!index.isValid()) return;
    
    int row = index.row();
    int reviewerId = reviewerModel->item(row, 0)->text().toInt();
    QString reviewerName = reviewerModel->item(row, 1)->text();
    
    ui->lblSelectedReviewer->setText("Évaluations de: " + reviewerName);
    loadReviewerEvaluations(reviewerId);
}

void ReviewerWindow::onAddReviewer()
{
    bool ok;
    QString nom = QInputDialog::getText(this, "Nouveau relecteur",
                                        "Nom du relecteur:", QLineEdit::Normal,
                                        "", &ok);
    if (!ok || nom.isEmpty()) return;
    
    QString email = QInputDialog::getText(this, "Nouveau relecteur",
                                          "Email:", QLineEdit::Normal,
                                          "", &ok);
    if (!ok || email.isEmpty()) return;
    
    Reviewer reviewer(0, nom, email);
    if (Database::instance().createReviewer(reviewer)) {
        QMessageBox::information(this, "Succès", "Relecteur ajouté");
        loadReviewers();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ajouter le relecteur");
    }
}

void ReviewerWindow::onRefresh()
{
    loadReviewers();
    evaluationModel->removeRows(0, evaluationModel->rowCount());
    ui->lblSelectedReviewer->clear();
    ui->lblReviewerStats->clear();
}
