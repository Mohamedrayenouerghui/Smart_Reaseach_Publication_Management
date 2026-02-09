#include "submission.h"
#include "ui_submission.h"
#include <QDate>

Submission::Submission(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Submission)
    , selectedRow(-1)
{
    ui->setupUi(this);
    
    // Set current date
    ui->dateEdit->setDate(QDate::currentDate());
    
    // Load sample data
    loadSubmissions();
    
    // Connect signals - but slots do nothing (empty implementations)
    connect(ui->addBtn, &QPushButton::clicked, this, &Submission::onAddSubmission);
    connect(ui->updateBtn, &QPushButton::clicked, this, &Submission::onUpdateSubmission);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &Submission::onDeleteSubmission);
    connect(ui->clearBtn, &QPushButton::clicked, this, &Submission::clearForm);
    connect(ui->sortBtn, &QPushButton::clicked, this, &Submission::onSortClicked);
    connect(ui->searchBtn, &QPushButton::clicked, this, &Submission::onSearchClicked);
    connect(ui->exportBtn, &QPushButton::clicked, this, &Submission::onExportClicked);
    connect(ui->statsBtn, &QPushButton::clicked, this, &Submission::onStatisticsClicked);
    connect(ui->submissionTable, &QTableWidget::itemSelectionChanged, this, &Submission::onTableSelectionChanged);
}

Submission::~Submission()
{
    delete ui;
}

void Submission::loadSubmissions()
{
    // Sample data for demonstration
    ui->submissionTable->setRowCount(3);
    ui->submissionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Row 1
    ui->submissionTable->setItem(0, 0, new QTableWidgetItem("1"));
    ui->submissionTable->setItem(0, 1, new QTableWidgetItem("Intelligence Artificielle et Santé"));
    ui->submissionTable->setItem(0, 2, new QTableWidgetItem("Dr. Ahmed Ben Ali"));
    ui->submissionTable->setItem(0, 3, new QTableWidgetItem("IA & Médecine"));
    ui->submissionTable->setItem(0, 4, new QTableWidgetItem("2026-01-15"));
    ui->submissionTable->setItem(0, 5, new QTableWidgetItem("En révision"));
    ui->submissionTable->setItem(0, 6, new QTableWidgetItem("Prof. Sarah Martin"));
    
    // Row 2
    ui->submissionTable->setItem(1, 0, new QTableWidgetItem("2"));
    ui->submissionTable->setItem(1, 1, new QTableWidgetItem("Blockchain et Sécurité"));
    ui->submissionTable->setItem(1, 2, new QTableWidgetItem("Dr. Mohamed Khalil"));
    ui->submissionTable->setItem(1, 3, new QTableWidgetItem("Cybersécurité"));
    ui->submissionTable->setItem(1, 4, new QTableWidgetItem("2026-02-01"));
    ui->submissionTable->setItem(1, 5, new QTableWidgetItem("En attente"));
    ui->submissionTable->setItem(1, 6, new QTableWidgetItem(""));
    
    // Row 3
    ui->submissionTable->setItem(2, 0, new QTableWidgetItem("3"));
    ui->submissionTable->setItem(2, 1, new QTableWidgetItem("Énergies Renouvelables"));
    ui->submissionTable->setItem(2, 2, new QTableWidgetItem("Dr. Fatima Zahra"));
    ui->submissionTable->setItem(2, 3, new QTableWidgetItem("Environnement"));
    ui->submissionTable->setItem(2, 4, new QTableWidgetItem("2026-01-20"));
    ui->submissionTable->setItem(2, 5, new QTableWidgetItem("Accepté"));
    ui->submissionTable->setItem(2, 6, new QTableWidgetItem("Prof. Jean Dupont"));
    
    ui->submissionTable->resizeColumnsToContents();
}
