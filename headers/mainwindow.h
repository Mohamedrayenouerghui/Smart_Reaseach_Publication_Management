#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNouvelleEvaluation();
    void onMesEvaluations();
    void onGestionRelecture();
    void onStatistiques();
    void onExportPDF();
    void onRefreshDashboard();

private:
    Ui::MainWindow *ui;
    
    void setupConnections();
    void updateStatistics();
    void loadRecentEvaluations();
    void createStatisticsCards();
};

#endif // MAINWINDOW_H
