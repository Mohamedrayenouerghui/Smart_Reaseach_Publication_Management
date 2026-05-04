#ifndef ARDUINOMONITOR_H
#define ARDUINOMONITOR_H

#include <QWidget>
#include <QSerialPort>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class ArduinoMonitor;
}

// Structure pour une lecture du capteur
struct CapteurData {
    QDateTime dateHeure;
    double    temperature;
    double    humidite;
    double    variation;
    QString   etat;       // "Normal" ou "FLAMME DETECTEE"
};

class ArduinoMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit ArduinoMonitor(QWidget *parent = nullptr);
    ~ArduinoMonitor();

private slots:
    void onConnecterClicked();
    void onDeconnecterClicked();
    void onDonneesRecues();
    void onEffacerClicked();
    void onExporterPDFClicked();
    void onRefreshPortsClicked();

private:
    Ui::ArduinoMonitor *ui;
    QSerialPort *m_serial;

    double m_lastTemp     = 0.0;
    double m_lastHumidite = 0.0;
    double m_lastVariation= 0.0;
    QString m_lastEtat    = "Normal";
    QString m_buffer;
    bool m_alerteActive   = false;  // évite les popups répétés

    void parseLine(const QString &line);
    void updateDisplay();
    void saveToDatabase(const CapteurData &data);
    void alerteFlamme();
    void updateConferenceEtat(const QString &etat);
    void updateLaboratoireEtat(const QString &etat);
    void createTableIfNeeded();
    void loadHistory();
};

#endif // ARDUINOMONITOR_H
