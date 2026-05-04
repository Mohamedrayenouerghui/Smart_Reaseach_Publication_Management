/*
 * ========================================
 * ARDUINO 2 - MONITEUR DE TEMPÉRATURE
 * Capteur DHT11 + Buzzer
 * ========================================
 * 
 * Composants:
 * - DHT11 (Température + Humidité) sur Pin 2
 * - Buzzer sur Pin 8
 * 
 * Port série: COM5 (à configurer dans Qt)
 * Baudrate: 9600
 * 
 * Protocole:
 * - Envoie "Temperature : XX.XX C"
 * - Envoie "Humidite : XX.XX %"
 * - Envoie "Variation : XX.XX C"
 * - Envoie ">>> FLAMME DETECTEE ! <<<" si température > seuil
 * - Envoie "Normal" si température normale
 */

#include <DHT.h>

// ==================== CONFIGURATION ====================
#define DHTPIN 2          // Pin du capteur DHT11
#define DHTTYPE DHT11     // Type de capteur (DHT11 ou DHT22)
#define BUZZER_PIN 8      // Pin du buzzer

#define TEMP_SEUIL 35.0   // Seuil de température (°C)
#define VAR_SEUIL 5.0     // Seuil de variation (°C)

// ==================== VARIABLES GLOBALES ====================
DHT dht(DHTPIN, DHTTYPE);

float lastTemp = 0.0;
float lastHum = 0.0;
bool flammeDetectee = false;

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  dht.begin();
  
  delay(2000);  // Attendre que le capteur se stabilise
  Serial.println("=== Moniteur Temperature DHT11 ===");
  Serial.println("Initialisation...");
  delay(1000);
}

// ==================== LOOP ====================
void loop() {
  // Lecture du capteur DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Vérifier si la lecture a échoué
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Erreur lecture DHT11");
    delay(2000);
    return;
  }
  
  // Calculer la variation de température
  float variation = 0.0;
  if (lastTemp != 0.0) {
    variation = abs(temperature - lastTemp);
  }
  
  // Envoyer les données au format attendu par Qt
  Serial.print("Temperature : ");
  Serial.print(temperature, 2);
  Serial.println(" C");
  
  Serial.print("Humidite : ");
  Serial.print(humidity, 2);
  Serial.println(" %");
  
  Serial.print("Variation   : ");
  Serial.print(variation, 2);
  Serial.println(" C");
  
  // Détection de flamme (température élevée OU variation rapide)
  if (temperature > TEMP_SEUIL || variation > VAR_SEUIL) {
    if (!flammeDetectee) {
      flammeDetectee = true;
      Serial.println(">>> FLAMME DETECTEE ! <<<");
      
      // Activer le buzzer (alarme)
      for (int i = 0; i < 5; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
      }
    }
  } else {
    if (flammeDetectee) {
      flammeDetectee = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
    Serial.println("Normal");
  }
  
  // Sauvegarder les valeurs pour la prochaine itération
  lastTemp = temperature;
  lastHum = humidity;
  
  // Attendre 2 secondes avant la prochaine lecture
  delay(2000);
}
