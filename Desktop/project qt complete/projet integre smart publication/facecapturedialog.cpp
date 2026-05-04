#include "FaceCaptureDialog.h"

#include <QDir>
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
QString FaceCaptureDialog::sanitizeEmail(const QString& email)
{
    // Replace characters that are invalid in directory names
    QString s = email.toLower();
    s.replace('@', "_at_");        // ← FIXED: double quotes = QString literal
    s.replace('.', '_');
    s.replace('/', '_');
    s.replace('\\', '_');
    return s;
}

QString FaceCaptureDialog::facesRootDir()
{
    return QCoreApplication::applicationDirPath() + "/faces";
}

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────
FaceCaptureDialog::FaceCaptureDialog(const QString& userEmail, QWidget* parent)
    : QDialog(parent), m_userEmail(userEmail)
{
    setWindowTitle("Enregistrement du visage");
    setFixedSize(520, 460);
    setModal(true);

    // ── Load Haar cascade ────────────────────────────────────────────────────
    // The XML ships with every OpenCV build inside etc/haarcascades/
    QString cascadePath =
        "C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/etc/haarcascades/"
        "haarcascade_frontalface_default.xml";

    if (!m_faceDetector.load(cascadePath.toStdString())) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de charger le détecteur de visages.\n"
                              "Vérifiez le chemin du fichier haarcascade_frontalface_default.xml");
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }

    // ── Open webcam (index 0) ─────────────────────────────────────────────
    m_cap.open(0);
    if (!m_cap.isOpened()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'ouvrir la webcam.\n"
                              "Vérifiez qu'aucune autre application ne l'utilise.");
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH,  640);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    setupUi();

    // ── Start frame timer ─────────────────────────────────────────────────
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &FaceCaptureDialog::onFrameTimer);
    m_timer->start(PREVIEW_FPS);
}

FaceCaptureDialog::~FaceCaptureDialog()
{
    releaseCamera();
}

void FaceCaptureDialog::releaseCamera()
{
    if (m_timer) m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
}

// ─────────────────────────────────────────────────────────────────────────────
//  UI setup
// ─────────────────────────────────────────────────────────────────────────────
void FaceCaptureDialog::setupUi()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(8);

    // Title
    auto* title = new QLabel("📷  Positionnez votre visage dans le cadre, puis cliquez Capturer.");
    title->setWordWrap(true);
    title->setStyleSheet("font-size:13px; color:#334155;");
    root->addWidget(title);

    // Preview label (shows webcam)
    m_preview = new QLabel;
    m_preview->setFixedSize(480, 320);
    m_preview->setAlignment(Qt::AlignCenter);
    m_preview->setStyleSheet("background:#000; border-radius:8px;");
    root->addWidget(m_preview, 0, Qt::AlignHCenter);

    // Progress bar
    m_progress = new QProgressBar;
    m_progress->setRange(0, CAPTURE_COUNT);
    m_progress->setValue(0);
    m_progress->setFormat("Captures : %v / %m");
    m_progress->setFixedHeight(22);
    m_progress->setStyleSheet(
        "QProgressBar { border:1.5px solid #C7D2FE; border-radius:6px; "
        "  background:#F0F4FF; text-align:center; font-size:12px; color:#3730A3; }"
        "QProgressBar::chunk { background:#667eea; border-radius:5px; }");
    root->addWidget(m_progress);

    // Status label
    m_statusLabel = new QLabel("En attente — aucun visage détecté");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size:12px; color:#64748B;");
    root->addWidget(m_statusLabel);

    // Buttons row
    auto* btnRow = new QHBoxLayout;
    btnRow->setSpacing(12);

    m_cancelBtn = new QPushButton("Annuler");
    m_cancelBtn->setFixedHeight(36);
    m_cancelBtn->setStyleSheet(
        "QPushButton { background:transparent; color:#64748B; "
        "  border:2px solid #E2E8F0; border-radius:8px; font-weight:600; padding:0 20px; }"
        "QPushButton:hover { background:#F8FAFC; }");
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    m_captureBtn = new QPushButton("📸  Capturer");
    m_captureBtn->setFixedHeight(36);
    m_captureBtn->setEnabled(false);   // enabled only when a face is in view
    m_captureBtn->setStyleSheet(
        "QPushButton { background:#0EA5E9; color:white; border:none; "
        "  border-radius:8px; font-weight:600; font-size:13px; padding:0 20px; }"
        "QPushButton:hover { background:#0284C7; }"
        "QPushButton:disabled { background:#BAE6FD; color:#7DD3FC; }");
    connect(m_captureBtn, &QPushButton::clicked, this, &FaceCaptureDialog::onCaptureClicked);

    btnRow->addStretch();
    btnRow->addWidget(m_cancelBtn);
    btnRow->addWidget(m_captureBtn);
    root->addLayout(btnRow);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Frame update (called by timer)
// ─────────────────────────────────────────────────────────────────────────────
void FaceCaptureDialog::onFrameTimer()
{
    cv::Mat frame;
    if (!m_cap.read(frame) || frame.empty()) return;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    m_faceDetector.detectMultiScale(
        gray, faces,
        1.1, 5, 0,
        cv::Size(80, 80));

    if (!faces.empty()) {
        // Use the largest detected face
        auto largest = *std::max_element(faces.begin(), faces.end(),
                                         [](const cv::Rect& a, const cv::Rect& b){ return a.area() < b.area(); });

        cv::rectangle(frame, largest, cv::Scalar(0, 200, 100), 2);
        m_lastFaceROI = gray(largest).clone();
        m_captureBtn->setEnabled(true);
        m_statusLabel->setText("✅ Visage détecté — cliquez Capturer");
        m_statusLabel->setStyleSheet("font-size:12px; color:#059669; font-weight:600;");
    } else {
        m_lastFaceROI = cv::Mat();
        m_captureBtn->setEnabled(false);
        m_statusLabel->setText("⏳ En attente — aucun visage détecté");
        m_statusLabel->setStyleSheet("font-size:12px; color:#64748B;");
    }

    // Display frame
    QImage img = matToQImage(frame);
    m_preview->setPixmap(QPixmap::fromImage(img).scaled(
        m_preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Capture button clicked
// ─────────────────────────────────────────────────────────────────────────────
void FaceCaptureDialog::onCaptureClicked()
{
    if (m_lastFaceROI.empty()) return;

    // Prepare output directory  faces/<sanitized_email>/
    QString dir = facesRootDir() + "/" + sanitizeEmail(m_userEmail);
    QDir().mkpath(dir);

    // Resize to standard 128×128 before saving
    cv::Mat resized;
    cv::resize(m_lastFaceROI, resized, cv::Size(128, 128));

    QString filePath = dir + "/face_" + QString::number(m_capturedCount) + ".jpg";
    cv::imwrite(filePath.toStdString(), resized);

    m_capturedCount++;
    m_progress->setValue(m_capturedCount);
    m_statusLabel->setText(
        QString("📸 %1/%2 captures effectuées")
            .arg(m_capturedCount).arg(CAPTURE_COUNT));
    m_statusLabel->setStyleSheet("font-size:12px; color:#0EA5E9; font-weight:600;");

    if (m_capturedCount >= CAPTURE_COUNT) {
        m_savedDir = dir;
        releaseCamera();
        m_statusLabel->setText("✅ Enregistrement terminé !");
        m_statusLabel->setStyleSheet("font-size:13px; color:#10B981; font-weight:700;");
        m_captureBtn->setEnabled(false);
        QTimer::singleShot(800, this, &QDialog::accept);
    }
}
