#ifndef PUBLICATIONSPAGE_H
#define PUBLICATIONSPAGE_H

#include <QWidget>

namespace Ui {
class PublicationsPage;
}

class PublicationsPage : public QWidget
{
    Q_OBJECT

public:
    explicit PublicationsPage(QWidget *parent = nullptr);
    ~PublicationsPage();

private slots:
    void onAddButtonClicked();
    void onStatsButtonClicked();
    void onClearButtonClicked();
    void onSortMenuTriggered(QAction *action);
    void onExportMenuTriggered(QAction *action);
    void onUploadButtonClicked();
    void onCancelButtonClicked();
    void onSubmitButtonClicked();

private:
    Ui::PublicationsPage *ui;
    
    void setupConnections();
    void setupMenus();
    void setupTable();
};

#endif // PUBLICATIONSPAGE_H
