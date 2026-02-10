#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QMainWindow>
#include <QtCharts>

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget *parent = nullptr);
    ~StatisticsWindow();

private slots:
    void onExportStatistics();
    void onRefresh();

private:
    Ui::StatisticsWindow *ui;
    
    void setupCharts();
    void createNoteDistributionChart();
    void createTimelineChart();
    void updateStatisticsLabels();
};

#endif // STATISTICSWINDOW_H
