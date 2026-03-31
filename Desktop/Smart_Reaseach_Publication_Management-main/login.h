#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login; // Note: In your XML the class is LoginWindow
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    // --- ADD THIS SECTION ---
signals:
    void loginSuccessful();
    // ------------------------

private slots:
    void on_loginBtn_clicked(); // Make sure this slot exists

private:
    Ui::Login *ui;
    void setupConnections();
    void setupIcons();

};

#endif // LOGIN_H
