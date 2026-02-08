/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *mainLayout;
    QSpacerItem *topSpacer;
    QLabel *mainTitle;
    QSpacerItem *spacerBeforeCard;
    QHBoxLayout *centeringLayout;
    QSpacerItem *horizontalSpacerLeft;
    QFrame *loginCard;
    QVBoxLayout *cardLayout;
    QLabel *logoPlaceholder;
    QLabel *welcomeLabel;
    QSpacerItem *fieldSpacer1;
    QLabel *instructionLabel;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QHBoxLayout *optionsLayout;
    QCheckBox *rememberCheck;
    QSpacerItem *horizontalSpacer;
    QPushButton *forgotPasswordBtn;
    QPushButton *loginBtn;
    QPushButton *signupBtn;
    QLabel *orLabel;
    QPushButton *facialAuthBtn;
    QSpacerItem *horizontalSpacerRight;
    QSpacerItem *spacerAfterCard;
    QLabel *footerLabel;
    QSpacerItem *bottomSpacer;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(1200, 800);
        Login->setMinimumSize(QSize(1200, 800));
        Login->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #667eea, stop:1 #764ba2);\n"
"}"));
        mainLayout = new QVBoxLayout(Login);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        topSpacer = new QSpacerItem(20, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(topSpacer);

        mainTitle = new QLabel(Login);
        mainTitle->setObjectName("mainTitle");
        mainTitle->setStyleSheet(QString::fromUtf8("font-size: 36px;\n"
"font-weight: 700;\n"
"color: white;\n"
"background-color: transparent;\n"
"text-align: center;"));
        mainTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(mainTitle);

        spacerBeforeCard = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(spacerBeforeCard);

        centeringLayout = new QHBoxLayout();
        centeringLayout->setObjectName("centeringLayout");
        horizontalSpacerLeft = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        centeringLayout->addItem(horizontalSpacerLeft);

        loginCard = new QFrame(Login);
        loginCard->setObjectName("loginCard");
        loginCard->setMinimumSize(QSize(500, 600));
        loginCard->setMaximumSize(QSize(500, 600));
        loginCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: rgba(255, 255, 255, 0.95);\n"
"    border-radius: 25px;\n"
"    border: 1px solid rgba(255, 255, 255, 0.2);\n"
"}"));
        loginCard->setFrameShape(QFrame::Shape::StyledPanel);
        loginCard->setFrameShadow(QFrame::Shadow::Raised);
        cardLayout = new QVBoxLayout(loginCard);
        cardLayout->setSpacing(20);
        cardLayout->setObjectName("cardLayout");
        cardLayout->setContentsMargins(50, 40, 50, 40);
        logoPlaceholder = new QLabel(loginCard);
        logoPlaceholder->setObjectName("logoPlaceholder");
        logoPlaceholder->setMinimumSize(QSize(120, 120));
        logoPlaceholder->setMaximumSize(QSize(120, 120));
        logoPlaceholder->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #667eea, stop:1 #764ba2);\n"
"    border-radius: 60px;\n"
"    border: 4px solid white;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 18px;\n"
"}"));
        logoPlaceholder->setAlignment(Qt::AlignmentFlag::AlignCenter);

        cardLayout->addWidget(logoPlaceholder);

        welcomeLabel = new QLabel(loginCard);
        welcomeLabel->setObjectName("welcomeLabel");
        welcomeLabel->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"font-weight: 600;\n"
"color: #2d3748;\n"
"background-color: transparent;\n"
"text-align: center;"));
        welcomeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        cardLayout->addWidget(welcomeLabel);

        fieldSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        cardLayout->addItem(fieldSpacer1);

        instructionLabel = new QLabel(loginCard);
        instructionLabel->setObjectName("instructionLabel");
        instructionLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: 400;\n"
"color: #718096;\n"
"background-color: transparent;\n"
"text-align: center;\n"
"margin-bottom: 10px;"));
        instructionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        cardLayout->addWidget(instructionLabel);

        emailEdit = new QLineEdit(loginCard);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setMinimumSize(QSize(0, 50));
        emailEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #f7fafc;\n"
"    border: 2px solid #e2e8f0;\n"
"    border-radius: 12px;\n"
"    padding: 0px 15px;\n"
"    font-size: 16px;\n"
"    color: #2d3748;\n"
"}\n"
"QLineEdit:focus {\n"
"    border: 2px solid #667eea;\n"
"    background-color: white;\n"
"}"));

        cardLayout->addWidget(emailEdit);

        passwordEdit = new QLineEdit(loginCard);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setMinimumSize(QSize(0, 50));
        passwordEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #f7fafc;\n"
"    border: 2px solid #e2e8f0;\n"
"    border-radius: 12px;\n"
"    padding: 0px 15px;\n"
"    font-size: 16px;\n"
"    color: #2d3748;\n"
"}\n"
"QLineEdit:focus {\n"
"    border: 2px solid #667eea;\n"
"    background-color: white;\n"
"}"));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        cardLayout->addWidget(passwordEdit);

        optionsLayout = new QHBoxLayout();
        optionsLayout->setObjectName("optionsLayout");
        rememberCheck = new QCheckBox(loginCard);
        rememberCheck->setObjectName("rememberCheck");
        rememberCheck->setStyleSheet(QString::fromUtf8("QCheckBox {\n"
"    color: #4a5568;\n"
"    font-size: 14px;\n"
"    background-color: transparent;\n"
"}"));

        optionsLayout->addWidget(rememberCheck);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        optionsLayout->addItem(horizontalSpacer);

        forgotPasswordBtn = new QPushButton(loginCard);
        forgotPasswordBtn->setObjectName("forgotPasswordBtn");
        forgotPasswordBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #667eea;\n"
"    border: none;\n"
"    font-size: 14px;\n"
"    font-weight: 500;\n"
"    text-decoration: underline;\n"
"}\n"
"QPushButton:hover {\n"
"    color: #764ba2;\n"
"}"));

        optionsLayout->addWidget(forgotPasswordBtn);


        cardLayout->addLayout(optionsLayout);

        loginBtn = new QPushButton(loginCard);
        loginBtn->setObjectName("loginBtn");
        loginBtn->setMinimumSize(QSize(0, 55));
        loginBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 18px;\n"
"    font-weight: 600;\n"
"}\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #5a6fd8, stop:1 #6a4091);\n"
"}\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4a5cc7, stop:1 #5a3579);\n"
"}"));

        cardLayout->addWidget(loginBtn);

        signupBtn = new QPushButton(loginCard);
        signupBtn->setObjectName("signupBtn");
        signupBtn->setMinimumSize(QSize(0, 50));
        signupBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #667eea;\n"
"    border: 2px solid #667eea;\n"
"    border-radius: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: 600;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(102, 126, 234, 0.1);\n"
"    border: 2px solid #764ba2;\n"
"    color: #764ba2;\n"
"}"));

        cardLayout->addWidget(signupBtn);

        orLabel = new QLabel(loginCard);
        orLabel->setObjectName("orLabel");
        orLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: 500;\n"
"color: #a0aec0;\n"
"background-color: transparent;\n"
"text-align: center;\n"
"margin: 10px 0px;"));
        orLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        cardLayout->addWidget(orLabel);

        facialAuthBtn = new QPushButton(loginCard);
        facialAuthBtn->setObjectName("facialAuthBtn");
        facialAuthBtn->setMinimumSize(QSize(0, 55));
        facialAuthBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: white;\n"
"    color: #2d3748;\n"
"    border: 2px solid #e2e8f0;\n"
"    border-radius: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: 600;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #f7fafc;\n"
"    border: 2px solid #667eea;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #edf2f7;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/face"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        facialAuthBtn->setIcon(icon);

        cardLayout->addWidget(facialAuthBtn);


        centeringLayout->addWidget(loginCard);

        horizontalSpacerRight = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        centeringLayout->addItem(horizontalSpacerRight);


        mainLayout->addLayout(centeringLayout);

        spacerAfterCard = new QSpacerItem(20, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(spacerAfterCard);

        footerLabel = new QLabel(Login);
        footerLabel->setObjectName("footerLabel");
        footerLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"font-weight: 400;\n"
"color: rgba(255, 255, 255, 0.8);\n"
"background-color: transparent;\n"
"text-align: center;\n"
"padding: 20px 0px;"));
        footerLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(footerLabel);

        bottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login - Syst\303\250me de Gestion des Publications de Recherche", nullptr));
        mainTitle->setText(QCoreApplication::translate("Login", "Syst\303\250me de Gestion des Publications", nullptr));
        logoPlaceholder->setText(QCoreApplication::translate("Login", "LOGO", nullptr));
        welcomeLabel->setText(QCoreApplication::translate("Login", "Bienvenue", nullptr));
        instructionLabel->setText(QCoreApplication::translate("Login", "Connectez-vous pour acc\303\251der au syst\303\250me", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("Login", "Adresse email", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("Login", "Mot de passe", nullptr));
        rememberCheck->setText(QCoreApplication::translate("Login", "Se souvenir de moi", nullptr));
        forgotPasswordBtn->setText(QCoreApplication::translate("Login", "Mot de passe oubli\303\251 ?", nullptr));
        loginBtn->setText(QCoreApplication::translate("Login", "Se connecter", nullptr));
        signupBtn->setText(QCoreApplication::translate("Login", "Cr\303\251er un nouveau compte", nullptr));
        orLabel->setText(QCoreApplication::translate("Login", "\342\224\200 OU \342\224\200", nullptr));
        facialAuthBtn->setText(QCoreApplication::translate("Login", "Authentification Faciale", nullptr));
        footerLabel->setText(QCoreApplication::translate("Login", "\302\251 2024 Syst\303\250me de Gestion des Publications de Recherche - RaMeGaNe", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
