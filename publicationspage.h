#ifndef PUBLICATIONSPAGE_H
#define PUBLICATIONSPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSet>
#include <QJsonArray>

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
    void onSpellCheckReplyFinished(QNetworkReply *reply);
    void onPlagiarismButtonClicked();
    void onModifierButtonClicked();
    void onSupprimerButtonClicked();
    void onSearchTextChanged();

private:
    Ui::PublicationsPage *ui;
    int currentEditingArticleId;
    QString currentSortOrder;
    QNetworkAccessManager *networkManager;
    
    void setupConnections();
    void setupMenus();
    void setupTable();
    void loadArticlesFromDatabase();
    void exportTableToPDF();
    void showSpellCheckResults(const QJsonArray &matches, const QString &originalText);
    
    // Plagiarism helpers
    QSet<QString> buildNgrams(const QString &text, int n = 3);
    double ngramSimilarity(const QSet<QString> &a, const QSet<QString> &b);
};

#endif // PUBLICATIONSPAGE_H
