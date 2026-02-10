#include "headers/evaluationlistwindow.h"
#include "ui_evaluationlistwindow.h"
#include "headers/evaluationformdialog.h"
#include "headers/database.h"
#include <QMessageBox>
#include <QStandardItem>

EvaluationListWindow::EvaluationListWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EvaluationListWindow),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setupTable();
    setupConnections();
    loadEvaluations();
}

EvaluationListWindow::~EvaluationListWindow()
{
    delete ui;
}

void EvaluationListWindow::setupTable()
{
    model->setHorizontalHeaderLabels({"ID", "Soumission", "Relecteur", "Note", "Date", "Statut"});
    ui->tableViewEvaluations->setModel(model);
    ui->tableViewEvaluations->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewEvaluations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewEvaluations->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void EvaluationListWindow::setupConnections()
{
    connect(ui->btnCreate, &QPushButton::clicked, this, &EvaluationListWindow::onCreateEvaluation);
    connect(ui->btnEdit, &QPushButton::clicked, this, &EvaluationListWindow::onEditEvaluation);
    connect(ui->btnDelete, &QPushButton::clicked, this, &EvaluationListWindow::onDeleteEvaluation);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &EvaluationListWindow::onExportPDF);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &EvaluationListWindow::onRefresh);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &EvaluationListWindow::onSearch);
    connect(ui->comboBoxFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &EvaluationListWindow::onFilterByStatus);
    connect(ui->btnSortDate, &QPushButton::clicked, this, &EvaluationListWindow::onSortByDate);
    connect(ui->btnSortNote, &QPushButton::clicked, this, &EvaluationListWindow::onSortByNote);
}

void EvaluationListWindow::loadEvaluations()
{
    model->removeRows(0, model->rowCount());
    
    QVector<Evaluation> evaluations = Database::instance().getAllEvaluations();
    
    for (const Evaluation& eval : evaluations) {
        QList<QStandardItem*> row;
        
        row << new QStandardItem(QString::number(eval.getId()));
        row << new QStandardItem(QString::number(eval.getSubmissionId()));
        
        Reviewer reviewer = Database::instance().getReviewerById(eval.getReviewerId());
        row << new QStandardItem(reviewer.getNom());
        
        row << new QStandardItem(QString::number(eval.getNote()));
        row << new QStandardItem(eval.getDateEvaluation().toString("dd/MM/yyyy"));
        row << new QStandardItem(eval.getStatutString());
        
        model->appendRow(row);
    }
    
    ui->tableViewEvaluations->resizeColumnsToContents();
}

void EvaluationListWindow::onCreateEvaluation()
{
    EvaluationFormDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Evaluation eval = dialog.getEvaluation();
        if (Database::instance().createEvaluation(eval)) {
            QMessageBox::information(this, "Succès", "Évaluation créée");
            loadEvaluations();
        }
    }
}

void EvaluationListWindow::onEditEvaluation()
{
    QModelIndex index = ui->tableViewEvaluations->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une évaluation");
        return;
    }
    
    int row = index.row();
    int evalId = model->item(row, 0)->text().toInt();
    
    Evaluation eval = Database::instance().getEvaluationById(evalId);
    EvaluationFormDialog dialog(eval, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        Evaluation updatedEval = dialog.getEvaluation();
        if (Database::instance().updateEvaluation(updatedEval)) {
            QMessageBox::information(this, "Succès", "Évaluation mise à jour");
            loadEvaluations();
        }
    }
}

void EvaluationListWindow::onDeleteEvaluation()
{
    QModelIndex index = ui->tableViewEvaluations->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une évaluation");
        return;
    }
    
    int row = index.row();
    int evalId = model->item(row, 0)->text().toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation", "Voulez-vous vraiment supprimer cette évaluation?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteEvaluation(evalId)) {
            QMessageBox::information(this, "Succès", "Évaluation supprimée");
            loadEvaluations();
        }
    }
}

void EvaluationListWindow::onExportPDF()
{
    QMessageBox::information(this, "Export PDF", "Fonctionnalité à implémenter");
}

void EvaluationListWindow::onSearch(const QString& text)
{
    for (int i = 0; i < model->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < model->columnCount(); ++j) {
            if (model->item(i, j)->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->tableViewEvaluations->setRowHidden(i, !match);
    }
}

void EvaluationListWindow::onFilterByStatus(int index)
{
    if (index == 0) { // Tous
        for (int i = 0; i < model->rowCount(); ++i) {
            ui->tableViewEvaluations->setRowHidden(i, false);
        }
    } else {
        QString status = (index == 1) ? "En cours" : "Terminée";
        for (int i = 0; i < model->rowCount(); ++i) {
            QString itemStatus = model->item(i, 5)->text();
            ui->tableViewEvaluations->setRowHidden(i, itemStatus != status);
        }
    }
}

void EvaluationListWindow::onSortByDate()
{
    model->sort(4);
}

void EvaluationListWindow::onSortByNote()
{
    model->sort(3);
}

void EvaluationListWindow::onRefresh()
{
    loadEvaluations();
}
