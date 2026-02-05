#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QFrame>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void loadStyleSheet();
    QWidget *createSidebar();
    QWidget *createContentArea();
    QPushButton *createSidebarButton(const QString &text, const QString &iconPath = "");

    // UI Components
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;

    // Sidebar buttons
    QPushButton *accueilBtn;
    QPushButton *publicationsBtn;
    QPushButton *utilisateursBtn;
    QPushButton *soumissionsBtn;
    QPushButton *evaluationsBtn;
    QPushButton *conferencesBtn;
    QPushButton *laboratoiresBtn;

    // Content area components
    QWidget *headerWidget;
    QLabel *titleLabel;
    QLabel *logoLabel;
    QWidget *currentContentWidget;
    QTableWidget *upperTable;
    QTableWidget *lowerTable;
    QLineEdit *searchBar;
    QPushButton *pushButton;
    QPushButton *flatButton;

private slots:
    void onAccueilClicked();
    void onPublicationsClicked();
    void onUtilisateursClicked();
    void onSoumissionsClicked();
    void onEvaluationsClicked();
    void onConferencesClicked();
    void onLaboratoiresClicked();

private:
    QWidget *createWelcomePage();
    QWidget *createPublicationsPage();
    QWidget *createUsersPage();
    QWidget *createSubmissionsPage();
    QWidget *createEvaluationsPage();
    QWidget *createConferencesPage();
    QWidget *createLaboratoriesPage();
    void switchToPage(QWidget *newPage);
};

#endif // MAINWINDOW_H
