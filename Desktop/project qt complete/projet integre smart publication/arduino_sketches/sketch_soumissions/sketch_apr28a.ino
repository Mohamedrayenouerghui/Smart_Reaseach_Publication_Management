#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 4, 5, 6, 8, 9);

const int bouton = 12;

bool dernierEtat = HIGH;
unsigned long dernierTemps = 0;
int nbClics = 0;

const unsigned long delaiDouble = 400; // temps max entre 2 clics (ms)

// Pour le test de communication
unsigned long dernierTest = 0;
const unsigned long intervalTest = 5000; // Test toutes les 5 secondes

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(bouton, INPUT_PULLUP);

  lcd.print("Systeme Pret");
  lcd.setCursor(0, 1);
  lcd.print("En attente...");
  
  delay(1000); // Attendre que le port série soit prêt
  
  // Envoyer un message de démarrage
  Serial.println("ARDUINO_READY");
  Serial.flush(); // Forcer l'envoi
}

void loop() {
  // TEST: Envoyer un message toutes les 5 secondes pour vérifier la communication
  if (millis() - dernierTest > intervalTest) {
    Serial.println("TEST_ALIVE");
    Serial.flush();
    dernierTest = millis();
  }
  
  int etat = digitalRead(bouton);

  // Détection du clic (front descendant)
  if (etat == LOW && dernierEtat == HIGH) {
    nbClics++;
    dernierTemps = millis();
    delay(50); // anti-rebond
  }

  // Si un clic a été fait et le délai est dépassé
  if (nbClics > 0 && (millis() - dernierTemps > delaiDouble)) {
    lcd.clear();

    if (nbClics == 1) {
      // 1 clic = ACCEPTER
      lcd.setCursor(0, 0);
      lcd.print("Article");
      lcd.setCursor(0, 1);
      lcd.print("ACCEPTE");
      
      // Envoyer à Qt
      Serial.println("ACCEPTER");
      Serial.flush(); // Forcer l'envoi immédiat
      
    } else if (nbClics >= 2) {
      // 2 clics ou plus = REJETER
      lcd.setCursor(0, 0);
      lcd.print("Article");
      lcd.setCursor(0, 1);
      lcd.print("REJETE");
      
      // Envoyer à Qt
      Serial.println("REJETER");
      Serial.flush(); // Forcer l'envoi immédiat
    }

    nbClics = 0; // reset
  }

  // Lire les réponses de Qt
  if (Serial.available() > 0) {
    String reponse = Serial.readStringUntil('\n');
    reponse.trim();
    
    // Traiter les messages LCD personnalisés
    if (reponse.startsWith("LCD:")) {
      String message = reponse.substring(4); // Enlever "LCD:"
      lcd.clear();
      
      // Afficher le message (max 16 caractères par ligne)
      if (message.length() <= 16) {
        lcd.setCursor(0, 0);
        lcd.print(message);
      } else {
        // Diviser en 2 lignes
        lcd.setCursor(0, 0);
        lcd.print(message.substring(0, 16));
        lcd.setCursor(0, 1);
        lcd.print(message.substring(16));
      }
      
    } else if (reponse == "ACCEPTE_OK") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Confirmation:");
      lcd.setCursor(0, 1);
      lcd.print("ACCEPTE OK");
      delay(2000);
      
      // Retour à l'état initial
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Systeme Pret");
      lcd.setCursor(0, 1);
      lcd.print("En attente...");
      
    } else if (reponse == "REJETE_OK") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Confirmation:");
      lcd.setCursor(0, 1);
      lcd.print("REJETE OK");
      delay(2000);
      
      // Retour à l'état initial
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Systeme Pret");
      lcd.setCursor(0, 1);
      lcd.print("En attente...");
      
    } else if (reponse == "ERREUR_DB") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERREUR");
      lcd.setCursor(0, 1);
      lcd.print("Base de donnees");
      delay(3000);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Systeme Pret");
      lcd.setCursor(0, 1);
      lcd.print("En attente...");
      
    } else if (reponse == "AUCUNE_SOUMISSION") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Aucune");
      lcd.setCursor(0, 1);
      lcd.print("soumission");
      delay(2000);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Systeme Pret");
      lcd.setCursor(0, 1);
      lcd.print("En attente...");
    }
  }

  dernierEtat = etat;
}