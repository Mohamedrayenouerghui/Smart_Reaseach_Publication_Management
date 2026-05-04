#include "locationservice.h"

#include <QDesktopServices>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

LocationService::LocationService(QWidget *parent)
    : QWidget(parent), ui(nullptr)
{
    setWindowTitle("📍 Localisation - Google Maps");
    setMinimumSize(400, 200);
    resize(400, 200);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("<b style='font-size:16px;'>📍 Localisation de la conférence</b>", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *label = new QLabel("Lieu de la conférence :", this);
    m_lieuLineEdit = new QLineEdit(this);
    m_lieuLineEdit->setPlaceholderText("Ex: Tunis, Paris, New York...");
    m_lieuLineEdit->setMinimumHeight(36);

    auto *btnLayout = new QHBoxLayout();
    auto *openButton = new QPushButton("🗺 Ouvrir dans Google Maps", this);
    openButton->setMinimumHeight(38);
    openButton->setStyleSheet(
        "QPushButton { background-color: #2563EB; color: white; border: none; "
        "border-radius: 6px; font-size: 13px; font-weight: bold; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #1d4ed8; }");

    auto *closeButton = new QPushButton("Fermer", this);
    closeButton->setMinimumHeight(38);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #6B7280; color: white; border: none; "
        "border-radius: 6px; font-size: 13px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #4B5563; }");

    btnLayout->addWidget(openButton);
    btnLayout->addWidget(closeButton);

    root->addWidget(titleLabel);
    root->addWidget(label);
    root->addWidget(m_lieuLineEdit);
    root->addLayout(btnLayout);
    root->addStretch();

    connect(openButton, &QPushButton::clicked, this, [this]() {
        QString lieu = m_lieuLineEdit->text().trimmed();
        if (lieu.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez saisir un lieu");
            return;
        }
        QUrl url(QString("https://www.google.com/maps/search/?api=1&query=%1")
                     .arg(QString(QUrl::toPercentEncoding(lieu))));
        QDesktopServices::openUrl(url);
    });

    connect(closeButton, &QPushButton::clicked, this, &LocationService::close);
}

LocationService::~LocationService()
{
    ui = nullptr;
}

void LocationService::setLieu(const QString &lieu)
{
    if (m_lieuLineEdit) {
        m_lieuLineEdit->setText(lieu);
    }
}
