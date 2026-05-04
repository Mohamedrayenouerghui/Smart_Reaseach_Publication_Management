#include "decisiondialog.h"
#include "arduinomanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

DecisionDialog::DecisionDialog(int idSoumission, const QString &articleTitle, 
                               ArduinoManager *arduino, QWidget *parent)
    : QDialog(parent)
    , soumissionId(idSoumission)
    , articleTitle(articleTitle)
    , decision("")
    , decisionMade(false)
    , arduinoManager(arduino)
    , animationStep(0)
{
    setupUI();
    connectSignals();
    sendWaitingMessageToArduino();
    
    // Démarrer l'animation d'attente
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &DecisionDialog::updateWaitingAnimation);
    animationTimer->start(500);
    
    qDebug() << "Dialog créé pour soumission" << idSoumission;
}

DecisionDialog::~DecisionDialog()
{
    if (animationTimer) {
        animationTimer->stop();
    }
}

void DecisionDialog::setupUI()
{
    setWindowTitle("Décision Arduino");
    setModal(true);
    setMinimumSize(500, 350);
    
    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Titre
    titleLabel = new QLabel("🤖 Décision via Arduino");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #8B5CF6; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Informations sur l'article
    QLabel *articleLabel = new QLabel(QString("📄 Article: %1").arg(articleTitle));
    articleLabel->setWordWrap(true);
    articleLabel->setStyleSheet(
        "font-size: 14px; "
        "color: #1E293B; "
        "background-color: #F8FAFC; "
        "padding: 15px; "
        "border-radius: 8px; "
        "border: 1px solid #E2E8F0;"
    );
    mainLayout->addWidget(articleLabel);
    
    // ID de la soumission
    QLabel *idLabel = new QLabel(QString("🔢 Soumission #%1").arg(soumissionId));
    idLabel->setStyleSheet(
        "font-size: 13px; "
        "color: #64748B; "
        "padding: 5px;"
    );
    mainLayout->addWidget(idLabel);
    
    // Séparateur
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #E2E8F0;");
    mainLayout->addWidget(line);
    
    // Label d'attente avec animation
    waitingLabel = new QLabel("⏳ En attente de décision Arduino");
    QFont waitingFont;
    waitingFont.setPointSize(14);
    waitingFont.setBold(true);
    waitingLabel->setFont(waitingFont);
    waitingLabel->setAlignment(Qt::AlignCenter);
    waitingLabel->setStyleSheet(
        "color: #F59E0B; "
        "background-color: #FEF3C7; "
        "padding: 20px; "
        "border-radius: 8px; "
        "border: 2px solid #F59E0B;"
    );
    mainLayout->addWidget(waitingLabel);
    
    // Instructions
    QLabel *instructionLabel = new QLabel(
        "Utilisez le bouton physique Arduino:\n"
        "• 1 clic = ACCEPTER\n"
        "• 2 clics = REJETER"
    );
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet(
        "font-size: 12px; "
        "color: #64748B; "
        "padding: 10px; "
        "background-color: #F8FAFC; "
        "border-radius: 6px;"
    );
    mainLayout->addWidget(instructionLabel);
    
    // Statut Arduino
    statusLabel = new QLabel("Arduino: Connecté");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "font-size: 11px; "
        "color: #10B981; "
        "padding: 5px;"
    );
    mainLayout->addWidget(statusLabel);
    
    // Label de debug pour voir les événements
    QLabel *debugLabel = new QLabel("🔍 Debug: En attente d'événements...");
    debugLabel->setObjectName("debugLabel");
    debugLabel->setAlignment(Qt::AlignCenter);
    debugLabel->setWordWrap(true);
    debugLabel->setStyleSheet(
        "font-size: 10px; "
        "color: #64748B; "
        "background-color: #F1F5F9; "
        "padding: 8px; "
        "border-radius: 4px; "
        "font-family: 'Courier New', monospace;"
    );
    mainLayout->addWidget(debugLabel);
    
    mainLayout->addStretch();
    
    // Bouton Annuler
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    cancelBtn = new QPushButton("Annuler");
    cancelBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #64748B; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 10px 24px; "
        "    font-size: 13px; "
        "    font-weight: 600; "
        "    min-width: 100px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #475569; "
        "}"
    );
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    setStyleSheet("QDialog { background-color: white; }");
}

void DecisionDialog::connectSignals()
{
    if (arduinoManager) {
        qDebug() << "🔗 Connexion des signaux ArduinoManager...";
        
        connect(arduinoManager, &ArduinoManager::soumissionAcceptee,
                this, &DecisionDialog::onDecisionAcceptee);
        connect(arduinoManager, &ArduinoManager::soumissionRejetee,
                this, &DecisionDialog::onDecisionRejetee);
        connect(arduinoManager, &ArduinoManager::statusChanged,
                this, &DecisionDialog::onArduinoStatusChanged);
        
        qDebug() << "✅ Signaux connectés avec succès";
        qDebug() << "📊 État Arduino: Connecté =" << arduinoManager->isArduinoConnected();
        qDebug() << "📊 Port Arduino:" << arduinoManager->getArduinoPort();
        
        // Mettre à jour le label de debug
        QLabel *debugLabel = findChild<QLabel*>("debugLabel");
        if (debugLabel) {
            QString debugText = QString("🔍 Debug: Signaux connectés\n"
                                       "Arduino: %1\n"
                                       "Port: %2\n"
                                       "Soumission ID: %3")
                .arg(arduinoManager->isArduinoConnected() ? "✅ Connecté" : "❌ Déconnecté")
                .arg(arduinoManager->getArduinoPort())
                .arg(soumissionId);
            debugLabel->setText(debugText);
        }
    } else {
        qDebug() << "❌ ERREUR CRITIQUE: ArduinoManager est NULL!";
        
        QLabel *debugLabel = findChild<QLabel*>("debugLabel");
        if (debugLabel) {
            debugLabel->setText("❌ Debug: ERREUR - ArduinoManager NULL!");
            debugLabel->setStyleSheet(
                "font-size: 10px; "
                "color: #EF4444; "
                "background-color: #FEE2E2; "
                "padding: 8px; "
                "border-radius: 4px; "
                "font-family: 'Courier New', monospace;"
            );
        }
    }
}

void DecisionDialog::sendWaitingMessageToArduino()
{
    qDebug() << "📤 sendWaitingMessageToArduino appelé";
    
    if (!arduinoManager) {
        qDebug() << "❌ ArduinoManager est NULL!";
        return;
    }
    
    if (!arduinoManager->isArduinoConnected()) {
        qDebug() << "❌ Arduino n'est pas connecté!";
        return;
    }
    
    // Définir cette soumission comme la soumission en cours
    arduinoManager->setCurrentSoumission(soumissionId);
    qDebug() << "✅ Soumission en cours définie:" << soumissionId;
    
    // Envoyer un message au LCD pour indiquer qu'on attend
    arduinoManager->sendStatusToArduino("En attente...");
    qDebug() << "✅ Message 'En attente...' envoyé au LCD";
    
    qDebug() << "⏳ DecisionDialog: En attente de décision pour soumission" << soumissionId;
}

void DecisionDialog::onDecisionAcceptee(int id)
{
    qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
    qDebug() << "║ DECISIONDIALOG::onDecisionAcceptee                            ║";
    qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
    qDebug() << "📡 Signal ACCEPTÉE reçu pour ID" << id;
    qDebug() << "📊 soumissionId actuel =" << soumissionId;
    qDebug() << "🔍 Comparaison: id ==" << id << ", soumissionId ==" << soumissionId;
    
    if (id == soumissionId) {
        qDebug() << "✅ IDs correspondent! Mise à jour de l'interface...";
        decision = "Accepté";
        decisionMade = true;
        
        if (animationTimer) {
            animationTimer->stop();
            qDebug() << "⏸️  Animation arrêtée";
        }
        
        waitingLabel->setText("✅ Article ACCEPTÉ");
        waitingLabel->setStyleSheet(
            "color: #10B981; "
            "background-color: #D1FAE5; "
            "padding: 20px; "
            "border-radius: 8px; "
            "border: 2px solid #10B981; "
            "font-size: 14px; "
            "font-weight: bold;"
        );
        
        qDebug() << "🎉 Interface mise à jour, fermeture dans 2 secondes...";
        QTimer::singleShot(2000, this, &QDialog::accept);
    } else {
        qDebug() << "⚠️  IDs ne correspondent pas, signal ignoré";
    }
    qDebug() << "═══════════════════════════════════════════════════════════════";
}

void DecisionDialog::onDecisionRejetee(int id)
{
    qDebug() << "╔═══════════════════════════════════════════════════════════════╗";
    qDebug() << "║ DECISIONDIALOG::onDecisionRejetee                             ║";
    qDebug() << "╚═══════════════════════════════════════════════════════════════╝";
    qDebug() << "📡 Signal REJETÉE reçu pour ID" << id;
    qDebug() << "📊 soumissionId actuel =" << soumissionId;
    qDebug() << "🔍 Comparaison: id ==" << id << ", soumissionId ==" << soumissionId;
    
    if (id == soumissionId) {
        qDebug() << "✅ IDs correspondent! Mise à jour de l'interface...";
        decision = "Rejeté";
        decisionMade = true;
        
        if (animationTimer) {
            animationTimer->stop();
            qDebug() << "⏸️  Animation arrêtée";
        }
        
        waitingLabel->setText("❌ Article REJETÉ");
        waitingLabel->setStyleSheet(
            "color: #EF4444; "
            "background-color: #FEE2E2; "
            "padding: 20px; "
            "border-radius: 8px; "
            "border: 2px solid #EF4444; "
            "font-size: 14px; "
            "font-weight: bold;"
        );
        
        qDebug() << "🎉 Interface mise à jour, fermeture dans 2 secondes...";
        QTimer::singleShot(2000, this, &QDialog::accept);
    } else {
        qDebug() << "⚠️  IDs ne correspondent pas, signal ignoré";
    }
    qDebug() << "═══════════════════════════════════════════════════════════════";
}

void DecisionDialog::onArduinoStatusChanged(const QString &status)
{
    statusLabel->setText("Arduino: " + status);
    
    if (status.contains("connecté", Qt::CaseInsensitive)) {
        statusLabel->setStyleSheet("font-size: 11px; color: #10B981; padding: 5px;");
    } else {
        statusLabel->setStyleSheet("font-size: 11px; color: #EF4444; padding: 5px;");
    }
}

void DecisionDialog::updateWaitingAnimation()
{
    // Animation avec des points
    QString dots[] = {"", ".", "..", "..."};
    animationStep = (animationStep + 1) % 4;
    
    waitingLabel->setText("⏳ En attente de décision Arduino" + dots[animationStep]);
}
