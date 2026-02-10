#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { 
    class MainWindow;
}
QT_END_NAMESPACE

class PublicationsPage;
class Submission;
class Conference;
class Laboratoire;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void initUserPage();
    void onAccueilClicked();
    void onPublicationsClicked();
    void onUtilisateursClicked();
    void onSoumissionsClicked();
    void onEvaluationsClicked();
    void onConferencesClicked();
    void onLaboratoiresClicked();

private:
    Ui::MainWindow *ui;
    PublicationsPage *publicationsPage;
    Submission *submissionPage;
    Conference *conferencePage;
    Laboratoire *laboratoirePage;
    
    void loadStyleSheet();
    void setupConnections();
    void setupIcons();
    void setupPublicationsPage();
    void setupSubmissionPage();
    void setupConferencePage();
    void setupLaboratoirePage();
    void updateTitleUnderline(const QString &color);
};

#endif // MAINWINDOW_H
