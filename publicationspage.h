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
    void onSpellCheckButtonClicked();
    void onPlagiarismButtonClicked();
    void onModifierButtonClicked();
    void onSupprimerButtonClicked();
    void onSearchTextChanged();

private:
    Ui::PublicationsPage *ui;
    int currentEditingArticleId;
    QString currentSortOrder;
    
    void setupConnections();
    void setupMenus();
    void setupTable();
    void loadArticlesFromDatabase();
    void exportTableToPDF();
};

#endif // PUBLICATIONSPAGE_H
