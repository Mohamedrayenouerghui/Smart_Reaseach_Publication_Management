#include "laboratoire.h"
#include "ui_laboratoire.h"
#include "connection.h"

#include <QDebug>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QFileDialog>
#include <QCursor>
#include <QMenu>
#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>
#include <QTextEdit>
#include <QScrollArea>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

// ============================================================
//  Helper: open DB or log
// ============================================================
static QSqlDatabase labOpenDb()
{
    QSqlDatabase db = Connection::createInstance().getDb();
    if (!db.isOpen()) {
        qDebug() << "[Laboratoire] Base de donnees non connectee";
    }
    return db;
}

// ============================================================
//  Helper: confirm dialog with styled buttons
// ============================================================
static int labConfirmDialog(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(
        "QPushButton { background-color: #6B7280; color: white; border: none; "
        "border-radius: 6px; padding: 8px 20px; min-width: 80px; font-size: 13px; }"
        "QPushButton:hover { background-color: #4B5563; }"
    );
    return msgBox.exec();
}

// ============================================================
//  Helper: convert LaboratoireModel -> LaboratoireData
// ============================================================
static LaboratoireData modelToData(const LaboratoireModel &m)
{
    LaboratoireData d;
    d.id           = m.getId();
    d.nom          = m.getNom();
    d.budget       = m.getBudget();
    d.dateCreation = m.getDateCreation();
    d.domaine      = m.getDomaine();
    d.localisation = m.getLocalisation();
    d.nbEmployes   = m.getNbEmployes();
    d.classement   = m.getClassement();
    return d;
}

// ============================================================
//  LaboratoireModel â€“ Constructors
// ============================================================
LaboratoireModel::LaboratoireModel()
    : m_id(0), m_budget(0.0), m_nbEmployes(0)
{
}

LaboratoireModel::LaboratoireModel(int id, const QString &nom, double budget,
                                   const QDate &dateCreation, const QString &domaine,
                                   const QString &localisation, int nbEmployes,
                                   const QString &classement, const QString &historique)
    : m_id(id), m_nom(nom), m_budget(budget), m_dateCreation(dateCreation),
      m_domaine(domaine), m_localisation(localisation),
      m_nbEmployes(nbEmployes), m_classement(classement), m_historique(historique)
{
}

// ============================================================
//  LaboratoireModel â€“ valider
// ============================================================
bool LaboratoireModel::valider(QString &erreur) const
{
    if (m_nom.trimmed().isEmpty()) {
        erreur = "Le nom du laboratoire est obligatoire.";
        return false;
    }
    if (m_budget < 0) {
        erreur = "Le budget ne peut pas etre negatif.";
        return false;
    }
    if (!m_dateCreation.isValid()) {
        erreur = "La date de creation est invalide.";
        return false;
    }
    if (m_domaine.trimmed().isEmpty()) {
        erreur = "Le domaine est obligatoire.";
        return false;
    }
    if (m_localisation.trimmed().isEmpty()) {
        erreur = "La localisation est obligatoire.";
        return false;
    }
    if (m_nbEmployes < 0) {
        erreur = "Le nombre d'employes ne peut pas etre negatif.";
        return false;
    }
    if (m_classement.trimmed().isEmpty()) {
        erreur = "Le classement est obligatoire.";
        return false;
    }
    return true;
}

// ============================================================
//  LaboratoireModel â€“ ajouter
// ============================================================
bool LaboratoireModel::ajouter()
{
    QString erreur;
    if (!valider(erreur)) {
        qDebug() << "[Laboratoire] Validation echouee:" << erreur;
        return false;
    }

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return false;

    QString dateStr  = m_dateCreation.toString("yyyy-MM-dd");
    QString budgetSql = QString::number(m_budget, 'f', 2);

    // Primary attempt: with SEQ_LABO.NEXTVAL
    QString sql = QString(
        "INSERT INTO LABORATOIRE (ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT) "
        "VALUES (SEQ_LABO.NEXTVAL, :nom, %1, TO_DATE('%2', 'YYYY-MM-DD'), :domaine, :localisation, :nbEmployes, :classement)"
    ).arg(budgetSql, dateStr);

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":nom",          m_nom);
    query.bindValue(":domaine",      m_domaine);
    query.bindValue(":localisation", m_localisation);
    query.bindValue(":nbEmployes",   m_nbEmployes);
    query.bindValue(":classement",   m_classement);

    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur ajout (seq):" << query.lastError().text();
        qDebug() << "[Laboratoire] SQL:" << sql;

        // Fallback: without explicit ID (auto or trigger)
        QString sqlFallback = QString(
            "INSERT INTO LABORATOIRE (NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT) "
            "VALUES (:nom, %1, TO_DATE('%2', 'YYYY-MM-DD'), :domaine, :localisation, :nbEmployes, :classement)"
        ).arg(budgetSql, dateStr);

        QSqlQuery fallback(db);
        fallback.prepare(sqlFallback);
        fallback.bindValue(":nom",          m_nom);
        fallback.bindValue(":domaine",      m_domaine);
        fallback.bindValue(":localisation", m_localisation);
        fallback.bindValue(":nbEmployes",   m_nbEmployes);
        fallback.bindValue(":classement",   m_classement);

        if (!fallback.exec()) {
            m_lastError = fallback.lastError().text();
            qDebug() << "[Laboratoire] Erreur ajout (fallback):" << m_lastError;
            return false;
        }
    }

    qDebug() << "[Laboratoire] Laboratoire ajoute avec succes";
    return true;
}

// ============================================================
//  LaboratoireModel â€“ modifier
// ============================================================
bool LaboratoireModel::modifier()
{
    QString erreur;
    if (!valider(erreur)) {
        qDebug() << "[Laboratoire] Validation echouee:" << erreur;
        return false;
    }
    if (m_id <= 0) {
        qDebug() << "[Laboratoire] ID invalide pour modification:" << m_id;
        return false;
    }

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return false;

    QString dateStr   = m_dateCreation.toString("yyyy-MM-dd");
    QString budgetSql = QString::number(m_budget, 'f', 2);

    QString sql = QString(
        "UPDATE LABORATOIRE SET "
        "NOM          = :nom, "
        "BUDGET       = %1, "
        "DATE_CREATION = TO_DATE('%2', 'YYYY-MM-DD'), "
        "DOMAINE      = :domaine, "
        "LOCALISATION = :localisation, "
        "nombre_chercheurs  = :nbEmployes, "
        "CLASSEMENT   = :classement "
        "WHERE ID_labo = %3"
    ).arg(budgetSql, dateStr, QString::number(m_id));

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":nom",          m_nom);
    query.bindValue(":domaine",      m_domaine);
    query.bindValue(":localisation", m_localisation);
    query.bindValue(":nbEmployes",   m_nbEmployes);
    query.bindValue(":classement",   m_classement);

    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur modification:" << query.lastError().text();
        qDebug() << "[Laboratoire] SQL:" << sql;
        return false;
    }

    // ── Mise à jour de l'historique ──────────────────────────────
    QString entree = QString("[%1] Modification : Nom=%2, Budget=%3, Domaine=%4, Classement=%5\n")
                         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"))
                         .arg(m_nom)
                         .arg(QString::number(m_budget, 'f', 2))
                         .arg(m_domaine)
                         .arg(m_classement);

    // Récupérer l'historique existant et y ajouter la nouvelle entrée
    QSqlQuery hQuery(db);
    hQuery.prepare("SELECT historique FROM LABORATOIRE WHERE ID_labo = :id");
    hQuery.bindValue(":id", m_id);
    QString ancienHistorique;
    if (hQuery.exec() && hQuery.next()) {
        ancienHistorique = hQuery.value(0).toString();
    }
    QString nouvelHistorique = entree + ancienHistorique;

    QSqlQuery updateHist(db);
    updateHist.prepare("UPDATE LABORATOIRE SET historique = :hist WHERE ID_labo = :id");
    updateHist.bindValue(":hist", nouvelHistorique);
    updateHist.bindValue(":id",   m_id);
    updateHist.exec();
    m_historique = nouvelHistorique;

    qDebug() << "[Laboratoire] Laboratoire modifie avec succes (ID:" << m_id << ")";
    return true;
}

// ============================================================
//  LaboratoireModel â€“ supprimer
// ============================================================
bool LaboratoireModel::supprimer()
{
    if (m_id <= 0) {
        qDebug() << "[Laboratoire] ID invalide pour suppression";
        return false;
    }

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM LABORATOIRE WHERE ID_labo = :id");
    query.bindValue(":id", m_id);

    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur suppression:" << query.lastError().text();
        return false;
    }

    qDebug() << "[Laboratoire] Laboratoire supprime avec succes";
    return true;
}

// ============================================================
//  LaboratoireModel â€“ afficherTous
// ============================================================
QList<LaboratoireModel> LaboratoireModel::afficherTous()
{
    QList<LaboratoireModel> labs;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return labs;

    QSqlQuery query(db);
    if (!query.exec(
            "SELECT ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT "
            "FROM LABORATOIRE ORDER BY DATE_CREATION DESC")) {
        qDebug() << "[Laboratoire] Erreur afficherTous:" << query.lastError().text();
        return labs;
    }

    while (query.next()) {
        LaboratoireModel lab;
        lab.setId(query.value(0).toInt());
        lab.setNom(query.value(1).toString());
        lab.setBudget(query.value(2).toDouble());
        lab.setDateCreation(query.value(3).toDate());
        lab.setDomaine(query.value(4).toString());
        lab.setLocalisation(query.value(5).toString());
        lab.setNbEmployes(query.value(6).toInt());
        lab.setClassement(query.value(7).toString());
        labs.append(lab);
    }

    return labs;
}

// ============================================================
//  LaboratoireModel â€“ rechercher
// ============================================================
QList<LaboratoireModel> LaboratoireModel::rechercher(const QString &critere, const QString &valeur)
{
    if (valeur.trimmed().isEmpty()) {
        return afficherTous();
    }

    QList<LaboratoireModel> labs;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return labs;

    QString sql =
        "SELECT ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT "
        "FROM LABORATOIRE WHERE ";

    if (critere == "Rechercher par Nom" || critere == "Nom") {
        sql += "UPPER(NOM) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Domaine" || critere == "Domaine") {
        sql += "UPPER(DOMAINE) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Localisation" || critere == "Localisation") {
        sql += "UPPER(LOCALISATION) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Classement" || critere == "Classement") {
        sql += "UPPER(CLASSEMENT) LIKE UPPER(:valeur)";
    } else if (critere == "Rechercher par Date" || critere == "Date") {
        sql += "TO_CHAR(DATE_CREATION, 'YYYY-MM-DD') LIKE :valeur";
    } else {
        return afficherTous();
    }

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":valeur", "%" + valeur + "%");

    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur recherche:" << query.lastError().text();
        return labs;
    }

    while (query.next()) {
        LaboratoireModel lab;
        lab.setId(query.value(0).toInt());
        lab.setNom(query.value(1).toString());
        lab.setBudget(query.value(2).toDouble());
        lab.setDateCreation(query.value(3).toDate());
        lab.setDomaine(query.value(4).toString());
        lab.setLocalisation(query.value(5).toString());
        lab.setNbEmployes(query.value(6).toInt());
        lab.setClassement(query.value(7).toString());
        labs.append(lab);
    }

    return labs;
}

// ============================================================
//  LaboratoireModel â€“ trierPar
// ============================================================
QList<LaboratoireModel> LaboratoireModel::trierPar(const QString &colonne, bool croissant)
{
    QList<LaboratoireModel> labs;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return labs;

    QString colonneSQL;
    if (colonne == "Nom")         colonneSQL = "NOM";
    else if (colonne == "Budget") colonneSQL = "BUDGET";
    else if (colonne == "Date")   colonneSQL = "DATE_CREATION";
    else if (colonne == "nombre_chercheurs" || colonne == "Employes") colonneSQL = "nombre_chercheurs";
    else if (colonne == "Domaine")    colonneSQL = "DOMAINE";
    else if (colonne == "Classement") colonneSQL = "CLASSEMENT";
    else                              colonneSQL = "DATE_CREATION";

    QString ordre = croissant ? "ASC" : "DESC";

    QString sql = QString(
        "SELECT ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT "
        "FROM LABORATOIRE ORDER BY %1 %2"
    ).arg(colonneSQL, ordre);

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qDebug() << "[Laboratoire] Erreur tri:" << query.lastError().text();
        return labs;
    }

    while (query.next()) {
        LaboratoireModel lab;
        lab.setId(query.value(0).toInt());
        lab.setNom(query.value(1).toString());
        lab.setBudget(query.value(2).toDouble());
        lab.setDateCreation(query.value(3).toDate());
        lab.setDomaine(query.value(4).toString());
        lab.setLocalisation(query.value(5).toString());
        lab.setNbEmployes(query.value(6).toInt());
        lab.setClassement(query.value(7).toString());
        labs.append(lab);
    }

    return labs;
}

// ============================================================
//  LaboratoireModel â€“ getById
// ============================================================
LaboratoireModel LaboratoireModel::getById(int id)
{
    LaboratoireModel lab;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return lab;

    QSqlQuery query(db);
    query.prepare(
        "SELECT ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, nombre_chercheurs, CLASSEMENT "
        "FROM LABORATOIRE WHERE ID_labo = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur getById:" << query.lastError().text();
        return lab;
    }

    if (query.next()) {
        lab.setId(query.value(0).toInt());
        lab.setNom(query.value(1).toString());
        lab.setBudget(query.value(2).toDouble());
        lab.setDateCreation(query.value(3).toDate());
        lab.setDomaine(query.value(4).toString());
        lab.setLocalisation(query.value(5).toString());
        lab.setNbEmployes(query.value(6).toInt());
        lab.setClassement(query.value(7).toString());
    }

    return lab;
}

// ============================================================
//  LaboratoireModel â€“ getStatsParDomaine
// ============================================================
QMap<QString, int> LaboratoireModel::getStatsParDomaine()
{
    QMap<QString, int> stats;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return stats;

    QSqlQuery query(db);
    if (query.exec("SELECT DOMAINE, COUNT(*) FROM LABORATOIRE GROUP BY DOMAINE")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    } else {
        qDebug() << "[Laboratoire] Erreur stats domaine:" << query.lastError().text();
    }

    return stats;
}

// ============================================================
//  LaboratoireModel â€“ getStatsParClassement
// ============================================================
QMap<QString, int> LaboratoireModel::getStatsParClassement()
{
    QMap<QString, int> stats;

    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return stats;

    QSqlQuery query(db);
    if (query.exec("SELECT CLASSEMENT, COUNT(*) FROM LABORATOIRE GROUP BY CLASSEMENT")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    } else {
        qDebug() << "[Laboratoire] Erreur stats classement:" << query.lastError().text();
    }

    return stats;
}

// ============================================================

// ============================================================
//  LaboratoireModel - getBudgetMoyenParDomaine
// ============================================================
QMap<QString, double> LaboratoireModel::getBudgetMoyenParDomaine()
{
    QMap<QString, double> stats;
    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return stats;
    QSqlQuery query(db);
    if (query.exec("SELECT DOMAINE, AVG(BUDGET) FROM LABORATOIRE GROUP BY DOMAINE ORDER BY DOMAINE")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toDouble());
        }
    } else {
        qDebug() << "[Laboratoire] Erreur budget moyen:" << query.lastError().text();
    }
    return stats;
}

// ============================================================
//  LaboratoireModel - getLabsDepassantBudget
// ============================================================
QList<LaboratoireModel> LaboratoireModel::getLabsDepassantBudget(double seuilPct, double budgetRef)
{
    QList<LaboratoireModel> labs;
    QSqlDatabase db = labOpenDb();
    if (!db.isOpen()) return labs;
    double seuil = budgetRef * (seuilPct / 100.0);
    QSqlQuery query(db);
    query.prepare(
        "SELECT ID_labo, NOM, BUDGET, DATE_CREATION, DOMAINE, LOCALISATION, "
        "nombre_chercheurs, CLASSEMENT, historique "
        "FROM LABORATOIRE WHERE BUDGET > :seuil ORDER BY BUDGET DESC");
    query.bindValue(":seuil", seuil);
    if (!query.exec()) {
        qDebug() << "[Laboratoire] Erreur alerte budget:" << query.lastError().text();
        return labs;
    }
    while (query.next()) {
        LaboratoireModel lab;
        lab.setId(query.value(0).toInt());
        lab.setNom(query.value(1).toString());
        lab.setBudget(query.value(2).toDouble());
        lab.setDateCreation(query.value(3).toDate());
        lab.setDomaine(query.value(4).toString());
        lab.setLocalisation(query.value(5).toString());
        lab.setNbEmployes(query.value(6).toInt());
        lab.setClassement(query.value(7).toString());
        lab.setHistorique(query.value(8).toString());
        labs.append(lab);
    }
    return labs;
}

//  Laboratoire Widget â€“ Constructor / Destructor




// ============================================================
//  Laboratoire Widget -- Constructor
// ============================================================
Laboratoire::Laboratoire(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Laboratoire)
{
    ui->setupUi(this);
    setupConnections();

    // Load data from Oracle on startup
    if (Connection::createInstance().getDb().isOpen()) {
        QList<LaboratoireModel> list = LaboratoireModel::afficherTous();
        QList<LaboratoireData> data;
        data.reserve(list.size());
        for (const LaboratoireModel &m : list) {
            data.append(modelToData(m));
        }
        refreshTable(data);
    }

}

// ============================================================
//  Laboratoire Widget -- Destructor
// ============================================================
Laboratoire::~Laboratoire()
{
    delete ui;
}

// ============================================================
//  setupConnections
// ============================================================
void Laboratoire::setupConnections()
{
    connect(ui->ajouterButton,         &QPushButton::clicked, this, &Laboratoire::onAjouterClicked);
    connect(ui->ajouterFormButton,     &QPushButton::clicked, this, &Laboratoire::onAjouterFormClicked);
    connect(ui->modifierButton,        &QPushButton::clicked, this, &Laboratoire::onModifierClicked);
    connect(ui->modifierFormButton,    &QPushButton::clicked, this, &Laboratoire::onModifierFormClicked);
    connect(ui->annulerModifButton,    &QPushButton::clicked, this, &Laboratoire::onAnnulerModifClicked);
    connect(ui->supprimerButton,       &QPushButton::clicked, this, &Laboratoire::onSupprimerClicked);
    connect(ui->rechercherButton,      &QPushButton::clicked, this, &Laboratoire::onRechercherClicked);
    connect(ui->trierButton,           &QPushButton::clicked, this, &Laboratoire::onTrierClicked);
    connect(ui->exporterPDFButton,     &QPushButton::clicked, this, &Laboratoire::onExporterPDFClicked);
    connect(ui->statistiquesButton,    &QPushButton::clicked, this, &Laboratoire::onStatistiquesClicked);
    connect(ui->clearSearchButton,     &QPushButton::clicked, this, &Laboratoire::onClearSearchClicked);
    connect(ui->historiqueButton,      &QPushButton::clicked, this, &Laboratoire::onHistoriqueClicked);
    connect(ui->alerteBudgetButton,    &QPushButton::clicked, this, &Laboratoire::onAlerteBudgetClicked);
    connect(ui->supprimerHistButton,   &QPushButton::clicked, this, &Laboratoire::onSupprimerHistoriqueClicked);
    connect(ui->exporterHistPDFButton, &QPushButton::clicked, this, &Laboratoire::onExporterHistPDFClicked);
}

// ============================================================
//  refreshTable -- fills 7-col table, stores IDs in m_labIds
// ============================================================
void Laboratoire::refreshTable(const QList<LaboratoireData> &labs)
{
    m_labIds.clear();
    ui->laboratoiresTable->setRowCount(0);
    ui->laboratoiresTable->setRowCount(labs.size());

    for (int row = 0; row < labs.size(); ++row) {
        const LaboratoireData &d = labs.at(row);
        m_labIds.append(d.id);  // store ID without displaying it

        // Col 0: NOM
        ui->laboratoiresTable->setItem(row, 0, new QTableWidgetItem(d.nom));
        // Col 1: BUDGET  "522.00 EUR"
        ui->laboratoiresTable->setItem(row, 1,
            new QTableWidgetItem(QString::number(d.budget, 'f', 2) + " \xe2\x82\xac"));
        // Col 2: DATE CREATION  "dd/MM/yyyy"
        ui->laboratoiresTable->setItem(row, 2,
            new QTableWidgetItem(d.dateCreation.toString("dd/MM/yyyy")));
        // Col 3: DOMAINE
        ui->laboratoiresTable->setItem(row, 3, new QTableWidgetItem(d.domaine));
        // Col 4: LOCALISATION
        ui->laboratoiresTable->setItem(row, 4, new QTableWidgetItem(d.localisation));
        // Col 5: CHERCHEURS
        ui->laboratoiresTable->setItem(row, 5,
            new QTableWidgetItem(QString::number(d.nbEmployes)));
        // Col 6: CLASSEMENT
        ui->laboratoiresTable->setItem(row, 6, new QTableWidgetItem(d.classement));
    }

    ui->laboratoiresTable->resizeColumnsToContents();
    ui->laboratoiresTable->horizontalHeader()->setStretchLastSection(true);
}

// ============================================================
//  readFormData  (add form)
// ============================================================
LaboratoireData Laboratoire::readFormData() const
{
    LaboratoireData d;
    d.id           = -1;
    d.nom          = ui->nomLineEdit->text().trimmed();
    d.budget       = ui->budgetSpinBox->value();
    d.dateCreation = ui->dateCreationEdit->date();
    d.domaine      = ui->domaineComboBox->currentText();
    d.localisation = ui->localisationLineEdit->text().trimmed();
    d.nbEmployes   = ui->nbEmployesSpinBox->value();
    d.classement   = ui->classementComboBox->currentText();
    return d;
}

// ============================================================
//  populateModifierForm
// ============================================================
void Laboratoire::populateModifierForm(const LaboratoireData &lab)
{
    ui->modIdLineEdit->setText(QString::number(lab.id));
    ui->modNomLineEdit->setText(lab.nom);
    ui->modBudgetSpinBox->setValue(lab.budget);
    ui->modDateCreationEdit->setDate(
        lab.dateCreation.isValid() ? lab.dateCreation : QDate::currentDate());

    int domaineIdx = ui->modDomaineComboBox->findText(lab.domaine);
    if (domaineIdx >= 0) ui->modDomaineComboBox->setCurrentIndex(domaineIdx);

    ui->modLocalisationLineEdit->setText(lab.localisation);
    ui->modNbEmployesSpinBox->setValue(lab.nbEmployes);

    int classementIdx = ui->modClassementComboBox->findText(lab.classement);
    if (classementIdx >= 0) ui->modClassementComboBox->setCurrentIndex(classementIdx);
}

// ============================================================
//  onAjouterClicked -- switch to tab index 1 (Ajouter)
// ============================================================
void Laboratoire::onAjouterClicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

// ============================================================
//  onAjouterFormClicked -- validate + insert + refresh + go to tab 0
// ============================================================
void Laboratoire::onAjouterFormClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    LaboratoireData formData = readFormData();

    LaboratoireModel lab(0, formData.nom, formData.budget, formData.dateCreation,
                         formData.domaine, formData.localisation,
                         formData.nbEmployes, formData.classement);

    QString erreur;
    if (!lab.valider(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    if (!lab.ajouter()) {
        QMessageBox::critical(this, "Erreur SQL Oracle",
            QString("Impossible d'ajouter le laboratoire.\n\nErreur Oracle :\n%1")
                .arg(lab.getLastError()));
        return;
    }

    // Clear add form
    ui->nomLineEdit->clear();
    ui->budgetSpinBox->setValue(0);
    ui->dateCreationEdit->setDate(QDate::currentDate());
    ui->localisationLineEdit->clear();
    ui->nbEmployesSpinBox->setValue(0);

    // Refresh table
    QList<LaboratoireModel> list = LaboratoireModel::afficherTous();
    QList<LaboratoireData> data;
    data.reserve(list.size());
    for (const LaboratoireModel &m : list) data.append(modelToData(m));
    refreshTable(data);

    QMessageBox::information(this, "Succes", "Laboratoire ajoute avec succes !");
    ui->tabWidget->setCurrentIndex(0);
}

// ============================================================
//  onModifierClicked -- get ID from m_labIds[row], fill form, tab 3
// ============================================================
void Laboratoire::onModifierClicked()
{
    int row = ui->laboratoiresTable->currentRow();
    if (row < 0 || row >= m_labIds.size()) {
        QMessageBox::information(this, "Modifier",
            "Veuillez selectionner un laboratoire dans la liste.");
        return;
    }

    int id = m_labIds.at(row);
    if (id <= 0) {
        QMessageBox::warning(this, "Modifier", "ID laboratoire invalide.");
        return;
    }

    LaboratoireModel lab = LaboratoireModel::getById(id);
    if (lab.getId() <= 0) {
        QMessageBox::warning(this, "Modifier",
            "Impossible de charger le laboratoire depuis la base de donnees.");
        return;
    }

    populateModifierForm(modelToData(lab));
    ui->tabWidget->setCurrentIndex(3);  // tab 3 = Modifier
}

// ============================================================
//  onModifierFormClicked -- compare fields, build DETAILS, update historique, save
// ============================================================
void Laboratoire::onModifierFormClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    bool ok = false;
    int id = ui->modIdLineEdit->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Modifier",
            "Aucun laboratoire charge. Selectionnez-en un dans la liste d'abord.");
        return;
    }

    // Load old values from DB for comparison
    LaboratoireModel oldLab = LaboratoireModel::getById(id);

    // Build new model from form
    LaboratoireModel newLab;
    newLab.setId(id);
    newLab.setNom(ui->modNomLineEdit->text().trimmed());
    newLab.setBudget(ui->modBudgetSpinBox->value());
    newLab.setDateCreation(ui->modDateCreationEdit->date());
    newLab.setDomaine(ui->modDomaineComboBox->currentText());
    newLab.setLocalisation(ui->modLocalisationLineEdit->text().trimmed());
    newLab.setNbEmployes(ui->modNbEmployesSpinBox->value());
    newLab.setClassement(ui->modClassementComboBox->currentText());

    QString erreur;
    if (!newLab.valider(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    // Build DETAILS string comparing old vs new
    QStringList changes;
    if (oldLab.getNom() != newLab.getNom())
        changes << QString("Nom: '%1' -> '%2'").arg(oldLab.getNom(), newLab.getNom());
    if (qAbs(oldLab.getBudget() - newLab.getBudget()) > 0.001)
        changes << QString("Budget: %1 EUR -> %2 EUR")
                       .arg(QString::number(oldLab.getBudget(), 'f', 2),
                            QString::number(newLab.getBudget(), 'f', 2));
    if (oldLab.getDateCreation() != newLab.getDateCreation())
        changes << QString("Date: %1 -> %2")
                       .arg(oldLab.getDateCreation().toString("dd/MM/yyyy"),
                            newLab.getDateCreation().toString("dd/MM/yyyy"));
    if (oldLab.getDomaine() != newLab.getDomaine())
        changes << QString("Domaine: '%1' -> '%2'").arg(oldLab.getDomaine(), newLab.getDomaine());
    if (oldLab.getLocalisation() != newLab.getLocalisation())
        changes << QString("Localisation: '%1' -> '%2'")
                       .arg(oldLab.getLocalisation(), newLab.getLocalisation());
    if (oldLab.getNbEmployes() != newLab.getNbEmployes())
        changes << QString("Chercheurs: %1 -> %2")
                       .arg(oldLab.getNbEmployes()).arg(newLab.getNbEmployes());
    if (oldLab.getClassement() != newLab.getClassement())
        changes << QString("Classement: '%1' -> '%2'")
                       .arg(oldLab.getClassement(), newLab.getClassement());

    QString details = changes.isEmpty() ? "Aucun changement" : changes.join("; ");

    // Build new historique CLOB line: "DATE|LABO_NOM|Modification|admin|DETAILS\n"
    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    QString newLine = QString("%1|%2|Modification|admin|%3\n")
                          .arg(dateStr, newLab.getNom(), details);

    // Prepend new line to existing historique
    QString ancienHist = oldLab.getHistorique();
    QString nouvelHist = newLine + ancienHist;
    newLab.setHistorique(nouvelHist);

    // Save main fields (use direct SQL to also update historique atomically)
    QSqlDatabase db = Connection::createInstance().getDb();
    QString dateSql = newLab.getDateCreation().toString("yyyy-MM-dd");
    QString budgetSql = QString::number(newLab.getBudget(), 'f', 2);

    QString sql = QString(
        "UPDATE LABORATOIRE SET "
        "NOM = :nom, "
        "BUDGET = %1, "
        "DATE_CREATION = TO_DATE('%2', 'YYYY-MM-DD'), "
        "DOMAINE = :domaine, "
        "LOCALISATION = :localisation, "
        "nombre_chercheurs = :nbEmployes, "
        "CLASSEMENT = :classement, "
        "historique = :historique "
        "WHERE ID_labo = %3"
    ).arg(budgetSql, dateSql, QString::number(id));

    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":nom",          newLab.getNom());
    query.bindValue(":domaine",      newLab.getDomaine());
    query.bindValue(":localisation", newLab.getLocalisation());
    query.bindValue(":nbEmployes",   newLab.getNbEmployes());
    query.bindValue(":classement",   newLab.getClassement());
    query.bindValue(":historique",   nouvelHist);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
            QString("Impossible de modifier le laboratoire.\n\nErreur :\n%1")
                .arg(query.lastError().text()));
        return;
    }

    // Refresh table
    QList<LaboratoireModel> list = LaboratoireModel::afficherTous();
    QList<LaboratoireData> data;
    data.reserve(list.size());
    for (const LaboratoireModel &m : list) data.append(modelToData(m));
    refreshTable(data);

    QMessageBox::information(this, "Succes", "Laboratoire modifie avec succes !");
    ui->tabWidget->setCurrentIndex(0);
}

// ============================================================
//  onAnnulerModifClicked -- clear modifier form, go to tab 0
// ============================================================
void Laboratoire::onAnnulerModifClicked()
{
    ui->modIdLineEdit->clear();
    ui->modNomLineEdit->clear();
    ui->modBudgetSpinBox->setValue(0);
    ui->modDateCreationEdit->setDate(QDate::currentDate());
    ui->modLocalisationLineEdit->clear();
    ui->modNbEmployesSpinBox->setValue(0);
    ui->tabWidget->setCurrentIndex(0);
}

// ============================================================
//  onSupprimerClicked -- get ID from m_labIds[row], confirm, delete, refresh
// ============================================================
void Laboratoire::onSupprimerClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    int row = ui->laboratoiresTable->currentRow();
    if (row < 0 || row >= m_labIds.size()) {
        QMessageBox::information(this, "Supprimer",
            "Veuillez selectionner un laboratoire dans la liste.");
        return;
    }

    int id = m_labIds.at(row);
    if (id <= 0) {
        QMessageBox::warning(this, "Supprimer", "ID laboratoire invalide.");
        return;
    }

    // Get name from table for confirmation message (col 0 = NOM)
    QString nom;
    if (ui->laboratoiresTable->item(row, 0))
        nom = ui->laboratoiresTable->item(row, 0)->text();

    if (labConfirmDialog(this, "Supprimer",
            QString("Confirmer la suppression de \"%1\" ?").arg(nom)) != QMessageBox::Yes) {
        return;
    }

    LaboratoireModel lab;
    lab.setId(id);

    if (!lab.supprimer()) {
        QMessageBox::critical(this, "Erreur",
            "Impossible de supprimer le laboratoire.");
        return;
    }

    // Refresh table
    QList<LaboratoireModel> list = LaboratoireModel::afficherTous();
    QList<LaboratoireData> data;
    data.reserve(list.size());
    for (const LaboratoireModel &m : list) data.append(modelToData(m));
    refreshTable(data);

    QMessageBox::information(this, "Succes", "Laboratoire supprime avec succes !");
}

// ============================================================
//  onRechercherClicked
// ============================================================
void Laboratoire::onRechercherClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    QString searchText = ui->searchLineEdit->text().trimmed();
    QString searchType = ui->searchComboBox->currentText();

    QList<LaboratoireModel> list = LaboratoireModel::rechercher(searchType, searchText);
    QList<LaboratoireData> data;
    data.reserve(list.size());
    for (const LaboratoireModel &m : list) data.append(modelToData(m));
    refreshTable(data);
}

// ============================================================
//  onTrierClicked -- QMenu with sort options
// ============================================================
void Laboratoire::onTrierClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    auto buildData = [](const QList<LaboratoireModel> &list) {
        QList<LaboratoireData> data;
        data.reserve(list.size());
        for (const LaboratoireModel &m : list) data.append(modelToData(m));
        return data;
    };

    QMenu menu;
    menu.addAction("Nom A\xe2\x86\x92Z",         [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Nom",        true))); });
    menu.addAction("Nom Z\xe2\x86\x92A",         [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Nom",        false))); });
    menu.addSeparator();
    menu.addAction("Budget \xe2\x86\x91",        [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Budget",     true))); });
    menu.addAction("Budget \xe2\x86\x93",        [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Budget",     false))); });
    menu.addSeparator();
    menu.addAction("Date \xe2\x86\x91",          [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Date",       true))); });
    menu.addAction("Date \xe2\x86\x93",          [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Date",       false))); });
    menu.addSeparator();
    menu.addAction("Domaine A\xe2\x86\x92Z",     [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Domaine",    true))); });
    menu.addAction("Domaine Z\xe2\x86\x92A",     [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Domaine",    false))); });
    menu.addSeparator();
    menu.addAction("Classement A\xe2\x86\x92Z",  [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Classement", true))); });
    menu.addAction("Classement Z\xe2\x86\x92A",  [this, buildData]() { refreshTable(buildData(LaboratoireModel::trierPar("Classement", false))); });

    menu.exec(QCursor::pos());
}

// ============================================================
//  onClearSearchClicked -- clear search, reload all
// ============================================================
void Laboratoire::onClearSearchClicked()
{
    ui->searchLineEdit->clear();

    if (Connection::createInstance().getDb().isOpen()) {
        QList<LaboratoireModel> list = LaboratoireModel::afficherTous();
        QList<LaboratoireData> data;
        data.reserve(list.size());
        for (const LaboratoireModel &m : list) data.append(modelToData(m));
        refreshTable(data);
    }
}

// ============================================================
//  onExporterPDFClicked -- export table to PDF
// ============================================================
void Laboratoire::onExporterPDFClicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Exporter PDF", "laboratoires.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QList<LaboratoireModel> list = LaboratoireModel::afficherTous();

    QString html;
    html += "<html><head><meta charset='UTF-8'></head><body>";
    html += "<h2 style='color:#8B5CF6;'>Liste des Laboratoires</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%' "
            "style='border-collapse:collapse; font-size:11px;'>";
    html += "<tr style='background-color:#8B5CF6; color:white;'>"
            "<th>Nom</th><th>Budget (DT)</th><th>Date Creation</th>"
            "<th>Domaine</th><th>Localisation</th><th>Chercheurs</th><th>Classement</th></tr>";

    for (int i = 0; i < list.size(); ++i) {
        const LaboratoireModel &lab = list.at(i);
        QString bg = (i % 2 == 0) ? "#FFFFFF" : "#F3F4F6";
        html += QString("<tr style='background-color:%1;'>").arg(bg);
        html += QString("<td>%1</td>").arg(lab.getNom().toHtmlEscaped());
        html += QString("<td>%1 \xe2\x82\xac</td>").arg(QString::number(lab.getBudget(), 'f', 2));
        html += QString("<td>%1</td>").arg(lab.getDateCreation().toString("dd/MM/yyyy"));
        html += QString("<td>%1</td>").arg(lab.getDomaine().toHtmlEscaped());
        html += QString("<td>%1</td>").arg(lab.getLocalisation().toHtmlEscaped());
        html += QString("<td>%1</td>").arg(lab.getNbEmployes());
        html += QString("<td>%1</td>").arg(lab.getClassement().toHtmlEscaped());
        html += "</tr>";
    }

    html += "</table>";
    html += QString("<p style='color:#6B7280; font-size:11px;'>Total : %1 laboratoire(s) - "
                    "Exporte le %2</p>")
                .arg(list.size())
                .arg(QDate::currentDate().toString("dd/MM/yyyy"));
    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);

    QMessageBox::information(this, "Exporter PDF",
        QString("PDF exporte avec succes !\n%1").arg(filePath));
}

// ============================================================
//  onExporterWordClicked -- disabled
// ============================================================
void Laboratoire::onExporterWordClicked()
{
    QMessageBox::information(this, "Exporter Word",
        "L'export Word est desactive dans cette version.");
}

// ============================================================
//  onStatistiquesClicked -- Qt Charts dialog with 3 tabs
// ============================================================
void Laboratoire::onStatistiquesClicked()
{
    if (!Connection::createInstance().getDb().isOpen()) {
        QMessageBox::warning(this, "Base de Donnees",
            "Aucune connexion a la base de donnees.");
        return;
    }

    QMap<QString, int>    statsDomaine    = LaboratoireModel::getStatsParDomaine();
    QMap<QString, int>    statsClassement = LaboratoireModel::getStatsParClassement();
    QMap<QString, double> statsBudget     = LaboratoireModel::getBudgetMoyenParDomaine();

    if (statsDomaine.isEmpty() && statsClassement.isEmpty()) {
        QMessageBox::information(this, "Statistiques",
            "Aucune donnee disponible pour generer les statistiques.");
        return;
    }

    // ── Pie chart: repartition par Domaine ──────────────────────
    auto *pieSeries = new QPieSeries();
    for (auto it = statsDomaine.cbegin(); it != statsDomaine.cend(); ++it) {
        QPieSlice *slice = pieSeries->append(
            QString("%1 (%2)").arg(it.key()).arg(it.value()), it.value());
        slice->setLabelVisible(true);
    }
    auto *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Repartition par Domaine");
    pieChart->setAnimationOptions(QChart::AllAnimations);
    pieChart->legend()->setAlignment(Qt::AlignBottom);
    auto *pieView = new QChartView(pieChart);
    pieView->setRenderHint(QPainter::Antialiasing);

    // ── Bar chart: repartition par Classement ───────────────────
    auto *barSet1 = new QBarSet("Laboratoires");
    QStringList catClass;
    for (auto it = statsClassement.cbegin(); it != statsClassement.cend(); ++it) {
        *barSet1 << it.value();
        catClass << it.key();
    }
    auto *barSeries1 = new QBarSeries();
    barSeries1->append(barSet1);
    auto *barChart1 = new QChart();
    barChart1->addSeries(barSeries1);
    barChart1->setTitle("Repartition par Classement");
    barChart1->setAnimationOptions(QChart::SeriesAnimations);
    auto *axisX1 = new QBarCategoryAxis();
    axisX1->append(catClass);
    barChart1->addAxis(axisX1, Qt::AlignBottom);
    barSeries1->attachAxis(axisX1);
    auto *axisY1 = new QValueAxis();
    axisY1->setLabelFormat("%d");
    axisY1->setTitleText("Nombre");
    barChart1->addAxis(axisY1, Qt::AlignLeft);
    barSeries1->attachAxis(axisY1);
    barChart1->legend()->setVisible(false);
    auto *barView1 = new QChartView(barChart1);
    barView1->setRenderHint(QPainter::Antialiasing);

    // ── Bar chart: Budget moyen par Domaine ─────────────────────
    auto *barSet2 = new QBarSet("Budget moyen (DT)");
    QStringList catBudget;
    for (auto it = statsBudget.cbegin(); it != statsBudget.cend(); ++it) {
        *barSet2 << it.value();
        catBudget << it.key();
    }
    auto *barSeries2 = new QBarSeries();
    barSeries2->append(barSet2);
    auto *barChart2 = new QChart();
    barChart2->addSeries(barSeries2);
    barChart2->setTitle("Budget Moyen par Domaine (DT)");
    barChart2->setAnimationOptions(QChart::SeriesAnimations);
    auto *axisX2 = new QBarCategoryAxis();
    axisX2->append(catBudget);
    barChart2->addAxis(axisX2, Qt::AlignBottom);
    barSeries2->attachAxis(axisX2);
    auto *axisY2 = new QValueAxis();
    axisY2->setLabelFormat("%.0f");
    axisY2->setTitleText("Budget (DT)");
    barChart2->addAxis(axisY2, Qt::AlignLeft);
    barSeries2->attachAxis(axisY2);
    barChart2->legend()->setVisible(false);
    auto *barView2 = new QChartView(barChart2);
    barView2->setRenderHint(QPainter::Antialiasing);

    // ── Summary label ────────────────────────────────────────────
    int total = 0;
    double budgetTotal = 0;
    for (int v : statsDomaine) total += v;
    for (double v : statsBudget) budgetTotal += v;

    QString budgetMoyenGlobal = statsBudget.isEmpty()
        ? "N/A"
        : QString::number(budgetTotal / statsBudget.size(), 'f', 2);

    auto *summaryLabel = new QLabel(
        QString("<b>Total laboratoires :</b> %1 &nbsp;&nbsp; "
                "<b>Domaines :</b> %2 &nbsp;&nbsp; "
                "<b>Classements :</b> %3 &nbsp;&nbsp; "
                "<b>Budget moyen global :</b> %4 DT")
            .arg(total)
            .arg(statsDomaine.size())
            .arg(statsClassement.size())
            .arg(budgetMoyenGlobal)
    );
    summaryLabel->setAlignment(Qt::AlignCenter);
    summaryLabel->setStyleSheet(
        "font-size:14px; padding:10px; background:#EEF2FF; "
        "border-radius:6px; font-weight:500;");

    // ── Dialog ───────────────────────────────────────────────────
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("Dashboard - Statistiques des Laboratoires");
    dialog->resize(1000, 650);

    auto *tabs = new QTabWidget(dialog);
    tabs->addTab(pieView,  "Repartition par Domaine");
    tabs->addTab(barView1, "Repartition par Classement");
    tabs->addTab(barView2, "Budget Moyen par Domaine");

    auto *closeBtn = new QPushButton("Fermer", dialog);
    closeBtn->setStyleSheet(
        "QPushButton { background-color:#6366F1; color:white; border:none; "
        "border-radius:6px; padding:8px 24px; font-size:13px; }"
        "QPushButton:hover { background-color:#4F46E5; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    auto *layout = new QVBoxLayout(dialog);
    layout->addWidget(summaryLabel);
    layout->addWidget(tabs);
    layout->addWidget(closeBtn);
    dialog->setLayout(layout);
    dialog->exec();
}

// ============================================================
//  onHistoriqueClicked -- switch to tab 2 (Historique) and refresh
// ============================================================
void Laboratoire::onHistoriqueClicked()
{
    ui->tabWidget->setCurrentIndex(2);
    refreshHistorique();
}

// ============================================================
//  refreshHistorique -- query ALL labs, parse CLOB, fill historiqueTable
//  CLOB line format: "DATE|LABO_NOM|TYPE|USER|DETAILS\n"
// ============================================================
void Laboratoire::refreshHistorique()
{
    if (!Connection::createInstance().getDb().isOpen()) return;

    // Collect all entries from all labs
    struct HistEntry {
        QString date;
        QString labNom;
        QString type;
        QString user;
        QString details;
    };
    QList<HistEntry> entries;

    QSqlDatabase db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    if (!query.exec("SELECT NOM, historique FROM LABORATOIRE WHERE historique IS NOT NULL")) {
        qDebug() << "[Laboratoire] refreshHistorique error:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString labNom = query.value(0).toString();
        QString clob   = query.value(1).toString();

        QStringList lines = clob.split('\n', Qt::SkipEmptyParts);
        for (const QString &line : lines) {
            QStringList parts = line.split('|');
            if (parts.size() >= 5) {
                HistEntry e;
                e.date    = parts.at(0).trimmed();
                e.labNom  = parts.at(1).trimmed();
                e.type    = parts.at(2).trimmed();
                e.user    = parts.at(3).trimmed();
                e.details = parts.mid(4).join("|").trimmed();
                entries.append(e);
            } else if (parts.size() == 4) {
                // Fallback: no details field
                HistEntry e;
                e.date    = parts.at(0).trimmed();
                e.labNom  = parts.at(1).trimmed();
                e.type    = parts.at(2).trimmed();
                e.user    = parts.at(3).trimmed();
                e.details = "";
                entries.append(e);
            } else if (!line.trimmed().isEmpty()) {
                // Legacy format: store as-is under lab name
                HistEntry e;
                e.date    = "";
                e.labNom  = labNom;
                e.type    = "Modification";
                e.user    = "admin";
                e.details = line.trimmed();
                entries.append(e);
            }
        }
    }

    // Sort by date DESC (string sort works for dd/MM/yyyy hh:mm if we convert)
    std::sort(entries.begin(), entries.end(), [](const HistEntry &a, const HistEntry &b) {
        // Parse "dd/MM/yyyy hh:mm" for comparison
        QDateTime da = QDateTime::fromString(a.date, "dd/MM/yyyy hh:mm");
        QDateTime db2 = QDateTime::fromString(b.date, "dd/MM/yyyy hh:mm");
        if (da.isValid() && db2.isValid()) return da > db2;
        return a.date > b.date;
    });

    // Fill historiqueTable (5 cols: DATE | LABORATOIRE | TYPE | UTILISATEUR | DETAILS)
    ui->historiqueTable->setRowCount(0);
    ui->historiqueTable->setRowCount(entries.size());

    for (int row = 0; row < entries.size(); ++row) {
        const HistEntry &e = entries.at(row);
        ui->historiqueTable->setItem(row, 0, new QTableWidgetItem(e.date));
        ui->historiqueTable->setItem(row, 1, new QTableWidgetItem(e.labNom));
        ui->historiqueTable->setItem(row, 2, new QTableWidgetItem(e.type));
        ui->historiqueTable->setItem(row, 3, new QTableWidgetItem(e.user));
        ui->historiqueTable->setItem(row, 4, new QTableWidgetItem(e.details));
    }

    ui->historiqueTable->resizeColumnsToContents();
    ui->historiqueTable->horizontalHeader()->setStretchLastSection(true);
}

// ============================================================
//  onSupprimerHistoriqueClicked -- clear selected lab's historique CLOB
// ============================================================
void Laboratoire::onSupprimerHistoriqueClicked()
{
    int row = ui->historiqueTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Supprimer Historique",
            "Veuillez selectionner une entree dans l'historique.");
        return;
    }

    // Get lab name from column 1 (LABORATOIRE)
    QTableWidgetItem *nomItem = ui->historiqueTable->item(row, 1);
    if (!nomItem || nomItem->text().isEmpty()) {
        QMessageBox::warning(this, "Supprimer Historique",
            "Impossible de determiner le laboratoire associe.");
        return;
    }
    QString labNom = nomItem->text();

    if (labConfirmDialog(this, "Supprimer Historique",
            QString("Effacer tout l'historique du laboratoire \"%1\" ?").arg(labNom))
            != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase db = Connection::createInstance().getDb();
    QSqlQuery query(db);
    query.prepare("UPDATE LABORATOIRE SET historique = NULL WHERE NOM = :nom");
    query.bindValue(":nom", labNom);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
            QString("Impossible d'effacer l'historique.\n\nErreur :\n%1")
                .arg(query.lastError().text()));
        return;
    }

    QMessageBox::information(this, "Succes",
        QString("Historique du laboratoire \"%1\" efface avec succes.").arg(labNom));
    refreshHistorique();
}

// ============================================================
//  onExporterHistPDFClicked -- export historiqueTable to PDF
// ============================================================
void Laboratoire::onExporterHistPDFClicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Exporter Historique PDF", "historique_laboratoires.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QString html;
    html += "<html><head><meta charset='UTF-8'></head><body>";
    html += "<h2 style='color:#8B5CF6;'>Historique des Modifications - Laboratoires</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%' "
            "style='border-collapse:collapse; font-size:10px;'>";
    html += "<tr style='background-color:#8B5CF6; color:white;'>"
            "<th>DATE</th><th>LABORATOIRE</th><th>TYPE DE MODIFICATION</th>"
            "<th>UTILISATEUR</th><th>DETAILS</th></tr>";

    int rowCount = ui->historiqueTable->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QString bg = (i % 2 == 0) ? "#FFFFFF" : "#F3F4F6";
        html += QString("<tr style='background-color:%1;'>").arg(bg);
        for (int col = 0; col < 5; ++col) {
            QTableWidgetItem *item = ui->historiqueTable->item(i, col);
            QString text = item ? item->text().toHtmlEscaped() : "";
            html += QString("<td>%1</td>").arg(text);
        }
        html += "</tr>";
    }

    html += "</table>";
    html += QString("<p style='color:#6B7280; font-size:10px;'>%1 entree(s) - "
                    "Exporte le %2</p>")
                .arg(rowCount)
                .arg(QDate::currentDate().toString("dd/MM/yyyy"));
    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10));
    printer.setPageOrientation(QPageLayout::Landscape);

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);

    QMessageBox::information(this, "Exporter PDF",
        QString("Historique exporte avec succes !\n%1").arg(filePath));
}

// ============================================================
//  verifierAlertesBudget -- labs > 80% of max budget
// ============================================================
void Laboratoire::verifierAlertesBudget(bool silencieux)
{
    if (!Connection::createInstance().getDb().isOpen()) return;

    // Calculate max budget reference
    QSqlDatabase db = Connection::createInstance().getDb();
    QSqlQuery q(db);
    double budgetMax = 0;
    if (q.exec("SELECT MAX(BUDGET) FROM LABORATOIRE") && q.next()) {
        budgetMax = q.value(0).toDouble();
    }
    if (budgetMax <= 0) return;

    QList<LaboratoireModel> alertes =
        LaboratoireModel::getLabsDepassantBudget(80.0, budgetMax);

    if (alertes.isEmpty()) {
        if (!silencieux) {
            QMessageBox::information(this, "Alerte Budget",
                "Aucun laboratoire ne depasse 80% du budget maximum.\n"
                "Tout est dans les normes.");
        }
        return;
    }

    // Build alert table HTML
    QString msg = QString(
        "<b style='color:#F97316;'>\xe2\x9a\xa0 %1 laboratoire(s) depassent 80%% "
        "du budget maximum (%2 DT)</b><br><br>")
            .arg(alertes.size())
            .arg(QString::number(budgetMax, 'f', 2));

    msg += "<table border='1' cellpadding='6' cellspacing='0' width='100%' "
           "style='border-collapse:collapse; font-size:12px;'>";
    msg += "<tr style='background:#FEF3C7;'>"
           "<th>Nom</th><th>Budget (DT)</th><th>% du Max</th>"
           "<th>Domaine</th><th>Classement</th></tr>";

    for (const LaboratoireModel &lab : alertes) {
        double pct = (lab.getBudget() / budgetMax) * 100.0;
        QString couleur = (pct >= 100.0) ? "#FEE2E2" : "#FEF9C3";
        msg += QString(
            "<tr style='background:%1;'>"
            "<td><b>%2</b></td>"
            "<td>%3</td>"
            "<td><b>%4%</b></td>"
            "<td>%5</td>"
            "<td>%6</td>"
            "</tr>")
                .arg(couleur)
                .arg(lab.getNom().toHtmlEscaped())
                .arg(QString::number(lab.getBudget(), 'f', 2))
                .arg(QString::number(pct, 'f', 1))
                .arg(lab.getDomaine().toHtmlEscaped())
                .arg(lab.getClassement().toHtmlEscaped());
    }
    msg += "</table>";

    // Build dialog with QScrollArea
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("Alerte Budget - Laboratoires");
    dialog->resize(780, 420);

    auto *label = new QLabel(dialog);
    label->setText(msg);
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(true);
    label->setStyleSheet("padding:10px;");

    auto *container = new QWidget();
    auto *cLayout   = new QVBoxLayout(container);
    cLayout->addWidget(label);
    container->setLayout(cLayout);

    auto *scroll = new QScrollArea(dialog);
    scroll->setWidget(container);
    scroll->setWidgetResizable(true);

    auto *closeBtn = new QPushButton("Fermer", dialog);
    closeBtn->setStyleSheet(
        "QPushButton { background-color:#F97316; color:white; border:none; "
        "border-radius:6px; padding:8px 24px; font-size:13px; }"
        "QPushButton:hover { background-color:#EA580C; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    auto *layout = new QVBoxLayout(dialog);
    layout->addWidget(scroll);
    layout->addWidget(closeBtn);
    dialog->setLayout(layout);
    dialog->exec();
}

// ============================================================
//  onAlerteBudgetClicked
// ============================================================
void Laboratoire::onAlerteBudgetClicked()
{
    verifierAlertesBudget(false);
}
