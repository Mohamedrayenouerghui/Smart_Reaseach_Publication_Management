#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

#include <opencv2/opencv.hpp>

// ─────────────────────────────────────────────────────────────────────────────
//  FaceAuthDialog
//
//  Opens the webcam, detects a face in real-time, then compares it against
//  every user's stored samples.  If the best match score exceeds THRESHOLD
//  the dialog auto-closes with Accepted and matchedEmail() is filled.
//
//  Recognition algorithm:
//    1. Detect face with Haar cascade.
//    2. Resize to 128×128 grayscale.
//    3. For every stored sample: compute normalised histogram (256 bins).
//    4. Compare with cv::HISTCMP_CORREL  (range 0..1, higher = more similar).
//    5. Best average score per user → candidate.
//    6. Accept if candidate score >= THRESHOLD.
//
//  Usage:
//      FaceAuthDialog dlg(this);
//      if (dlg.exec() == QDialog::Accepted) {
//          QString email = dlg.matchedEmail();
//          // load user from DB by email and open main window
//      }
// ─────────────────────────────────────────────────────────────────────────────
class FaceAuthDialog : public QDialog
{
    Q_OBJECT

public:
    // How similar the face must be (0.0 – 1.0).  Raise to reduce false positives.
    static constexpr double THRESHOLD   = 0.62;
    static constexpr int    PREVIEW_FPS = 20;

    explicit FaceAuthDialog(QWidget* parent = nullptr);
    ~FaceAuthDialog() override;

    QString matchedEmail() const { return m_matchedEmail; }

private slots:
    void onFrameTimer();

private:
    void setupUi();
    void releaseCamera();

    // Returns the e-mail of the best-matching stored user, or "" if below threshold.
    QString matchFace(const cv::Mat& faceGray);

    // Compute a normalised grayscale histogram for a 128×128 face image.
    static cv::Mat computeHistogram(const cv::Mat& faceGray);

    static QString facesRootDir();
    static QString emailFromDirName(const QString& dirName);

    // ── widgets ──────────────────────────────────────────────────────────────
    QLabel*      m_preview     = nullptr;
    QLabel*      m_statusLabel = nullptr;
    QPushButton* m_cancelBtn   = nullptr;

    // ── OpenCV ───────────────────────────────────────────────────────────────
    cv::VideoCapture      m_cap;
    cv::CascadeClassifier m_faceDetector;
    QTimer*               m_timer = nullptr;

    // ── state ────────────────────────────────────────────────────────────────
    QString m_matchedEmail;
    int     m_consecutiveMatches = 0;  // require N consecutive hits to avoid flicker
    static constexpr int REQUIRED_CONSECUTIVE = 3;
};
