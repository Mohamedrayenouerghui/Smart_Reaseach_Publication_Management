#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

#include <opencv2/opencv.hpp>

// ─────────────────────────────────────────────────────────────────────────────
//  FaceCaptureDialog
//
//  Opens the webcam, shows a live preview with a green face-detection rectangle,
//  and captures CAPTURE_COUNT face samples for the given user e-mail.
//
//  Samples are saved under:
//      <app_dir>/faces/<sanitized_email>/face_0.jpg … face_9.jpg
//
//  Usage:
//      FaceCaptureDialog dlg(email, this);
//      if (dlg.exec() == QDialog::Accepted)
//          ui->faceStatusLabel->setText("✅ Visage enregistré !");
// ─────────────────────────────────────────────────────────────────────────────
class FaceCaptureDialog : public QDialog
{
    Q_OBJECT

public:
    static constexpr int CAPTURE_COUNT = 10;   // samples to collect
    static constexpr int PREVIEW_FPS   = 20;   // ms between frames

    explicit FaceCaptureDialog(const QString& userEmail, QWidget* parent = nullptr);
    ~FaceCaptureDialog() override;

    // Returns the directory where face samples were saved (empty on failure).
    QString savedDirectory() const { return m_savedDir; }

private slots:
    void onFrameTimer();
    void onCaptureClicked();

private:
    void setupUi();
    void releaseCamera();
    static QString sanitizeEmail(const QString& email);
    static QString facesRootDir();

    // ── widgets ──────────────────────────────────────────────────────────────
    QLabel*       m_preview      = nullptr;
    QLabel*       m_statusLabel  = nullptr;
    QPushButton*  m_captureBtn   = nullptr;
    QPushButton*  m_cancelBtn    = nullptr;
    QProgressBar* m_progress     = nullptr;

    // ── OpenCV ───────────────────────────────────────────────────────────────
    cv::VideoCapture        m_cap;
    cv::CascadeClassifier   m_faceDetector;
    QTimer*                 m_timer    = nullptr;

    // ── state ────────────────────────────────────────────────────────────────
    QString  m_userEmail;
    QString  m_savedDir;
    int      m_capturedCount = 0;
    cv::Mat  m_lastFaceROI;          // most-recently detected face region
};
