#include "statisticspage.h"
#include "ui_statisticspage.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QVBoxLayout>
#include <QLabel>
#include <QDate>

StatisticsPage::StatisticsPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    setWindowTitle("Statistiques des Publications");
    resize(1200, 900);
    
    loadStatistics();
}

StatisticsPage::~StatisticsPage()
{
    delete ui;
}

void StatisticsPage::loadStatistics()
{
    createDomaineChart();
    createDateChart();
}

void StatisticsPage::createDomaineChart()
{
    // Requête pour compter les articles par domaine
    QSqlQuery query;
    query.prepare("SELECT domaine, COUNT(*) as count FROM ARTICLE GROUP BY domaine ORDER BY count DESC");
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des statistiques par domaine.");
        return;
    }
    
    // Créer la série de données
    QPieSeries *series = new QPieSeries();
    
    // Couleurs personnalisées
    QStringList colors = {"#8B5CF6", "#3B82F6", "#10B981", "#F59E0B", "#EF4444", "#EC4899", "#6366F1"};
    int colorIndex = 0;
    
    // Stocker les données temporairement
    QList<QPair<QString, int>> data;
    int totalArticles = 0;
    
    while (query.next()) {
        QString domaine = query.value(0).toString();
        int count = query.value(1).toInt();
        data.append(qMakePair(domaine, count));
        totalArticles += count;
    }
    
    // Si pas de données
    if (totalArticles == 0) {
        QLabel *noDataLabel = new QLabel("Aucune donnée disponible");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setStyleSheet("font-size: 16px; color: #64748B;");
        ui->domaineChartLayout->addWidget(noDataLabel);
        return;
    }
    
    // Créer les slices avec les pourcentages corrects
    for (const auto &item : data) {
        QString domaine = item.first;
        int count = item.second;
        
        QPieSlice *slice = series->append(domaine, count);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2 (%3%)")
                        .arg(domaine)
                        .arg(count)
                        .arg(100.0 * count / totalArticles, 0, 'f', 1));
        
        // Appliquer la couleur
        if (colorIndex < colors.size()) {
            slice->setBrush(QColor(colors[colorIndex]));
            colorIndex++;
        }
        
        // Effet hover
        slice->setExplodeDistanceFactor(0.05);
        connect(slice, &QPieSlice::hovered, [slice](bool show) {
            slice->setExploded(show);
        });
    }
    
    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Répartition par Domaine (%1 articles)").arg(totalArticles));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    // Style du graphique
    QFont titleFont;
    titleFont.setPixelSize(20);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);
    chart->setBackgroundBrush(QBrush(QColor("#FFFFFF")));
    chart->setMargins(QMargins(10, 10, 10, 10));
    
    // Augmenter la taille de la légende
    QFont legendFont;
    legendFont.setPixelSize(12);
    chart->legend()->setFont(legendFont);
    
    // Créer la vue
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300);
    
    // Ajouter au layout
    ui->domaineChartLayout->addWidget(chartView);
}

void StatisticsPage::createDateChart()
{
    // Requête pour compter les articles par période
    QSqlQuery query;
    query.prepare("SELECT Date_creation FROM ARTICLE ORDER BY Date_creation");
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des statistiques par date.");
        return;
    }
    
    // Compter par période (année-mois ou année selon la distribution)
    QMap<QString, int> periodCount;
    
    while (query.next()) {
        QDate date = query.value(0).toDate();
        if (date.isValid()) {
            // Grouper par mois et année
            QString period = date.toString("MMM yyyy");
            periodCount[period]++;
        }
    }
    
    // Si pas de données
    if (periodCount.isEmpty()) {
        QLabel *noDataLabel = new QLabel("Aucune donnée disponible");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setStyleSheet("font-size: 16px; color: #64748B;");
        ui->dateChartLayout->addWidget(noDataLabel);
        return;
    }
    
    // Créer la série de données
    QPieSeries *series = new QPieSeries();
    
    // Couleurs pour les périodes
    QStringList colors = {"#8B5CF6", "#3B82F6", "#10B981", "#F59E0B", "#EF4444", "#EC4899", "#6366F1", "#14B8A6"};
    int colorIndex = 0;
    
    int totalArticles = 0;
    for (auto it = periodCount.begin(); it != periodCount.end(); ++it) {
        totalArticles += it.value();
    }
    
    for (auto it = periodCount.begin(); it != periodCount.end(); ++it) {
        QString period = it.key();
        int count = it.value();
        
        QPieSlice *slice = series->append(period, count);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2 (%3%)")
                        .arg(period)
                        .arg(count)
                        .arg(100.0 * count / totalArticles, 0, 'f', 1));
        
        // Appliquer la couleur
        if (colorIndex < colors.size()) {
            slice->setBrush(QColor(colors[colorIndex]));
            colorIndex++;
        } else {
            colorIndex = 0;
            slice->setBrush(QColor(colors[colorIndex]));
        }
        
        // Effet hover
        slice->setExplodeDistanceFactor(0.05);
        connect(slice, &QPieSlice::hovered, [slice](bool show) {
            slice->setExploded(show);
        });
    }
    
    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Répartition par Date de Création (%1 articles)").arg(totalArticles));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    // Style du graphique
    QFont titleFont;
    titleFont.setPixelSize(20);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);
    chart->setBackgroundBrush(QBrush(QColor("#FFFFFF")));
    chart->setMargins(QMargins(10, 10, 10, 10));
    
    // Augmenter la taille de la légende
    QFont legendFont;
    legendFont.setPixelSize(12);
    chart->legend()->setFont(legendFont);
    
    // Créer la vue
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300);
    
    // Ajouter au layout
    ui->dateChartLayout->addWidget(chartView);
}
