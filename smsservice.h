#ifndef SMSSERVICE_H
#define SMSSERVICE_H

#include <QWidget>

namespace Ui {
class SMSService;
}

class SMSService : public QWidget
{
    Q_OBJECT

public:
    explicit SMSService(QWidget *parent = nullptr);
    ~SMSService();

private slots:
    void onSendSMSClicked();
    void onBackClicked();
    void onClearClicked();

private:
    Ui::SMSService *ui;
    void setupConnections();
};

#endif // SMSSERVICE_H
