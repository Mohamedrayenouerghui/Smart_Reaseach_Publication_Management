#ifndef ARDUINOPAGE_H
#define ARDUINOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>

namespace Ui {
class ArduinoPage;
}

class ArduinoManager;

class ArduinoPage : public QWidget
{
    Q_OBJECT

public:
    explicit ArduinoPage(QWidget *parent = nullptr);
    ~ArduinoPage();
    
    void setArduinoManager(ArduinoManager *manager);

private slots:
    void onArduinoStatusChanged(const QString &status);
    void onSoumissionAcceptee(int id);
    void onSoumissionRejetee(int id);
    void onRefreshClicked();
    void onTestConnectionClicked();
    void updateLCDPreview();

private:
    Ui::ArduinoPage *ui;
    ArduinoManager *arduinoManager;
    QTimer *lcdUpdateTimer;
    
    void setupUI();
    void loadPendingSubmissions();
    void updateConnectionStatus(bool connected);
    void addLogMessage(const QString &message, const QString &type = "info");
};

#endif // ARDUINOPAGE_H
