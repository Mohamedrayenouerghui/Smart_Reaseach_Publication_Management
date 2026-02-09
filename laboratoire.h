#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include <QWidget>

namespace Ui {
class Laboratoire;
}

class Laboratoire : public QWidget
{
    Q_OBJECT

public:
    explicit Laboratoire(QWidget *parent = nullptr);
    ~Laboratoire();

private slots:
    void onSortMenuTriggered(QAction *action);
    void onExportMenuTriggered(QAction *action);

private:
    Ui::Laboratoire *ui;
    
    void setupTable();
    void setupMenus();
    void setupConnections();
};

#endif // LABORATOIRE_H

