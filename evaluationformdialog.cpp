#include "evaluationformdialog.h"
#include "ui_evaluationformdialog.h"
#include "database.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ─────────────────────────── CONSTRUCTORS ────────────────────
EvaluationFormDialog::EvaluationFormDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EvaluationFormDialog)
    , editMode(false)
    , evaluationId(0)
    , resolvedSubmissionId(-1)
{
    ui->setupUi(this);
    setupForm();
}

EvaluationFormDialog::EvaluationFormDialog(const Evaluation& eval, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EvaluationFormDialog)
    , editMode(true)
    , evaluationId(eval.getId())
    , resolvedSubmissionId(eval.getSubmissionId())
{
    ui->setupUi(this);
    setupForm();
    populateForm(eval);
}

EvaluationFormDialog::~EvaluationFormDialog()
{
    delete ui;
}

// ─────────────────────────── SETUP ───────────────────────────
void EvaluationFormDialog::setupForm()
{
    setWindowTitle(editMode ? "Modifier l'evaluation" : "Nouvelle evaluation");
    setMinimumWidth(500);

    loadArticles();
    loadReviewers();

    ui->spinBoxNote->setRange(1, 10);
    ui->spinBoxNote->setValue(5);
    ui->dateEditEval->setDate(QDate::currentDate());

    connect(ui->btnSave,   &QPushButton::clicked,        this, &EvaluationFormDialog::onSave);
    connect(ui->btnCancel, &QPushButton::clicked,        this, &EvaluationFormDialog::onCancel);
    connect(ui->spinBoxNote, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EvaluationFormDialog::onNoteChanged);

    onNoteChanged(ui->spinBoxNote->value());
}

// ─────────────────────────── LOAD COMBOS ─────────────────────

void EvaluationFormDialog::loadArticles()
{
    ui->comboBoxArticle->clear();

    QSqlQuery query(Connection::createInstance().getDb());
    // Load articles with their author name for clarity
    query.exec(
        "SELECT A.ID_article, A.titre, NVL(U.nom || ' ' || U.prenom, 'Inconnu') AS auteur "
        "FROM ARTICLE A "
        "LEFT JOIN USERS U ON A.ID_user = U.ID_user "
        "ORDER BY A.Date_creation DESC"
    );

    if (!query.isActive()) {
        qDebug() << "loadArticles error:" << query.lastError().text();
        // Fallback: load without join
        QSqlQuery q2(Connection::createInstance().getDb());
        q2.exec("SELECT ID_article, titre FROM ARTICLE ORDER BY Date_creation DESC");
        while (q2.next()) {
            ui->comboBoxArticle->addItem(
                q2.value("titre").toString(),
                q2.value("ID_article").toInt()
            );
        }
        return;
    }

    while (query.next()) {
        QString label = QString("[%1] %2 — par %3")
            .arg(query.value("ID_article").toString())
            .arg(query.value("titre").toString())
            .arg(query.value("auteur").toString());
        ui->comboBoxArticle->addItem(label, query.value("ID_article").toInt());
    }

    if (ui->comboBoxArticle->count() == 0) {
        ui->comboBoxArticle->addItem("-- Aucun article disponible --", -1);
    }
}

void EvaluationFormDialog::loadReviewers()
{
    ui->comboBoxReviewer->clear();

    QSqlQuery query(Connection::createInstance().getDb());
    query.exec("SELECT ID_user, nom, email FROM USERS WHERE Role='Relecteur' ORDER BY nom");

    while (query.next()) {
        QString label = query.value("nom").toString()
                      + " <" + query.value("email").toString() + ">";
        ui->comboBoxReviewer->addItem(label, query.value("ID_user").toInt());
    }

    if (ui->comboBoxReviewer->count() == 0) {
        ui->comboBoxReviewer->addItem("-- Aucun relecteur (ajoutez un User avec Role=Relecteur) --", -1);
    }
}

// ─────────────────────────── GET/CREATE SOUMISSION ───────────
int EvaluationFormDialog::getOrCreateSubmission(int articleId)
{
    QSqlQuery query(Connection::createInstance().getDb());

    // 1. Check if a soumission already exists for this article
    query.prepare("SELECT ID_soumission FROM SOUMISSION WHERE ID_article = :id AND ROWNUM = 1");
    query.bindValue(":id", articleId);
    if (query.exec() && query.next()) {
        int existingId = query.value("ID_soumission").toInt();
        qDebug() << "Found existing SOUMISSION id=" << existingId;
        return existingId;
    }

    // 2. No soumission exists — create one automatically
    QSqlQuery ins(Connection::createInstance().getDb());
    ins.prepare(
        "INSERT INTO SOUMISSION (ID_article, Date_envoi, Etat) "
        "VALUES (:art, SYSDATE, 'En revision')"
    );
    ins.bindValue(":art", articleId);

    if (!ins.exec()) {
        qDebug() << "Failed to create SOUMISSION:" << ins.lastError().text();
        return -1;
    }

    // 3. Retrieve the new ID
    QSqlQuery sel(Connection::createInstance().getDb());
    sel.prepare("SELECT MAX(ID_soumission) FROM SOUMISSION WHERE ID_article = :id");
    sel.bindValue(":id", articleId);
    if (sel.exec() && sel.next()) {
        int newId = sel.value(0).toInt();
        qDebug() << "Created SOUMISSION id=" << newId;
        return newId;
    }

    return -1;
}

// ─────────────────────────── POPULATE (edit mode) ────────────
void EvaluationFormDialog::populateForm(const Evaluation& eval)
{
    // In edit mode we already have resolvedSubmissionId from constructor
    // Try to select the matching article in the combo
    // (best-effort since we only store soumission_id, not article_id directly)

    int index = ui->comboBoxReviewer->findData(eval.getReviewerId());
    if (index != -1) ui->comboBoxReviewer->setCurrentIndex(index);

    ui->spinBoxNote->setValue(eval.getNote());
    ui->textEditCommentaires->setPlainText(eval.getCommentaires());
    ui->dateEditEval->setDate(eval.getDateEvaluation());

    if (eval.getStatut() == Evaluation::EnCours)
        ui->radioEnCours->setChecked(true);
    else
        ui->radioTerminee->setChecked(true);
}

// ─────────────────────────── GET RESULT ──────────────────────
Evaluation EvaluationFormDialog::getEvaluation() const
{
    Evaluation eval;
    if (editMode) eval.setId(evaluationId);

    eval.setSubmissionId(resolvedSubmissionId); // Set by onSave()
    eval.setReviewerId(ui->comboBoxReviewer->currentData().toInt());
    eval.setNote(ui->spinBoxNote->value());
    eval.setCommentaires(ui->textEditCommentaires->toPlainText());
    eval.setDateEvaluation(ui->dateEditEval->date());
    eval.setStatut(ui->radioEnCours->isChecked() ? Evaluation::EnCours : Evaluation::Terminee);

    return eval;
}

// ─────────────────────────── SLOTS ───────────────────────────
void EvaluationFormDialog::onSave()
{
    // Validate article selection
    int articleId = ui->comboBoxArticle->currentData().toInt();
    if (articleId <= 0) {
        QMessageBox::warning(this, "Validation",
            "Veuillez selectionner un article.\n\n"
            "Si la liste est vide, ajoutez d'abord un article dans 'Publications'.");
        return;
    }

    // Validate reviewer selection
    int reviewerId = ui->comboBoxReviewer->currentData().toInt();
    if (reviewerId <= 0) {
        QMessageBox::warning(this, "Validation",
            "Veuillez selectionner un relecteur.\n\n"
            "Si la liste est vide, allez dans 'Users' et ajoutez un utilisateur "
            "avec le role 'Relecteur'.");
        return;
    }

    // Auto-create or reuse soumission for this article
    if (!editMode) {
        resolvedSubmissionId = getOrCreateSubmission(articleId);
        if (resolvedSubmissionId <= 0) {
            QMessageBox::critical(this, "Erreur",
                "Impossible de creer la soumission pour cet article.\n"
                "Verifiez la connexion Oracle.");
            return;
        }
    }

    accept();
}

void EvaluationFormDialog::onCancel()
{
    reject();
}

void EvaluationFormDialog::onNoteChanged(int value)
{
    QString text = QString::number(value) + "/10";
    ui->lblNoteValue->setText(text);

    QString color;
    if      (value <= 3)  color = "#EF4444";  // red
    else if (value <= 5)  color = "#F59E0B";  // orange
    else if (value <= 7)  color = "#3B82F6";  // blue
    else                  color = "#10B981";  // green

    ui->lblNoteValue->setStyleSheet(
        QString("font-weight: 700; font-size: 16px; color: %1;").arg(color));
}
