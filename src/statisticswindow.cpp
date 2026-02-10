#include "headers/statisticswindow.h"
#include "ui_statisticswindow.h"
#include "headers/database.h"
#include <QBarSet>
#include <QBarSeries>
#include <QValueAxis>
#include <QBarCategoryAxis>

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    
    setupCharts();
    updateStatisticsLabels();
    
    connect(ui->btnRefresh, &QPushButton::clicked, this, &StatisticsWindow::onRefresh);
    connect(ui->btnExport, &QPushButton::clicked, this, &StatisticsWindow::onExportStatistics);
}

StatisticsWindow::~StatisticsWindow()
{
    delete ui;
}

void StatisticsWindow::setupCharts()
{
    createNoteDistributionChart();
    createTimelineChart();
}

void StatisticsWindow::createNoteDistributionChart()
{
    QMap<int, int> distribution = Database::instance().getNoteDistribution();
    
    QBarSet *set = new QBarSet("Nombre d'évaluations");
    
    for (int i = 1; i <= 10; ++i) {
        *set << distribution.value(i, 0);
    }
    
    QBarSeries *series = new QBarSeries();
    series->append(set);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des notes");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    QStringList categories;
    for (int i = 1; i <= 10; ++i) {
        categories << QString::number(i);
    }
    
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Note");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre");
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    chart->legend()->setVisible(false);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    ui->chartWidgetDistribution->layout()->addWidget(chartView);
}

void StatisticsWindow::createTimelineChart()
{
    // Placeholder for timeline chart
    // Would need date-based aggregation
    QChart *chart = new QChart();
    chart->setTitle("Timeline des évaluations (à implémenter)");
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    ui->chartWidgetTimeline->layout()->addWidget(chartView);
}

void StatisticsWindow::updateStatisticsLabels()
{
    int total = Database::instance().getTotalEvaluations();
    int pending = Database::instance().getPendingEvaluations();
    int completed = Database::instance().getCompletedEvaluations();
    
    ui->lblTotalEvaluations->setText(QString("Total: %1").arg(total));
    ui->lblPendingEvaluations->setText(QString("En cours: %1").arg(pending));
    ui->lblCompletedEvaluations->setText(QString("Terminées: %1").arg(completed));
    
    // Average evaluation time (placeholder)
    ui->lblAverageTime->setText("Temps moyen: 45.5 min");
}

void StatisticsWindow::onExportStatistics()
{
    QMessageBox::information(this, "Export", "Fonctionnalité d'export à implémenter");
}

void StatisticsWindow::onRefresh()
{
    // Clear existing charts
    QLayoutItem *item;
    while ((item = ui->chartWidgetDistribution->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    while ((item = ui->chartWidgetTimeline->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    setupCharts();
    updateStatisticsLabels();
}
