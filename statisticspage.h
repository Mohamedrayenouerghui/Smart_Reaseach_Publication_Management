#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
namespace Ui { class StatisticsPage; }
QT_END_NAMESPACE

class StatisticsPage : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

private:
    Ui::StatisticsPage *ui;
    
    void loadStatistics();
    void createDomaineChart();
    void createDateChart();
};

#endif // STATISTICSPAGE_H
