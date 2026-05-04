#include "FaceAuthDialog.h"

#include <QDir>
#include <QFileInfoList>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────────────
static QImage matToQImage(const cv::Mat& mat)
{
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows,
                  static_cast<int>(rgb.step),
                  QImage::Format_RGB888).copy();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Static helpers
// ─────────────────────────────────────────────────────────────────────────────
QString FaceAuthDialog::facesRootDir()
{
    return QCoreApplication::applicationDirPath() + "/faces";
}

// Reverses the sanitization done in FaceCaptureDialog: _at_ → @, _ → .
QString FaceAuthDialog::emailFromDirName(const QString& dirName)
{
    QString s = dirName;
    s.replace("_at_", "@");
    // Only replace underscores that were originally dots (heuristic: after @)
    int atPos = s.indexOf('@');
    if (atPos != -1) {
        QString domain = s.mid(atPos + 1);
        domain.replace('_', '.');
        s = s.left(atPos + 1) + domain;
    }
    return s;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────
FaceAuthDialog::FaceAuthDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Authentification Faciale");
    setFixedSize(520, 430);
    setModal(true);

    // ── Haar cascade ─────────────────────────────────────────────────────────
    QString cascadePath =
        "C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/etc/haarcascades/"
        "haarcascade_frontalface_default.xml";

    if (!m_faceDetector.load(cascadePath.toStdString())) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de charger le détecteur de visages.");
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }

    // ── Webcam ───────────────────────────────────────────────────────────────
    m_cap.open(0);
    if (!m_cap.isOpened()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'ouvrir la webcam.");
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH,  640);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    setupUi();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &FaceAuthDialog::onFrameTimer);
    m_timer->start(PREVIEW_FPS);
}

FaceAuthDialog::~FaceAuthDialog()
{
    releaseCamera();
}

void FaceAuthDialog::releaseCamera()
{
    if (m_timer) m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
}

// ─────────────────────────────────────────────────────────────────────────────
//  UI
// ─────────────────────────────────────────────────────────────────────────────
void FaceAuthDialog::setupUi()
{
    setStyleSheet("QDialog { background:#FFFFFF; }");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(8);

    auto* title = new QLabel("🔐  Authentification par reconnaissance faciale");
    title->setWordWrap(true);
    title->setStyleSheet("font-size:14px; font-weight:600; color:#1E293B;");
    root->addWidget(title);

    m_preview = new QLabel;
    m_preview->setFixedSize(480, 320);
    m_preview->setAlignment(Qt::AlignCenter);
    m_preview->setStyleSheet("background:#000; border-radius:8px;");
    root->addWidget(m_preview, 0, Qt::AlignHCenter);

    m_statusLabel = new QLabel("⏳ Positionnez votre visage face à la caméra…");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("font-size:13px; color:#64748B;");
    root->addWidget(m_statusLabel);

    auto* btnRow = new QHBoxLayout;
    m_cancelBtn = new QPushButton("Annuler");
    m_cancelBtn->setFixedHeight(36);
    m_cancelBtn->setStyleSheet(
        "QPushButton { background:transparent; color:#64748B; "
        "  border:2px solid #E2E8F0; border-radius:8px; font-weight:600; padding:0 24px; }"
        "QPushButton:hover { background:#F8FAFC; }");
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnRow->addStretch();
    btnRow->addWidget(m_cancelBtn);
    root->addLayout(btnRow);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Frame update
// ─────────────────────────────────────────────────────────────────────────────
void FaceAuthDialog::onFrameTimer()
{
    cv::Mat frame;
    if (!m_cap.read(frame) || frame.empty()) return;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    m_faceDetector.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(80, 80));

    if (faces.empty()) {
        m_consecutiveMatches = 0;
        m_statusLabel->setText("⏳ Positionnez votre visage face à la caméra…");
        m_statusLabel->setStyleSheet("font-size:13px; color:#64748B;");
        QImage img = matToQImage(frame);
        m_preview->setPixmap(QPixmap::fromImage(img).scaled(
            m_preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        return;
    }

    // Largest face
    auto largest = *std::max_element(faces.begin(), faces.end(),
                                     [](const cv::Rect& a, const cv::Rect& b){ return a.area() < b.area(); });

    // Draw detection rectangle
    cv::rectangle(frame, largest, cv::Scalar(0, 180, 255), 2);

    // Crop, resize, match
    cv::Mat faceROI = gray(largest).clone();
    cv::resize(faceROI, faceROI, cv::Size(128, 128));

    QString candidate = matchFace(faceROI);

    if (!candidate.isEmpty()) {
        m_consecutiveMatches++;
        m_statusLabel->setText(
            QString("✅ Correspondance trouvée : %1  (%2/%3)")
                .arg(candidate)
                .arg(m_consecutiveMatches)
                .arg(REQUIRED_CONSECUTIVE));
        m_statusLabel->setStyleSheet("font-size:13px; color:#10B981; font-weight:700;");

        if (m_consecutiveMatches >= REQUIRED_CONSECUTIVE) {
            m_matchedEmail = candidate;
            releaseCamera();
            QTimer::singleShot(300, this, &QDialog::accept);
        }
    } else {
        m_consecutiveMatches = 0;
        m_statusLabel->setText("❌ Visage non reconnu — réessayez");
        m_statusLabel->setStyleSheet("font-size:13px; color:#EF4444;");
    }

    QImage img = matToQImage(frame);
    m_preview->setPixmap(QPixmap::fromImage(img).scaled(
        m_preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Face matching (histogram correlation)
// ─────────────────────────────────────────────────────────────────────────────
cv::Mat FaceAuthDialog::computeHistogram(const cv::Mat& faceGray)
{
    cv::Mat hist;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = range;
    cv::calcHist(&faceGray, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange);
    cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
    return hist;
}

QString FaceAuthDialog::matchFace(const cv::Mat& faceGray)
{
    QDir root(facesRootDir());
    if (!root.exists()) return {};

    cv::Mat queryHist = computeHistogram(faceGray);

    QString bestEmail;
    double  bestScore = -1.0;

    // Iterate over every user sub-directory
    QFileInfoList userDirs = root.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& dirInfo : userDirs) {
        QDir userDir(dirInfo.absoluteFilePath());
        QFileInfoList samples = userDir.entryInfoList({"face_*.jpg"}, QDir::Files);
        if (samples.isEmpty()) continue;

        double totalScore = 0.0;
        int    validSamples = 0;

        for (const QFileInfo& fi : samples) {
            cv::Mat stored = cv::imread(fi.absoluteFilePath().toStdString(),
                                        cv::IMREAD_GRAYSCALE);
            if (stored.empty()) continue;
            cv::resize(stored, stored, cv::Size(128, 128));

            cv::Mat storedHist = computeHistogram(stored);
            double score = cv::compareHist(queryHist, storedHist, cv::HISTCMP_CORREL);
            totalScore += score;
            validSamples++;
        }

        if (validSamples == 0) continue;

        double avgScore = totalScore / validSamples;
        qDebug() << "[FaceAuth] user=" << dirInfo.fileName()
                 << "avgScore=" << avgScore;

        if (avgScore > bestScore) {
            bestScore = avgScore;
            bestEmail = emailFromDirName(dirInfo.fileName());
        }
    }

    return (bestScore >= THRESHOLD) ? bestEmail : QString();
}
