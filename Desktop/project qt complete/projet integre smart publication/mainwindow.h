#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { 
    class MainWindow;
}
QT_END_NAMESPACE

class PublicationsPage;
class Submission;
class Conference;
class Laboratoire;
class Evaluation;
class ArduinoManager;
class ArduinoPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    void setRelecteurMode(bool isRelecteur);

private slots:
    void initUserPage();
    void onAccueilClicked();
    void onPublicationsClicked();
    void onUtilisateursClicked();
    void onSoumissionsClicked();
    void onEvaluationsClicked();
    void onConferencesClicked();
    void onLaboratoiresClicked();
    void onLogoutClicked();
    void showArduinoInterface();

private:
    Ui::MainWindow *ui;
    PublicationsPage *publicationsPage;
    Submission *submissionPage;
    Conference *conferencePage;
    Laboratoire *laboratoirePage;
    Evaluation *evaluationPage;
    ArduinoPage *arduinoPage;
    ArduinoManager *arduinoManager;
    QLabel *arduinoStatusLabel;
    
    void loadStyleSheet();
    void setupConnections();
    void setupIcons();
    void setupPublicationsPage();
    void setupSubmissionPage();
    void setupEvaluationPage();
    void setupConferencePage();
    void setupLaboratoirePage();
    void setupArduinoPage();
    void updateTitleUnderline(const QString &color);
    void setupLogoutButton();
    void setupArduino();
    void updateArduinoStatus(const QString &status);};

#endif // MAINWINDOW_H
