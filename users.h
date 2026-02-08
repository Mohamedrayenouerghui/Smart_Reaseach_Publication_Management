#ifndef USERS_H
#define USERS_H

#include <QWidget>

namespace Ui {
class Users;
}

class Users : public QWidget
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = nullptr);
    ~Users();

private:
    Ui::Users *ui;
    void setupTable();
    void setupMenus();
    void setupConnections();
};

#endif // USERSPAGE_H
