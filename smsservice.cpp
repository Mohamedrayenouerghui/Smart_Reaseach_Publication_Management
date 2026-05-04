#include "smsservice.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTextEdit>
#include <QUrl>
#include <QUrlQuery>
#include <QVBoxLayout>

SMSService::SMSService(QWidget *parent)
    : QWidget(parent), ui(nullptr)
{
    setWindowTitle("Envoyer WhatsApp");
    setMinimumSize(420, 320);
    resize(420, 320);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(20, 20, 20, 20);

    auto *titleLabel = new QLabel("<b style='font-size:16px;'>📩 Envoyer un message WhatsApp</b>", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *phoneLabel = new QLabel("Numéro de téléphone :", this);
    auto *phoneNumberLineEdit = new QLineEdit(this);
    phoneNumberLineEdit->setObjectName("phoneNumberLineEdit");
    phoneNumberLineEdit->setPlaceholderText("+216XXXXXXXX");
    phoneNumberLineEdit->setMinimumHeight(36);

    QRegularExpression phoneRegex("^[+]?[0-9]{8,15}$");
    phoneNumberLineEdit->setValidator(new QRegularExpressionValidator(phoneRegex, phoneNumberLineEdit));

    auto *msgLabel = new QLabel("Message :", this);
    auto *messageTextEdit = new QTextEdit(this);
    messageTextEdit->setObjectName("messageTextEdit");
    messageTextEdit->setPlaceholderText("Entrez votre message ici...");
    messageTextEdit->setMinimumHeight(100);

    auto *btnLayout = new QHBoxLayout();
    auto *sendSMSButton = new QPushButton("📤 Envoyer WhatsApp", this);
    sendSMSButton->setObjectName("sendSMSButton");
    sendSMSButton->setMinimumHeight(38);
    sendSMSButton->setStyleSheet(
        "QPushButton { background-color: #25D366; color: white; border: none; "
        "border-radius: 6px; font-size: 13px; font-weight: bold; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #1ebe5d; }");

    auto *clearButton = new QPushButton("🗑 Effacer", this);
    clearButton->setObjectName("clearButton");
    clearButton->setMinimumHeight(38);
    clearButton->setStyleSheet(
        "QPushButton { background-color: #6B7280; color: white; border: none; "
        "border-radius: 6px; font-size: 13px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #4B5563; }");

    btnLayout->addWidget(sendSMSButton);
    btnLayout->addWidget(clearButton);

    root->addWidget(titleLabel);
    root->addWidget(phoneLabel);
    root->addWidget(phoneNumberLineEdit);
    root->addWidget(msgLabel);
    root->addWidget(messageTextEdit);
    root->addLayout(btnLayout);

    connect(sendSMSButton, &QPushButton::clicked, this, [this, phoneNumberLineEdit, messageTextEdit]() {
        QString phoneNumber = phoneNumberLineEdit->text().trimmed();
        QString message = messageTextEdit->toPlainText().trimmed();

        if (phoneNumber.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de téléphone");
            return;
        }

        if (message.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un message");
            return;
        }

        if (!phoneNumber.startsWith("+")) {
            phoneNumber = "+216" + phoneNumber;
        }

        QString waNumber = phoneNumber;
        waNumber.remove('+');

        QUrlQuery query;
        query.addQueryItem("text", message);

        QUrl url(QString("https://wa.me/%1").arg(waNumber));
        url.setQuery(query);

        if (!QDesktopServices::openUrl(url)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir WhatsApp.");
            return;
        }

        QMessageBox::information(this, "WhatsApp",
            "WhatsApp a été ouvert dans le navigateur.\nCliquez sur 'Envoyer' dans WhatsApp pour finaliser.");
    });

    connect(clearButton, &QPushButton::clicked, this, [phoneNumberLineEdit, messageTextEdit]() {
        phoneNumberLineEdit->clear();
        messageTextEdit->clear();
        phoneNumberLineEdit->setFocus();
    });
}

SMSService::~SMSService()
{
    ui = nullptr;
}

void SMSService::setupConnections()
{
}

void SMSService::onSendSMSClicked()
{
}

void SMSService::onBackClicked()
{
    close();
}

void SMSService::onClearClicked()
{
}
