/********************************************************************************
** Form generated from reading UI file 'laboratoire.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABORATOIRE_H
#define UI_LABORATOIRE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Laboratoire
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *listeTab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *toolbarLayout;
    QPushButton *ajouterButton;
    QPushButton *historiqueButton;
    QPushButton *modifierButton;
    QPushButton *trierButton;
    QPushButton *statistiquesButton;
    QPushButton *exporterPDFButton;
    QPushButton *notificationButton;
    QPushButton *supprimerButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *searchLayout;
    QComboBox *searchComboBox;
    QLineEdit *searchLineEdit;
    QPushButton *rechercherButton;
    QPushButton *clearSearchButton;
    QTableWidget *laboratoiresTable;
    QWidget *ajouterTab;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout;
    QLabel *nomLabel;
    QLineEdit *nomLineEdit;
    QLabel *budgetLabel;
    QDoubleSpinBox *budgetSpinBox;
    QLabel *dateCreationLabel;
    QDateEdit *dateCreationEdit;
    QLabel *domaineLabel;
    QComboBox *domaineComboBox;
    QLabel *localisationLabel;
    QLineEdit *localisationLineEdit;
    QLabel *nbEmployesLabel;
    QSpinBox *nbEmployesSpinBox;
    QLabel *classementLabel;
    QComboBox *classementComboBox;
    QHBoxLayout *buttonLayout;
    QPushButton *validerAjoutButton;
    QPushButton *annulerButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *historiqueTab;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *historiqueTable;
    QHBoxLayout *historiqueButtonLayout;
    QPushButton *supprimerHistoriqueButton;
    QPushButton *exporterHistoriqueButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *Laboratoire)
    {
        if (Laboratoire->objectName().isEmpty())
            Laboratoire->setObjectName("Laboratoire");
        Laboratoire->resize(1200, 800);
        verticalLayout = new QVBoxLayout(Laboratoire);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(Laboratoire);
        tabWidget->setObjectName("tabWidget");

        // ── Liste Tab ──
        listeTab = new QWidget();
        listeTab->setObjectName("listeTab");
        verticalLayout_2 = new QVBoxLayout(listeTab);
        verticalLayout_2->setObjectName("verticalLayout_2");

        toolbarLayout = new QHBoxLayout();
        toolbarLayout->setObjectName("toolbarLayout");
        ajouterButton = new QPushButton(listeTab); ajouterButton->setObjectName("ajouterButton"); toolbarLayout->addWidget(ajouterButton);
        historiqueButton = new QPushButton(listeTab); historiqueButton->setObjectName("historiqueButton"); toolbarLayout->addWidget(historiqueButton);
        modifierButton = new QPushButton(listeTab); modifierButton->setObjectName("modifierButton"); toolbarLayout->addWidget(modifierButton);
        trierButton = new QPushButton(listeTab); trierButton->setObjectName("trierButton"); toolbarLayout->addWidget(trierButton);
        statistiquesButton = new QPushButton(listeTab); statistiquesButton->setObjectName("statistiquesButton"); toolbarLayout->addWidget(statistiquesButton);
        exporterPDFButton = new QPushButton(listeTab); exporterPDFButton->setObjectName("exporterPDFButton"); toolbarLayout->addWidget(exporterPDFButton);
        notificationButton = new QPushButton(listeTab); notificationButton->setObjectName("notificationButton"); toolbarLayout->addWidget(notificationButton);
        supprimerButton = new QPushButton(listeTab); supprimerButton->setObjectName("supprimerButton"); toolbarLayout->addWidget(supprimerButton);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        toolbarLayout->addItem(horizontalSpacer);
        verticalLayout_2->addLayout(toolbarLayout);

        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName("searchLayout");
        searchComboBox = new QComboBox(listeTab);
        searchComboBox->addItem(QString());
        searchComboBox->addItem(QString());
        searchComboBox->addItem(QString());
        searchComboBox->addItem(QString());
        searchComboBox->setObjectName("searchComboBox");
        searchLayout->addWidget(searchComboBox);
        searchLineEdit = new QLineEdit(listeTab); searchLineEdit->setObjectName("searchLineEdit"); searchLayout->addWidget(searchLineEdit);
        rechercherButton = new QPushButton(listeTab); rechercherButton->setObjectName("rechercherButton"); searchLayout->addWidget(rechercherButton);
        clearSearchButton = new QPushButton(listeTab); clearSearchButton->setObjectName("clearSearchButton"); searchLayout->addWidget(clearSearchButton);
        verticalLayout_2->addLayout(searchLayout);

        laboratoiresTable = new QTableWidget(listeTab);
        if (laboratoiresTable->columnCount() < 8) laboratoiresTable->setColumnCount(8);
        for (int i = 0; i < 8; ++i) laboratoiresTable->setHorizontalHeaderItem(i, new QTableWidgetItem());
        laboratoiresTable->setObjectName("laboratoiresTable");
        laboratoiresTable->setColumnCount(8);
        verticalLayout_2->addWidget(laboratoiresTable);
        tabWidget->addTab(listeTab, QString());

        // ── Ajouter Tab ──
        ajouterTab = new QWidget();
        ajouterTab->setObjectName("ajouterTab");
        verticalLayout_3 = new QVBoxLayout(ajouterTab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        scrollArea = new QScrollArea(ajouterTab);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1172, 729));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName("verticalLayout_4");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");

        nomLabel = new QLabel(scrollAreaWidgetContents); nomLabel->setObjectName("nomLabel");
        formLayout->setWidget(0, QFormLayout::LabelRole, nomLabel);
        nomLineEdit = new QLineEdit(scrollAreaWidgetContents); nomLineEdit->setObjectName("nomLineEdit");
        formLayout->setWidget(0, QFormLayout::FieldRole, nomLineEdit);

        budgetLabel = new QLabel(scrollAreaWidgetContents); budgetLabel->setObjectName("budgetLabel");
        formLayout->setWidget(1, QFormLayout::LabelRole, budgetLabel);
        budgetSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents); budgetSpinBox->setObjectName("budgetSpinBox");
        budgetSpinBox->setMaximum(999999999.99);
        formLayout->setWidget(1, QFormLayout::FieldRole, budgetSpinBox);

        dateCreationLabel = new QLabel(scrollAreaWidgetContents); dateCreationLabel->setObjectName("dateCreationLabel");
        formLayout->setWidget(2, QFormLayout::LabelRole, dateCreationLabel);
        dateCreationEdit = new QDateEdit(scrollAreaWidgetContents); dateCreationEdit->setObjectName("dateCreationEdit");
        dateCreationEdit->setCalendarPopup(true);
        formLayout->setWidget(2, QFormLayout::FieldRole, dateCreationEdit);

        domaineLabel = new QLabel(scrollAreaWidgetContents); domaineLabel->setObjectName("domaineLabel");
        formLayout->setWidget(3, QFormLayout::LabelRole, domaineLabel);
        domaineComboBox = new QComboBox(scrollAreaWidgetContents); domaineComboBox->setObjectName("domaineComboBox");
        for (int i = 0; i < 10; ++i) domaineComboBox->addItem(QString());
        formLayout->setWidget(3, QFormLayout::FieldRole, domaineComboBox);

        localisationLabel = new QLabel(scrollAreaWidgetContents); localisationLabel->setObjectName("localisationLabel");
        formLayout->setWidget(4, QFormLayout::LabelRole, localisationLabel);
        localisationLineEdit = new QLineEdit(scrollAreaWidgetContents); localisationLineEdit->setObjectName("localisationLineEdit");
        formLayout->setWidget(4, QFormLayout::FieldRole, localisationLineEdit);

        nbEmployesLabel = new QLabel(scrollAreaWidgetContents); nbEmployesLabel->setObjectName("nbEmployesLabel");
        formLayout->setWidget(5, QFormLayout::LabelRole, nbEmployesLabel);
        nbEmployesSpinBox = new QSpinBox(scrollAreaWidgetContents); nbEmployesSpinBox->setObjectName("nbEmployesSpinBox");
        nbEmployesSpinBox->setMaximum(10000);
        formLayout->setWidget(5, QFormLayout::FieldRole, nbEmployesSpinBox);

        classementLabel = new QLabel(scrollAreaWidgetContents); classementLabel->setObjectName("classementLabel");
        formLayout->setWidget(6, QFormLayout::LabelRole, classementLabel);
        classementComboBox = new QComboBox(scrollAreaWidgetContents); classementComboBox->setObjectName("classementComboBox");
        for (int i = 0; i < 6; ++i) classementComboBox->addItem(QString());
        formLayout->setWidget(6, QFormLayout::FieldRole, classementComboBox);

        buttonLayout = new QHBoxLayout(); buttonLayout->setObjectName("buttonLayout");
        validerAjoutButton = new QPushButton(scrollAreaWidgetContents); validerAjoutButton->setObjectName("validerAjoutButton");
        buttonLayout->addWidget(validerAjoutButton);
        annulerButton = new QPushButton(scrollAreaWidgetContents); annulerButton->setObjectName("annulerButton");
        buttonLayout->addWidget(annulerButton);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        buttonLayout->addItem(horizontalSpacer_2);
        formLayout->setLayout(7, QFormLayout::FieldRole, buttonLayout);

        verticalLayout_4->addLayout(formLayout);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        verticalLayout_4->addItem(verticalSpacer);
        scrollArea->setWidget(scrollAreaWidgetContents);
        verticalLayout_3->addWidget(scrollArea);
        tabWidget->addTab(ajouterTab, QString());

        // ── Historique Tab ──
        historiqueTab = new QWidget();
        historiqueTab->setObjectName("historiqueTab");
        verticalLayout_5 = new QVBoxLayout(historiqueTab);
        verticalLayout_5->setObjectName("verticalLayout_5");
        historiqueTable = new QTableWidget(historiqueTab);
        if (historiqueTable->columnCount() < 5) historiqueTable->setColumnCount(5);
        for (int i = 0; i < 5; ++i) historiqueTable->setHorizontalHeaderItem(i, new QTableWidgetItem());
        historiqueTable->setObjectName("historiqueTable");
        historiqueTable->setColumnCount(5);
        verticalLayout_5->addWidget(historiqueTable);

        historiqueButtonLayout = new QHBoxLayout(); historiqueButtonLayout->setObjectName("historiqueButtonLayout");
        supprimerHistoriqueButton = new QPushButton(historiqueTab); supprimerHistoriqueButton->setObjectName("supprimerHistoriqueButton");
        historiqueButtonLayout->addWidget(supprimerHistoriqueButton);
        exporterHistoriqueButton = new QPushButton(historiqueTab); exporterHistoriqueButton->setObjectName("exporterHistoriqueButton");
        historiqueButtonLayout->addWidget(exporterHistoriqueButton);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        historiqueButtonLayout->addItem(horizontalSpacer_3);
        verticalLayout_5->addLayout(historiqueButtonLayout);
        tabWidget->addTab(historiqueTab, QString());

        verticalLayout->addWidget(tabWidget);
        retranslateUi(Laboratoire);
        tabWidget->setCurrentIndex(0);
        QMetaObject::connectSlotsByName(Laboratoire);
    }

    void retranslateUi(QWidget *Laboratoire)
    {
        Laboratoire->setWindowTitle(QCoreApplication::translate("Laboratoire", "Laboratoire", nullptr));
        Laboratoire->setStyleSheet(QCoreApplication::translate("Laboratoire",
            "QWidget { background-color: #f8f9fa; font-family: 'Segoe UI', Arial, sans-serif; }\n"
            "QTabWidget::pane { border: 1px solid #e5e7eb; background-color: white; border-radius: 8px; }\n"
            "QTabBar::tab { background-color: #f1f3f4; color: #6b7280; padding: 12px 24px; margin-right: 2px; border-top-left-radius: 8px; border-top-right-radius: 8px; font-weight: 500; font-size: 14px; }\n"
            "QTabBar::tab:selected { background-color: #8B5CF6; color: white; }\n"
            "QTabBar::tab:hover:!selected { background-color: #E5E7EB; }\n"
            "QTableWidget { background-color: white; border: 1px solid #E5E7EB; border-radius: 8px; gridline-color: #E5E7EB; }\n"
            "QTableWidget::item { padding: 8px; color: #374151; }\n"
            "QTableWidget::item:selected { background-color: #EDE9FE; color: #8B5CF6; }\n"
            "QHeaderView::section { background-color: #F9FAFB; color: #6B7280; padding: 12px; border: none; border-bottom: 2px solid #E5E7EB; font-weight: 600; font-size: 13px; }\n"
            "QLineEdit, QTextEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox { padding: 10px 12px; border: 1px solid #D1D5DB; border-radius: 6px; background-color: white; color: #374151; font-size: 14px; }\n"
            "QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #8B5CF6; outline: none; }\n"
            "QComboBox::drop-down { border: none; padding-right: 10px; }\n"
            "QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid #6B7280; margin-right: 5px; }\n"
            "QLabel { color: #374151; font-size: 14px; font-weight: 500; }\n"
            "QPushButton { background-color: transparent; border-radius: 8px; font-family: 'Segoe UI', sans-serif; font-size: 13px; font-weight: 600; padding: 8px 20px; min-height: 40px; }\n"
            "QPushButton#ajouterButton, QPushButton#validerAjoutButton { color: #10B981; border: 2px solid #10B981; }\n"
            "QPushButton#ajouterButton:hover, QPushButton#validerAjoutButton:hover { background-color: #10B981; color: white; }\n"
            "QPushButton#annulerButton, QPushButton#supprimerButton, QPushButton#supprimerHistoriqueButton { color: #EF4444; border: 2px solid #EF4444; }\n"
            "QPushButton#annulerButton:hover, QPushButton#supprimerButton:hover, QPushButton#supprimerHistoriqueButton:hover { background-color: #EF4444; color: white; }\n"
            "QPushButton#historiqueButton, QPushButton#trierButton, QPushButton#exporterHistoriqueButton { color: #8B5CF6; border: 2px solid #8B5CF6; }\n"
            "QPushButton#historiqueButton:hover, QPushButton#trierButton:hover, QPushButton#exporterHistoriqueButton:hover { background-color: #8B5CF6; color: white; }\n"
            "QPushButton#modifierButton, QPushButton#rechercherButton { color: #3B82F6; border: 2px solid #3B82F6; }\n"
            "QPushButton#modifierButton:hover, QPushButton#rechercherButton:hover { background-color: #3B82F6; color: white; }\n"
            "QPushButton#exporterPDFButton, QPushButton#notificationButton { color: #F59E0B; border: 2px solid #F59E0B; }\n"
            "QPushButton#exporterPDFButton:hover, QPushButton#notificationButton:hover { background-color: #F59E0B; color: white; }\n"
            "QPushButton#statistiquesButton { color: #14B8A6; border: 2px solid #14B8A6; }\n"
            "QPushButton#statistiquesButton:hover { background-color: #14B8A6; color: white; }\n"
            "QPushButton#clearSearchButton { color: #64748B; border: 2px solid #E2E8F0; }\n"
            "QPushButton#clearSearchButton:hover { background-color: #64748B; color: white; }\n",
            nullptr));

        ajouterButton->setText(QCoreApplication::translate("Laboratoire", "\342\236\225 Ajouter", nullptr));
        historiqueButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\223\232 Historique", nullptr));
        modifierButton->setText(QCoreApplication::translate("Laboratoire", "\342\234\217\357\270\217 Modifier", nullptr));
        trierButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\224\204 Trier", nullptr));
        statistiquesButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\223\210 Statistiques", nullptr));
        exporterPDFButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\223\204 Exporter PDF", nullptr));
        notificationButton->setToolTip(QCoreApplication::translate("Laboratoire", "Voir les notifications", nullptr));
        notificationButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\224\224", nullptr));
        supprimerButton->setText(QCoreApplication::translate("Laboratoire", "\342\235\214 Supprimer", nullptr));

        searchComboBox->setItemText(0, QCoreApplication::translate("Laboratoire", "Rechercher par Nom", nullptr));
        searchComboBox->setItemText(1, QCoreApplication::translate("Laboratoire", "Rechercher par Domaine", nullptr));
        searchComboBox->setItemText(2, QCoreApplication::translate("Laboratoire", "Rechercher par Localisation", nullptr));
        searchComboBox->setItemText(3, QCoreApplication::translate("Laboratoire", "Rechercher par Budget", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("Laboratoire", "Rechercher un laboratoire...", nullptr));
        rechercherButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\224\215 Rechercher", nullptr));
        clearSearchButton->setText(QCoreApplication::translate("Laboratoire", "\342\234\226 Effacer", nullptr));

        laboratoiresTable->horizontalHeaderItem(0)->setText(QCoreApplication::translate("Laboratoire", "ID", nullptr));
        laboratoiresTable->horizontalHeaderItem(1)->setText(QCoreApplication::translate("Laboratoire", "Nom", nullptr));
        laboratoiresTable->horizontalHeaderItem(2)->setText(QCoreApplication::translate("Laboratoire", "Budget", nullptr));
        laboratoiresTable->horizontalHeaderItem(3)->setText(QCoreApplication::translate("Laboratoire", "Date Cr\303\251ation", nullptr));
        laboratoiresTable->horizontalHeaderItem(4)->setText(QCoreApplication::translate("Laboratoire", "Domaine", nullptr));
        laboratoiresTable->horizontalHeaderItem(5)->setText(QCoreApplication::translate("Laboratoire", "Localisation", nullptr));
        laboratoiresTable->horizontalHeaderItem(6)->setText(QCoreApplication::translate("Laboratoire", "NB Employ\303\251s", nullptr));
        laboratoiresTable->horizontalHeaderItem(7)->setText(QCoreApplication::translate("Laboratoire", "Classement", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(listeTab), QCoreApplication::translate("Laboratoire", "Liste des Laboratoires", nullptr));

        nomLabel->setText(QCoreApplication::translate("Laboratoire", "Nom du Laboratoire:", nullptr));
        nomLineEdit->setPlaceholderText(QCoreApplication::translate("Laboratoire", "Entrez le nom du laboratoire", nullptr));
        budgetLabel->setText(QCoreApplication::translate("Laboratoire", "Budget (DT):", nullptr));
        dateCreationLabel->setText(QCoreApplication::translate("Laboratoire", "Date de Cr\303\251ation:", nullptr));
        domaineLabel->setText(QCoreApplication::translate("Laboratoire", "Domaine:", nullptr));
        domaineComboBox->setItemText(0, QCoreApplication::translate("Laboratoire", "Intelligence Artificielle", nullptr));
        domaineComboBox->setItemText(1, QCoreApplication::translate("Laboratoire", "Cybers\303\251curit\303\251", nullptr));
        domaineComboBox->setItemText(2, QCoreApplication::translate("Laboratoire", "Big Data", nullptr));
        domaineComboBox->setItemText(3, QCoreApplication::translate("Laboratoire", "Cloud Computing", nullptr));
        domaineComboBox->setItemText(4, QCoreApplication::translate("Laboratoire", "IoT", nullptr));
        domaineComboBox->setItemText(5, QCoreApplication::translate("Laboratoire", "Blockchain", nullptr));
        domaineComboBox->setItemText(6, QCoreApplication::translate("Laboratoire", "R\303\251seaux", nullptr));
        domaineComboBox->setItemText(7, QCoreApplication::translate("Laboratoire", "D\303\251veloppement Logiciel", nullptr));
        domaineComboBox->setItemText(8, QCoreApplication::translate("Laboratoire", "Robotique", nullptr));
        domaineComboBox->setItemText(9, QCoreApplication::translate("Laboratoire", "Autre", nullptr));
        localisationLabel->setText(QCoreApplication::translate("Laboratoire", "Localisation:", nullptr));
        localisationLineEdit->setPlaceholderText(QCoreApplication::translate("Laboratoire", "Entrez la localisation", nullptr));
        nbEmployesLabel->setText(QCoreApplication::translate("Laboratoire", "Nombre d'Employ\303\251s:", nullptr));
        classementLabel->setText(QCoreApplication::translate("Laboratoire", "Classement:", nullptr));
        classementComboBox->setItemText(0, QCoreApplication::translate("Laboratoire", "A+", nullptr));
        classementComboBox->setItemText(1, QCoreApplication::translate("Laboratoire", "A", nullptr));
        classementComboBox->setItemText(2, QCoreApplication::translate("Laboratoire", "B+", nullptr));
        classementComboBox->setItemText(3, QCoreApplication::translate("Laboratoire", "B", nullptr));
        classementComboBox->setItemText(4, QCoreApplication::translate("Laboratoire", "C", nullptr));
        classementComboBox->setItemText(5, QCoreApplication::translate("Laboratoire", "Non Class\303\251", nullptr));
        validerAjoutButton->setText(QCoreApplication::translate("Laboratoire", "+ Ajouter", nullptr));
        annulerButton->setText(QCoreApplication::translate("Laboratoire", "\342\234\226 Annuler", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ajouterTab), QCoreApplication::translate("Laboratoire", "Ajouter un Laboratoire", nullptr));

        historiqueTable->horizontalHeaderItem(0)->setText(QCoreApplication::translate("Laboratoire", "Date", nullptr));
        historiqueTable->horizontalHeaderItem(1)->setText(QCoreApplication::translate("Laboratoire", "Laboratoire", nullptr));
        historiqueTable->horizontalHeaderItem(2)->setText(QCoreApplication::translate("Laboratoire", "Type de Modification", nullptr));
        historiqueTable->horizontalHeaderItem(3)->setText(QCoreApplication::translate("Laboratoire", "Utilisateur", nullptr));
        historiqueTable->horizontalHeaderItem(4)->setText(QCoreApplication::translate("Laboratoire", "D\303\251tails", nullptr));
        supprimerHistoriqueButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        exporterHistoriqueButton->setText(QCoreApplication::translate("Laboratoire", "\360\237\223\204 Exporter en PDF", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(historiqueTab), QCoreApplication::translate("Laboratoire", "Historique de Modification", nullptr));
    }
};

namespace Ui {
class Laboratoire : public Ui_Laboratoire {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABORATOIRE_H
