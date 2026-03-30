#include "submission.h"
#include "ui_submission.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <QDate>

Submission::Submission(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Submission)
    , selectedRow(-1)
{
    ui->setupUi(this);

    setupTable();
    setupConnections();
    loadArticlesCombo();
    loadUsersCombo();
    loadConferencesCombo();
    loadSubmissions();
}

Submission::~Submission()
{
    delete ui;
}

// ─────────────────────────── SETUP ───────────────────────────
void Submission::setupTable()
{
    ui->submissionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->submissionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->submissionTable->verticalHeader()->setVisible(false);
    ui->submissionTable->horizontalHeader()->setStretchLastSection(true);
    ui->submissionTable->setAlternatingRowColors(true);
}

void Submission::setupConnections()
{
    connect(ui->addBtn,    &QPushButton::clicked, this, &Submission::onAddSubmission);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &Submission::onDeleteSubmission);
    connect(ui->clearBtn,  &QPushButton::clicked, this, &Submission::clearForm);
    connect(ui->comboArticle, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Submission::onArticleChanged);
    connect(ui->sortBtn,   &QPushButton::clicked, this, &Submission::onSortClicked);
    connect(ui->searchBtn, &QPushButton::clicked, this, &Submission::onSearchClicked);
    connect(ui->statsBtn,  &QPushButton::clicked, this, &Submission::onStatisticsClicked);
    connect(ui->submissionTable, &QTableWidget::itemSelectionChanged,
            this, &Submission::onTableSelectionChanged);
}

// ─────────────────────────── POPULATE COMBOS ─────────────────
void Submission::loadUsersCombo()
{
    ui->comboUser->clear();
    ui->comboUser->addItem("-- Selectionnez un chercheur --", -1);

    QSqlQuery q(Connection::createInstance().getDb());
    q.exec("SELECT ID_user, nom, prenom, email FROM USERS ORDER BY nom");
    while (q.next()) {
        QString label = q.value("nom").toString() + " " + q.value("prenom").toString()
                      + "  <" + q.value("email").toString() + ">";
        ui->comboUser->addItem(label, q.value("ID_user").toInt());
    }
}

void Submission::loadArticlesCombo()
{
    ui->comboArticle->clear();
    ui->comboArticle->addItem("-- Selectionnez un article --", -1);

    QSqlQuery q(Connection::createInstance().getDb());
    // Show: titre — by auteur
    q.exec(
        "SELECT A.ID_article, A.titre, "
        "NVL(U.nom || ' ' || U.prenom, 'Auteur inconnu') AS auteur "
        "FROM ARTICLE A "
        "LEFT JOIN USERS U ON A.ID_user = U.ID_user "
        "ORDER BY A.Date_creation DESC"
    );

    if (!q.isActive()) {
        // Fallback without join
        QSqlQuery q2(Connection::createInstance().getDb());
        q2.exec("SELECT ID_article, titre FROM ARTICLE ORDER BY Date_creation DESC");
        while (q2.next())
            ui->comboArticle->addItem(q2.value("titre").toString(), q2.value("ID_article"));
        return;
    }

    while (q.next()) {
        QString label = q.value("titre").toString()
                      + "   [par " + q.value("auteur").toString() + "]";
        ui->comboArticle->addItem(label, q.value("ID_article").toInt());
    }

    if (ui->comboArticle->count() <= 1)
        ui->comboArticle->addItem("Aucun article — ajoutez-en dans Publications", -1);
}

void Submission::loadConferencesCombo()
{
    ui->comboConference->clear();
    ui->comboConference->addItem("-- Aucune (soumission libre) --", -1);

    QSqlQuery q(Connection::createInstance().getDb());
    q.exec(
        "SELECT ID_conference, nom, lieu, Date_debut "
        "FROM CONFERENCE "
        "WHERE etat IN ('Ouverte', 'En cours') "
        "ORDER BY Date_debut ASC"
    );

    // If no open conferences, load all
    if (!q.isActive() || !q.isSelect()) {
        QSqlQuery q2(Connection::createInstance().getDb());
        q2.exec("SELECT ID_conference, nom FROM CONFERENCE ORDER BY nom");
        while (q2.next())
            ui->comboConference->addItem(q2.value("nom").toString(), q2.value("ID_conference"));
        return;
    }

    int count = 0;
    while (q.next()) {
        QString label = q.value("nom").toString()
                      + " — " + q.value("lieu").toString()
                      + " (" + q.value("Date_debut").toDate().toString("dd/MM/yyyy") + ")";
        ui->comboConference->addItem(label, q.value("ID_conference").toInt());
        count++;
    }

    // If still empty, load all conferences regardless of status
    if (count == 0) {
        QSqlQuery q3(Connection::createInstance().getDb());
        q3.exec("SELECT ID_conference, nom FROM CONFERENCE ORDER BY nom");
        while (q3.next())
            ui->comboConference->addItem(q3.value("nom").toString(), q3.value("ID_conference"));
    }
}

// ─────────────────────────── LOAD TABLE ──────────────────────
void Submission::loadSubmissions(const QString &filter)
{
    QSqlQuery query(Connection::createInstance().getDb());

    QString sql =
        "SELECT S.ID_soumission, "
        "       A.titre                                       AS article, "
        "       NVL(C.nom, 'Libre')                          AS conference, "
        "       NVL(U.nom || ' ' || U.prenom, 'Inconnu')     AS auteur, "
        "       S.Date_envoi, "
        "       S.Etat "
        "FROM SOUMISSION S "
        "LEFT JOIN ARTICLE    A ON S.ID_article    = A.ID_article "
        "LEFT JOIN CONFERENCE C ON S.ID_conference = C.ID_conference "
        "LEFT JOIN USERS      U ON S.ID_user       = U.ID_user ";

    if (!filter.isEmpty())
        sql += "WHERE " + filter + " ";

    sql += "ORDER BY S.Date_envoi DESC";

    if (!query.exec(sql)) {
        qDebug() << "loadSubmissions error:" << query.lastError().text();
        return;
    }

    ui->submissionTable->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->submissionTable->insertRow(row);

        QString etat = query.value("Etat").toString();

        auto item = [&](const QString &val) {
            auto *it = new QTableWidgetItem(val);
            it->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            return it;
        };

        ui->submissionTable->setItem(row, 0, item(query.value("ID_soumission").toString()));
        ui->submissionTable->setItem(row, 1, item(query.value("article").toString()));
        ui->submissionTable->setItem(row, 2, item(query.value("conference").toString()));
        ui->submissionTable->setItem(row, 3, item(query.value("auteur").toString()));
        ui->submissionTable->setItem(row, 4, item(
            query.value("Date_envoi").toDate().toString("dd/MM/yyyy")));
        ui->submissionTable->setItem(row, 5, item(etat));

        // Color code by status
        QColor color;
        if      (etat == "En attente")  color = QColor("#FEF9C3"); // yellow
        else if (etat == "En revision") color = QColor("#DBEAFE"); // blue
        else if (etat == "Accepte")     color = QColor("#DCFCE7"); // green
        else if (etat == "Refuse")      color = QColor("#FEE2E2"); // red
        else                            color = QColor("#F8FAFC");

        for (int col = 0; col < 6; col++)
            if (ui->submissionTable->item(row, col))
                ui->submissionTable->item(row, col)->setBackground(color);

        row++;
    }

    ui->submissionTable->resizeColumnsToContents();
    ui->submissionTable->setColumnWidth(0, 50);
    ui->submissionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

// ─────────────────────────── ADD ─────────────────────────────
void Submission::onAddSubmission()
{
    int articleId = ui->comboArticle->currentData().toInt();
    if (articleId <= 0) {
        QMessageBox::warning(this, "Validation",
            "Veuillez selectionner un article.\n\n"
            "Si la liste est vide, ajoutez d'abord un article dans 'Publications'.");
        return;
    }

    int confId = ui->comboConference->currentData().toInt(); // -1 = libre
    QString etat = ui->statusCombo->currentText();

    // Get user from comboUser selection
    int userId = ui->comboUser->currentData().toInt();
    if (userId <= 0) {
        // Fallback: get from article's author field
        QSqlQuery qa(Connection::createInstance().getDb());
        qa.prepare("SELECT ID_user FROM ARTICLE WHERE ID_article = :id");
        qa.bindValue(":id", articleId);
        if (qa.exec() && qa.next())
            userId = qa.value("ID_user").toInt();
    }

    // Check for duplicate submission (same article + same conference)
    if (confId > 0) {
        QSqlQuery dup(Connection::createInstance().getDb());
        dup.prepare("SELECT COUNT(*) FROM SOUMISSION WHERE ID_article=:a AND ID_conference=:c");
        dup.bindValue(":a", articleId);
        dup.bindValue(":c", confId);
        if (dup.exec() && dup.next() && dup.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Doublon",
                "Cet article a deja ete soumis a cette conference !");
            return;
        }
    }

    // INSERT — split into variants to avoid QVariant() null-binding bug with Oracle ODBC
    // Reason: QVariant() (invalid type) is sent as BINARY by Qt -> ORA-00932
    QSqlQuery ins(Connection::createInstance().getDb());

    if (confId > 0 && userId > 0) {
        ins.prepare("INSERT INTO SOUMISSION (ID_article, ID_conference, ID_user, Date_envoi, Etat) "
                    "VALUES (:art, :conf, :usr, SYSDATE, :etat)");
        ins.bindValue(":art",  articleId);
        ins.bindValue(":conf", confId);
        ins.bindValue(":usr",  userId);
        ins.bindValue(":etat", etat);
    } else if (confId > 0) {
        ins.prepare("INSERT INTO SOUMISSION (ID_article, ID_conference, Date_envoi, Etat) "
                    "VALUES (:art, :conf, SYSDATE, :etat)");
        ins.bindValue(":art",  articleId);
        ins.bindValue(":conf", confId);
        ins.bindValue(":etat", etat);
    } else if (userId > 0) {
        ins.prepare("INSERT INTO SOUMISSION (ID_article, ID_user, Date_envoi, Etat) "
                    "VALUES (:art, :usr, SYSDATE, :etat)");
        ins.bindValue(":art", articleId);
        ins.bindValue(":usr", userId);
        ins.bindValue(":etat", etat);
    } else {
        ins.prepare("INSERT INTO SOUMISSION (ID_article, Date_envoi, Etat) "
                    "VALUES (:art, SYSDATE, :etat)");
        ins.bindValue(":art",  articleId);
        ins.bindValue(":etat", etat);
    }

    if (!ins.exec()) {
        QMessageBox::critical(this, "Erreur base de donnees",
            "Impossible de creer la soumission:\n" + ins.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes",
        "Article soumis avec succes !\n\nIl sera maintenant disponible pour evaluation.");

    clearForm();
    loadSubmissions();
    ui->tabWidget->setCurrentIndex(1);

}

// ─────────────────────────── DELETE ──────────────────────────
void Submission::onDeleteSubmission()
{
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Selection", "Selectionnez une soumission.");
        return;
    }

    int id        = ui->submissionTable->item(selectedRow, 0)->text().toInt();
    QString titre = ui->submissionTable->item(selectedRow, 1)->text();

    int rep = QMessageBox::question(this, "Confirmation",
        QString("Supprimer la soumission de:\n\"%1\" ?\n\n"
                "ATTENTION: les evaluations liees seront aussi supprimees.").arg(titre),
        QMessageBox::Yes | QMessageBox::No);
    if (rep != QMessageBox::Yes) return;

    // Delete linked evaluations first
    QSqlQuery qe(Connection::createInstance().getDb());
    qe.prepare("DELETE FROM Evaluation WHERE soumission_id = :id");
    qe.bindValue(":id", id);
    qe.exec();

    // Delete soumission
    QSqlQuery qs(Connection::createInstance().getDb());
    qs.prepare("DELETE FROM SOUMISSION WHERE ID_soumission = :id");
    qs.bindValue(":id", id);

    if (!qs.exec()) {
        QMessageBox::critical(this, "Erreur", qs.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes", "Soumission supprimee.");
    selectedRow = -1;
    ui->deleteBtn->setEnabled(false);
    loadSubmissions();
}

// ─────────────────────────── SELECTION ───────────────────────
void Submission::onTableSelectionChanged()
{
    selectedRow = ui->submissionTable->currentRow();
    ui->deleteBtn->setEnabled(selectedRow >= 0);
}

// ─────────────────────────── FORM ────────────────────────────
void Submission::clearForm()
{
    loadArticlesCombo();
    loadUsersCombo();
    loadConferencesCombo();
    ui->comboArticle->setCurrentIndex(0);
    ui->comboUser->setCurrentIndex(0);
    ui->comboConference->setCurrentIndex(0);
    ui->statusCombo->setCurrentIndex(0);
}

void Submission::onArticleChanged(int /*index*/)
{
    // Auto-select the article's author in comboUser when article changes
    int articleId = ui->comboArticle->currentData().toInt();
    if (articleId <= 0) return;

    QSqlQuery q(Connection::createInstance().getDb());
    q.prepare("SELECT ID_user FROM ARTICLE WHERE ID_article = :id");
    q.bindValue(":id", articleId);
    if (q.exec() && q.next()) {
        int authorId = q.value("ID_user").toInt();
        int idx = ui->comboUser->findData(authorId);
        if (idx >= 0) ui->comboUser->setCurrentIndex(idx);
    }
}

// ─────────────────────────── SEARCH / SORT / STATS ───────────
void Submission::onSearchClicked()
{
    QString text = ui->searchEdit->text().trimmed();
    if (text.isEmpty()) { loadSubmissions(); return; }
    loadSubmissions(QString("LOWER(A.titre) LIKE LOWER('%%%1%')").arg(text));
}

void Submission::onSortClicked()
{
    ui->submissionTable->sortItems(4, Qt::DescendingOrder);
}

void Submission::onExportClicked()
{
    QMessageBox::information(this, "Export", "Export PDF a implementer.");
}

void Submission::onStatisticsClicked()
{
    QSqlQuery q(Connection::createInstance().getDb());
    q.exec("SELECT Etat, COUNT(*) as cnt FROM SOUMISSION GROUP BY Etat ORDER BY cnt DESC");
    QString stats = "📊 Statistiques des Soumissions\n\n";
    int total = 0;
    while (q.next()) {
        int cnt = q.value("cnt").toInt();
        stats += QString("  %-15 : %2\n").arg(q.value("Etat").toString()).arg(cnt);
        total += cnt;
    }
    stats += QString("\n  Total : %1").arg(total);
    QMessageBox::information(this, "Statistiques", stats);
}
