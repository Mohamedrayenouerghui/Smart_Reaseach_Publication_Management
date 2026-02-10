#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/evaluationlistwindow.h"
#include "headers/evaluationformdialog.h"
#include "headers/reviewerwindow.h"
#include "headers/statisticswindow.h"
#include "headers/database.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Initialize database
    if (!Database::instance().initDatabase()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'initialiser la base de données");
    }
    
    setupConnections();
    updateStatistics();
    loadRecentEvaluations();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->btnNouvelleEvaluation, &QPushButton::clicked, this, &MainWindow::onNouvelleEvaluation);
    connect(ui->btnMesEvaluations, &QPushButton::clicked, this, &MainWindow::onMesEvaluations);
    connect(ui->btnGestionRelecture, &QPushButton::clicked, this, &MainWindow::onGestionRelecture);
    connect(ui->btnStatistiques, &QPushButton::clicked, this, &MainWindow::onStatistiques);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &MainWindow::onExportPDF);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefreshDashboard);
    
    // Menu actions
    connect(ui->actionNouvelle_Evaluation, &QAction::triggered, this, &MainWindow::onNouvelleEvaluation);
    connect(ui->actionMes_Evaluations, &QAction::triggered, this, &MainWindow::onMesEvaluations);
    connect(ui->actionStatistiques, &QAction::triggered, this, &MainWindow::onStatistiques);
    connect(ui->actionQuitter, &QAction::triggered, this, &QMainWindow::close);
}

void MainWindow::onNouvelleEvaluation()
{
    EvaluationFormDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Evaluation eval = dialog.getEvaluation();
        if (Database::instance().createEvaluation(eval)) {
            QMessageBox::information(this, "Succès", "Évaluation créée avec succès");
            updateStatistics();
            loadRecentEvaluations();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de créer l'évaluation");
        }
    }
}

void MainWindow::onMesEvaluations()
{
    EvaluationListWindow *window = new EvaluationListWindow(this);
    window->show();
}

void MainWindow::onGestionRelecture()
{
    ReviewerWindow *window = new ReviewerWindow(this);
    window->show();
}

void MainWindow::onStatistiques()
{
    StatisticsWindow *window = new StatisticsWindow(this);
    window->show();
}

void MainWindow::onExportPDF()
{
    QMessageBox::information(this, "Export PDF", "Fonctionnalité d'export PDF à implémenter");
}

void MainWindow::onRefreshDashboard()
{
    updateStatistics();
    loadRecentEvaluations();
}

void MainWindow::updateStatistics()
{
    int total = Database::instance().getTotalEvaluations();
    int pending = Database::instance().getPendingEvaluations();
    int completed = Database::instance().getCompletedEvaluations();
    
    ui->lblTotalEvaluations->setText(QString::number(total));
    ui->lblPendingEvaluations->setText(QString::number(pending));
    ui->lblCompletedEvaluations->setText(QString::number(completed));
}

void MainWindow::loadRecentEvaluations()
{
    ui->tableRecentEvaluations->setRowCount(0);
    ui->tableRecentEvaluations->setColumnCount(6);
    ui->tableRecentEvaluations->setHorizontalHeaderLabels(
        {"ID", "Soumission", "Relecteur", "Note", "Date", "Statut"});
    
    QVector<Evaluation> evaluations = Database::instance().getAllEvaluations();
    
    int maxRows = qMin(10, evaluations.size());
    for (int i = 0; i < maxRows; ++i) {
        const Evaluation& eval = evaluations[i];
        ui->tableRecentEvaluations->insertRow(i);
        
        ui->tableRecentEvaluations->setItem(i, 0, new QTableWidgetItem(QString::number(eval.getId())));
        ui->tableRecentEvaluations->setItem(i, 1, new QTableWidgetItem(QString::number(eval.getSubmissionId())));
        
        Reviewer reviewer = Database::instance().getReviewerById(eval.getReviewerId());
        ui->tableRecentEvaluations->setItem(i, 2, new QTableWidgetItem(reviewer.getNom()));
        
        ui->tableRecentEvaluations->setItem(i, 3, new QTableWidgetItem(QString::number(eval.getNote())));
        ui->tableRecentEvaluations->setItem(i, 4, new QTableWidgetItem(eval.getDateEvaluation().toString("dd/MM/yyyy")));
        ui->tableRecentEvaluations->setItem(i, 5, new QTableWidgetItem(eval.getStatutString()));
    }
    
    ui->tableRecentEvaluations->resizeColumnsToContents();
}
