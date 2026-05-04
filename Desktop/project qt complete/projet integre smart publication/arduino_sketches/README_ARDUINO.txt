╔══════════════════════════════════════════════════════════════════╗
║     CONFIGURATION DES 3 ARDUINOS                                 ║
╚══════════════════════════════════════════════════════════════════╝

## 🎯 Vue d'Ensemble

Ce projet utilise **3 cartes Arduino** connectées simultanément sur **3 ports COM différents**.

═══════════════════════════════════════════════════════════════════

## 🔴 ARDUINO 1 : Décisions de Soumissions

**Dossier:** sketch_soumissions/
**Port COM:** COM4
**Baudrate:** 9600

**Composants:**
- Bouton poussoir → Pin 12
- LCD 1602A (mode 4 bits)
  - RS → Pin 2
  - E  → Pin 4
  - D4 → Pin 5
  - D5 → Pin 6
  - D6 → Pin 8
  - D7 → Pin 9

**Fonctionnalité:**
- 1 clic sur le bouton = ACCEPTER la soumission
- 2 clics rapides = REJETER la soumission
- LCD affiche les états et confirmations
- Communication bidirectionnelle avec Qt

**Protocole:**
Arduino → Qt: "ACCEPTER" ou "REJETER"
Qt → Arduino: "ACCEPTE_OK" ou "REJETE_OK"

**Code Qt:** arduinocontroller, arduinomanager, decisiondialog

═══════════════════════════════════════════════════════════════════

## 🟡 ARDUINO 2 : Moniteur de Température

**Dossier:** sketch_temperature/
**Port COM:** COM5 (à configurer)
**Baudrate:** 9600

**Composants:**
- Capteur DHT11 → Pin 2
- Buzzer → Pin 8

**Fonctionnalité:**
- Mesure température et humidité toutes les 2 secondes
- Détecte les flammes (température > 35°C OU variation > 5°C)
- Active le buzzer en cas d'alerte
- Met à jour les états des conférences et laboratoires dans la BD

**Protocole:**
Arduino → Qt:
- "Temperature : XX.XX C"
- "Humidite : XX.XX %"
- "Variation : XX.XX C"
- ">>> FLAMME DETECTEE ! <<<" (si alerte)
- "Normal" (si tout va bien)

**Code Qt:** arduinomonitor

**Base de données:**
- Table: CAPTEUR_DATA
- Colonnes: ID_CAPTEUR, DATE_LECTURE, TEMPERATURE, HUMIDITE, VARIATION, ETAT

═══════════════════════════════════════════════════════════════════

## 🟢 ARDUINO 3 : Identification Faciale

**Dossier:** PAS de code Arduino physique
**Port COM:** N/A (utilise uniquement OpenCV + Webcam)
**Baudrate:** N/A

**Composants:**
- Webcam USB (intégrée ou externe)
- OpenCV 4.5.5 installé localement

**Fonctionnalité:**
- Capture de 10 échantillons de visage par utilisateur
- Reconnaissance faciale par comparaison d'histogrammes
- Seuil de reconnaissance: 0.62 (THRESHOLD dans faceauthdialog.h)
- Stockage des échantillons: `<app_dir>/faces/<email_sanitized>/face_0.jpg ... face_9.jpg`
- Authentification sans mot de passe via reconnaissance faciale

**Protocole:**
- Pas de communication série (pas d'Arduino physique)
- Utilise uniquement OpenCV avec la webcam du PC
- Détection de visage avec Haar Cascade (haarcascade_frontalface_default.xml)
- Comparaison d'histogrammes normalisés (cv::HISTCMP_CORREL)

**Code Qt:** faceauthdialog, facecapturedialog, arduino (classe générique)

**Intégration:**
- Login: Bouton "Authentification Faciale" → ouvre FaceAuthDialog
- Users: Bouton "📷 Capturer le Visage" → ouvre FaceCaptureDialog
- Nécessite OpenCV installé: `C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64`

**Fichier Haar Cascade requis:**
`C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/etc/haarcascades/haarcascade_frontalface_default.xml`

═══════════════════════════════════════════════════════════════════

## 📋 INSTALLATION

### 1. Téléverser les codes Arduino

**Arduino 1 (Soumissions):**
```
1. Ouvrir Arduino IDE
2. Fichier → Ouvrir → sketch_soumissions/sketch_soumissions.ino
3. Outils → Port → COM4
4. Téléverser
5. FERMER Arduino IDE
```

**Arduino 2 (Température):**
```
1. Ouvrir Arduino IDE
2. Fichier → Ouvrir → sketch_temperature/sketch_temperature.ino
3. Installer la bibliothèque DHT:
   - Croquis → Inclure une bibliothèque → Gérer les bibliothèques
   - Rechercher "DHT sensor library" par Adafruit
   - Installer "DHT sensor library" + "Adafruit Unified Sensor"
4. Outils → Port → COM5
5. Téléverser
6. FERMER Arduino IDE
```

**Arduino 3 (Identification):**
✅ PAS de code Arduino à téléverser
- Utilise uniquement OpenCV + Webcam
- Vérifier que OpenCV est installé: `C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64`
- Vérifier que le fichier Haar Cascade existe:
  `C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/etc/haarcascades/haarcascade_frontalface_default.xml`
- Tester la webcam dans une autre application avant de lancer Qt

### 2. Vérifier les ports COM

Dans le Gestionnaire de périphériques Windows:
```
1. Win + X → Gestionnaire de périphériques
2. Ports (COM et LPT)
3. Noter les ports COM des 3 Arduinos
4. Mettre à jour les ports dans le code Qt si nécessaire
```

### 3. Lancer l'application Qt

```
1. Ouvrir Qt Creator
2. Ouvrir CMakeLists.txt
3. Compiler (Ctrl+B)
4. Lancer (Ctrl+R)
```

═══════════════════════════════════════════════════════════════════

## ⚠️ IMPORTANT

**TOUJOURS fermer Arduino IDE avant de lancer Qt !**

Arduino IDE bloque les ports série. Si Qt ne peut pas se connecter:
1. Fermer complètement Arduino IDE
2. Débrancher/rebrancher les Arduinos
3. Relancer Qt

═══════════════════════════════════════════════════════════════════

## 🔧 DÉPANNAGE

**Problème:** Arduino ne répond pas
**Solution:**
- Vérifier que Arduino IDE est fermé
- Vérifier le port COM dans le Gestionnaire de périphériques
- Vérifier les connexions physiques
- Regarder les logs dans "Application Output" de Qt Creator

**Problème:** Erreur "Port already in use"
**Solution:**
- Fermer Arduino IDE
- Fermer toutes les applications qui utilisent le port série
- Redémarrer Qt Creator

**Problème:** DHT11 retourne NaN
**Solution:**
- Vérifier les connexions (VCC, GND, Data)
- Attendre 2 secondes après le démarrage
- Vérifier que la bibliothèque DHT est installée

═══════════════════════════════════════════════════════════════════

## 📊 ÉTAT D'AVANCEMENT

✅ Arduino 1 - Soumissions (100% fonctionnel)
✅ Arduino 2 - Température (Code créé, à tester avec DHT11)
✅ Arduino 3 - Identification Faciale (100% intégré - OpenCV + Webcam)

═══════════════════════════════════════════════════════════════════
