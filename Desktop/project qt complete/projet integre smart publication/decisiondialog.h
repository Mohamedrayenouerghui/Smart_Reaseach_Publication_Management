#ifndef DECISIONDIALOG_H
#define DECISIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

class ArduinoManager;

class DecisionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecisionDialog(int idSoumission, const QString &articleTitle, 
                           ArduinoManager *arduino, QWidget *parent = nullptr);
    ~DecisionDialog();
    
    QString getDecision() const { return decision; }
    bool wasDecisionMade() const { return decisionMade; }

private slots:
    void onDecisionAcceptee(int id);
    void onDecisionRejetee(int id);
    void onArduinoStatusChanged(const QString &status);
    void updateWaitingAnimation();

private:
    int soumissionId;
    QString articleTitle;
    QString decision;
    bool decisionMade;
    
    ArduinoManager *arduinoManager;
    
    // Widgets
    QLabel *titleLabel;
    QLabel *statusLabel;
    QLabel *waitingLabel;
    QPushButton *cancelBtn;
    
    QTimer *animationTimer;
    int animationStep;
    
    void setupUI();
    void connectSignals();
    void sendWaitingMessageToArduino();
};

#endif // DECISIONDIALOG_H
