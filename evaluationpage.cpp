#include "evaluationpage.h"
#include "ui_evaluationpage.h"
#include "evaluation.h"
#include "evaluationformdialog.h"
#include "database.h"
#include "reviewer.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPageLayout>
#include <QDate>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>

#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QBarCategoryAxis>

EvaluationPage::EvaluationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvaluationPage),
    model(new QStandardItemModel(this)),
    reviewerModel(new QStandardItemModel(this)),
    reviewerEvalModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setupTable();
    setupReviewerTab();
    setupConnections();
    loadEvaluations();
    setupCharts();
    updateStatLabels();
}

EvaluationPage::~EvaluationPage()
{
    delete ui;
}

// ═══════════════════════ LIST TAB ═══════════════════════

void EvaluationPage::setupTable()
{
    model->setHorizontalHeaderLabels(
        {"ID", "Soumission", "Relecteur", "Note", "Date", "Statut"});
    ui->tableViewEvaluations->setModel(model);
    ui->tableViewEvaluations->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewEvaluations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewEvaluations->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewEvaluations->setAlternatingRowColors(true);
}

void EvaluationPage::setupConnections()
{
    connect(ui->btnCreate,  &QPushButton::clicked, this, &EvaluationPage::onCreateEvaluation);
    connect(ui->btnEdit,    &QPushButton::clicked, this, &EvaluationPage::onEditEvaluation);
    connect(ui->btnDelete,  &QPushButton::clicked, this, &EvaluationPage::onDeleteEvaluation);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &EvaluationPage::onExportPDF);
    connect(ui->btnRefreshList, &QPushButton::clicked, this, &EvaluationPage::onRefreshList);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &EvaluationPage::onSearch);
    connect(ui->comboBoxFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EvaluationPage::onFilterByStatus);
    connect(ui->btnSortDate, &QPushButton::clicked, this, &EvaluationPage::onSortByDate);
    connect(ui->btnSortNote, &QPushButton::clicked, this, &EvaluationPage::onSortByNote);

    connect(ui->btnRefreshStats, &QPushButton::clicked, this, &EvaluationPage::onRefreshStats);
    connect(ui->btnExportStats,  &QPushButton::clicked, this, &EvaluationPage::onExportStats);

    // Reviewer tab
    connect(ui->listViewReviewers, &QListView::clicked, this, &EvaluationPage::onReviewerSelected);
    connect(ui->btnAddReviewer,     &QPushButton::clicked, this, &EvaluationPage::onAddReviewer);
    connect(ui->btnRefreshReviewers,&QPushButton::clicked, this, &EvaluationPage::onRefreshReviewers);
}

void EvaluationPage::loadEvaluations()
{
    model->removeRows(0, model->rowCount());

    QVector<Evaluation> evaluations = Database::instance().getAllEvaluations();

    for (const Evaluation& eval : evaluations) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(eval.getId()));
        row << new QStandardItem(QString::number(eval.getSubmissionId()));

        // Get reviewer name from DB
        Reviewer reviewer = Database::instance().getReviewerById(eval.getReviewerId());
        row << new QStandardItem(reviewer.getNom());

        row << new QStandardItem(QString::number(eval.getNote()));
        row << new QStandardItem(eval.getDateEvaluation().toString("dd/MM/yyyy"));

        QString statut = eval.getStatutString();
        QStandardItem* statusItem = new QStandardItem(statut);
        if (eval.getStatut() == Evaluation::EnCours)
            statusItem->setForeground(QBrush(QColor("#f39c12")));
        else
            statusItem->setForeground(QBrush(QColor("#27ae60")));
        row << statusItem;

        model->appendRow(row);
    }

    ui->tableViewEvaluations->resizeColumnsToContents();
}

void EvaluationPage::onCreateEvaluation()
{
    EvaluationFormDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Evaluation eval = dialog.getEvaluation();
        if (Database::instance().createEvaluation(eval)) {
            QMessageBox::information(this, "Succès", "Évaluation créée avec succès.");
            loadEvaluations();
            updateStatLabels();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de créer l'évaluation.");
        }
    }
}

void EvaluationPage::onEditEvaluation()
{
    QModelIndex index = ui->tableViewEvaluations->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une évaluation.");
        return;
    }

    int evalId = model->item(index.row(), 0)->text().toInt();
    Evaluation eval = Database::instance().getEvaluationById(evalId);

    EvaluationFormDialog dialog(eval, this);
    if (dialog.exec() == QDialog::Accepted) {
        Evaluation updated = dialog.getEvaluation();
        if (Database::instance().updateEvaluation(updated)) {
            QMessageBox::information(this, "Succès", "Évaluation mise à jour.");
            loadEvaluations();
            updateStatLabels();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de mettre à jour.");
        }
    }
}

void EvaluationPage::onDeleteEvaluation()
{
    QModelIndex index = ui->tableViewEvaluations->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une évaluation.");
        return;
    }

    int row = index.row();
    int evalId = model->item(row, 0)->text().toInt();
    QString soumission = model->item(row, 1)->text();
    QString relecteur  = model->item(row, 2)->text();

    // Ask for reason (conflit d'intérêt, etc.)
    QDialog dlg(this);
    dlg.setWindowTitle("Supprimer l'évaluation");
    dlg.setMinimumWidth(420);

    QVBoxLayout* layout = new QVBoxLayout(&dlg);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    layout->addWidget(new QLabel(
        QString("<b>Soumission #%1 — Relecteur : %2</b>").arg(soumission).arg(relecteur), &dlg));
    layout->addWidget(new QLabel("Motif de suppression :", &dlg));

    QTextEdit* motifEdit = new QTextEdit(&dlg);
    motifEdit->setPlaceholderText("ex: Conflit d'intérêt, doublon, erreur de saisie…");
    motifEdit->setFixedHeight(80);
    layout->addWidget(motifEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    if (motifEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Motif requis",
            "Veuillez indiquer un motif pour la suppression.");
        return;
    }

    if (Database::instance().deleteEvaluation(evalId)) {
        QMessageBox::information(this, "Supprimée",
            QString("Évaluation #%1 supprimée.\nMotif : %2")
                .arg(evalId).arg(motifEdit->toPlainText().trimmed()));
        loadEvaluations();
        updateStatLabels();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer l'évaluation.");
    }
}

void EvaluationPage::onExportPDF()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Exporter en PDF",
        QDir::homePath() + "/evaluations.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);

    QString html;
    html += "<html><body style='font-family:Arial;'>";
    html += "<h2 style='text-align:center;color:#9f7aea;'>Liste des Évaluations</h2>";
    html += "<p style='text-align:right;color:gray;'>Exporté le : "
            + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table border='1' cellspacing='0' cellpadding='5' width='100%' "
            "style='border-collapse:collapse;'>";
    html += "<tr style='background-color:#9f7aea;color:white;font-weight:bold;'>"
            "<th>ID</th><th>Soumission</th><th>Relecteur</th>"
            "<th>Note</th><th>Date</th><th>Statut</th></tr>";

    for (int i = 0; i < model->rowCount(); ++i) {
        if (ui->tableViewEvaluations->isRowHidden(i)) continue;
        QString bg = (i % 2 == 0) ? "#ffffff" : "#f5f0ff";
        html += QString("<tr style='background-color:%1;'>").arg(bg);
        for (int j = 0; j < model->columnCount(); ++j) {
            html += "<td style='padding:4px;'>" + model->item(i, j)->text() + "</td>";
        }
        html += "</tr>";
    }
    html += "</table></body></html>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Export PDF", "PDF exporté :\n" + fileName);
}

void EvaluationPage::onSearch(const QString& text)
{
    for (int i = 0; i < model->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < model->columnCount(); ++j) {
            if (model->item(i, j)->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->tableViewEvaluations->setRowHidden(i, !match);
    }
}

void EvaluationPage::onFilterByStatus(int index)
{
    if (index == 0) { // Tous
        for (int i = 0; i < model->rowCount(); ++i)
            ui->tableViewEvaluations->setRowHidden(i, false);
    } else {
        QString status = (index == 1) ? "En cours" : "Terminée";
        for (int i = 0; i < model->rowCount(); ++i) {
            ui->tableViewEvaluations->setRowHidden(
                i, model->item(i, 5)->text() != status);
        }
    }
}

void EvaluationPage::onSortByDate()
{
    model->sort(4); // Date column
}

void EvaluationPage::onSortByNote()
{
    model->sort(3); // Note column
}

void EvaluationPage::onRefreshList()
{
    loadEvaluations();
}

// ═══════════════════════ STATISTICS TAB ═══════════════════════

void EvaluationPage::updateStatLabels()
{
    int total     = Database::instance().getTotalEvaluations();
    int pending   = Database::instance().getPendingEvaluations();
    int completed = Database::instance().getCompletedEvaluations();
    double avg    = Database::instance().getAverageNote();

    ui->lblTotalEval->setText(QString("Total: %1").arg(total));
    ui->lblPendingEval->setText(QString("En cours: %1").arg(pending));
    ui->lblCompletedEval->setText(QString("Terminées: %1").arg(completed));
    ui->lblAvgNote->setText(
        total > 0 ? QString("Moyenne: %1/10").arg(avg, 0, 'f', 1) : "Moyenne: —");
}

void EvaluationPage::setupCharts()
{
    createNoteDistributionChart();
    createTimelineChart();
}

void EvaluationPage::createNoteDistributionChart()
{
    QMap<int, int> distribution = Database::instance().getNoteDistribution();

    QBarSet *set = new QBarSet("Évaluations");
    set->setColor(QColor("#9f7aea"));

    for (int i = 1; i <= 10; ++i)
        *set << distribution.value(i, 0);

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des notes");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    for (int i = 1; i <= 10; ++i)
        categories << QString::number(i);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Note");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre");
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (!ui->chartWidgetDistribution->layout())
        ui->chartWidgetDistribution->setLayout(new QVBoxLayout());
    ui->chartWidgetDistribution->layout()->addWidget(chartView);
}

void EvaluationPage::createTimelineChart()
{
    QMap<QString, int> monthly = Database::instance().getEvaluationsPerMonth();

    QBarSet *set = new QBarSet("Évaluations");
    set->setColor(QColor("#667eea"));
    QStringList categories;

    for (auto it = monthly.begin(); it != monthly.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }

    QChart *chart = new QChart();
    chart->setTitle("Évaluations par mois");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    if (!categories.isEmpty()) {
        QBarSeries *series = new QBarSeries();
        series->append(set);
        chart->addSeries(series);

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setTitleText("Mois");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Nombre");
        axisY->setLabelFormat("%d");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    } else {
        chart->setTitle("Timeline (aucune donnée)");
    }

    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (!ui->chartWidgetTimeline->layout())
        ui->chartWidgetTimeline->setLayout(new QVBoxLayout());
    ui->chartWidgetTimeline->layout()->addWidget(chartView);
}

void EvaluationPage::onRefreshStats()
{
    QLayoutItem *item;
    if (ui->chartWidgetDistribution->layout()) {
        while ((item = ui->chartWidgetDistribution->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    if (ui->chartWidgetTimeline->layout()) {
        while ((item = ui->chartWidgetTimeline->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    setupCharts();
    updateStatLabels();
}

void EvaluationPage::onExportStats()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Exporter les statistiques",
        QDir::homePath() + "/eval_statistiques.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    int total     = Database::instance().getTotalEvaluations();
    int pending   = Database::instance().getPendingEvaluations();
    int completed = Database::instance().getCompletedEvaluations();
    double avg    = Database::instance().getAverageNote();
    QMap<int, int> dist = Database::instance().getNoteDistribution();

    QString html;
    html += "<html><body style='font-family:Arial;'>";
    html += "<h2 style='text-align:center;color:#9f7aea;'>Rapport Statistiques — Évaluations</h2>";
    html += "<p style='text-align:right;color:gray;'>Généré le : "
            + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";

    html += "<h3>Résumé</h3>";
    html += "<table border='1' cellpadding='6' cellspacing='0' style='border-collapse:collapse;width:50%;'>";
    html += "<tr><td><b>Total</b></td><td>" + QString::number(total) + "</td></tr>";
    html += "<tr style='background:#f5f0ff;'><td><b>En cours</b></td><td>" + QString::number(pending) + "</td></tr>";
    html += "<tr><td><b>Terminées</b></td><td>" + QString::number(completed) + "</td></tr>";
    html += "<tr style='background:#f5f0ff;'><td><b>Note moyenne</b></td><td>"
            + (total > 0 ? QString::number(avg, 'f', 1) + "/10" : "—") + "</td></tr>";
    html += "</table>";

    html += "<h3>Distribution des notes</h3>";
    html += "<table border='1' cellpadding='6' cellspacing='0' style='border-collapse:collapse;width:50%;'>";
    html += "<tr style='background:#9f7aea;color:white;'><th>Note</th><th>Nb</th></tr>";
    for (int i = 1; i <= 10; ++i) {
        QString bg = (i % 2 == 0) ? "#f5f0ff" : "#ffffff";
        html += QString("<tr style='background:%1;'><td>%2</td><td>%3</td></tr>")
                    .arg(bg).arg(i).arg(dist.value(i, 0));
    }
    html += "</table></body></html>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Export", "Rapport exporté :\n" + fileName);
}

// ═══════════════════════ REVIEWERS TAB ═══════════════════════

void EvaluationPage::setupReviewerTab()
{
    // Reviewer list (left panel)
    ui->listViewReviewers->setModel(reviewerModel);

    // Reviewer evaluations table (right panel)
    reviewerEvalModel->setHorizontalHeaderLabels({"ID", "Soumission", "Note", "Date", "Statut"});
    ui->tableViewReviewerEvals->setModel(reviewerEvalModel);
    ui->tableViewReviewerEvals->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewReviewerEvals->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadReviewers();
}

void EvaluationPage::loadReviewers()
{
    reviewerModel->clear();

    QVector<Reviewer> reviewers = Database::instance().getAllReviewers();

    for (const Reviewer& reviewer : reviewers) {
        QStandardItem* item = new QStandardItem(
            QString("%1  <%2>").arg(reviewer.getNom()).arg(reviewer.getEmail())
        );
        item->setData(reviewer.getId(),  Qt::UserRole);
        item->setData(reviewer.getNom(), Qt::UserRole + 1);
        item->setEditable(false);
        reviewerModel->appendRow(item);
    }
}

void EvaluationPage::loadReviewerEvaluations(int reviewerId)
{
    reviewerEvalModel->removeRows(0, reviewerEvalModel->rowCount());

    QVector<Evaluation> evaluations = Database::instance().getEvaluationsByReviewer(reviewerId);

    ui->lblReviewerStats->setText(QString("Total évaluations: %1").arg(evaluations.size()));

    for (const Evaluation& eval : evaluations) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(eval.getId()));
        row << new QStandardItem(QString::number(eval.getSubmissionId()));
        row << new QStandardItem(QString::number(eval.getNote()));
        row << new QStandardItem(eval.getDateEvaluation().toString("dd/MM/yyyy"));
        row << new QStandardItem(eval.getStatutString());
        reviewerEvalModel->appendRow(row);
    }

    ui->tableViewReviewerEvals->resizeColumnsToContents();
}

void EvaluationPage::onReviewerSelected(const QModelIndex& index)
{
    if (!index.isValid()) return;

    int reviewerId       = index.data(Qt::UserRole).toInt();
    QString reviewerName = index.data(Qt::UserRole + 1).toString();

    ui->lblSelectedReviewer->setText("Évaluations de: " + reviewerName);
    loadReviewerEvaluations(reviewerId);
}

void EvaluationPage::onAddReviewer()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouveau Relecteur");
    dialog.setMinimumWidth(350);

    QFormLayout* form = new QFormLayout(&dialog);
    form->setSpacing(12);
    form->setContentsMargins(16, 16, 16, 16);

    QLineEdit* nomEdit   = new QLineEdit(&dialog);
    QLineEdit* emailEdit = new QLineEdit(&dialog);
    nomEdit->setPlaceholderText("ex: Dr. Martin");
    emailEdit->setPlaceholderText("ex: martin@univ.dz");

    form->addRow("Nom :", nomEdit);
    form->addRow("Email :", emailEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    QString nom   = nomEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();

    if (nom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Champs requis", "Le nom et l'email sont obligatoires.");
        return;
    }

    // Check for duplicate name in already-loaded list
    for (int i = 0; i < reviewerModel->rowCount(); ++i) {
        if (reviewerModel->item(i)->data(Qt::UserRole + 1).toString() == nom) {
            QMessageBox::warning(this, "Doublon", "Un relecteur avec ce nom existe déjà.");
            return;
        }
    }

    Reviewer reviewer(0, nom, email);
    if (Database::instance().createReviewer(reviewer)) {
        loadReviewers();
        QMessageBox::information(this, "Succès", QString("Relecteur \"%1\" ajouté.").arg(nom));
    } else {
        QMessageBox::warning(this, "Erreur",
            "Impossible d'ajouter le relecteur.\n(L'email existe peut-être déjà en base.)");
    }
}

void EvaluationPage::onRefreshReviewers()
{
    loadReviewers();
    reviewerEvalModel->removeRows(0, reviewerEvalModel->rowCount());
    ui->lblSelectedReviewer->setText("Sélectionnez un relecteur");
    ui->lblReviewerStats->clear();
}
