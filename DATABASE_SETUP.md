# Configuration de la Base de Données

## 📋 Prérequis

1. **Oracle Database** ou **SQL Server** installé
2. **ODBC Driver** installé pour votre base de données
3. **Qt SQL Module** (déjà configuré dans CMakeLists.txt)

## 🔧 Configuration ODBC (Windows)

### Étape 1: Ouvrir l'Administrateur de Sources de Données ODBC

1. Appuyez sur `Windows + R`
2. Tapez `odbcad32` et appuyez sur Entrée
3. Cela ouvre "Administrateur de sources de données ODBC"

### Étape 2: Créer une nouvelle source de données

1. Allez dans l'onglet **"DSN Système"** ou **"DSN Utilisateur"**
2. Cliquez sur **"Ajouter"**
3. Sélectionnez votre driver (ex: Oracle, SQL Server, MySQL)
4. Cliquez sur **"Terminer"**

### Étape 3: Configurer la source de données

Remplissez les informations:
- **Nom de la source de données**: `SmartResearch_DB`
- **Description**: Base de données Smart Research Management
- **Serveur**: localhost (ou l'adresse de votre serveur)
- **Port**: 1521 (Oracle) ou 1433 (SQL Server)
- **Base de données**: SmartResearch

### Étape 4: Tester la connexion

1. Cliquez sur **"Tester la connexion"**
2. Entrez vos identifiants:
   - Utilisateur: `admin`
   - Mot de passe: `admin123`
3. Si le test réussit, cliquez sur **"OK"**

## 📝 Modifier les Paramètres de Connexion

Ouvrez le fichier `connection.cpp` et modifiez:

```cpp
db.setDatabaseName("SmartResearch_DB"); // Votre nom DSN
db.setUserName("admin");                 // Votre utilisateur
db.setPassword("admin123");              // Votre mot de passe
```

## 🗄️ Structure de la Base de Données

### Tables Nécessaires:

1. **Publications**
   - ID_publication (INT, PRIMARY KEY)
   - titre (VARCHAR)
   - domaine (VARCHAR)
   - contenu_pdf (BLOB)
   - date_creation (DATE)
   - etat (VARCHAR)

2. **Utilisateurs**
   - ID_utilisateur (INT, PRIMARY KEY)
   - nom (VARCHAR)
   - prenom (VARCHAR)
   - email (VARCHAR)
   - mot_de_passe (VARCHAR)
   - role (VARCHAR)

3. **Soumissions**
   - ID_soumission (INT, PRIMARY KEY)
   - titre (VARCHAR)
   - auteur (VARCHAR)
   - domaine (VARCHAR)
   - date_soumission (DATE)
   - statut (VARCHAR)
   - relecteur (VARCHAR)

4. **Conferences**
   - ID_conference (INT, PRIMARY KEY)
   - nom (VARCHAR)
   - lieu (VARCHAR)
   - date_debut (DATE)
   - date_fin (DATE)
   - frais_inscription (DECIMAL)
   - etat (VARCHAR)
   - theme (VARCHAR)

5. **Laboratoires**
   - ID_laboratoire (INT, PRIMARY KEY)
   - nom (VARCHAR)
   - budget (DECIMAL)
   - date_creation (DATE)
   - domaine (VARCHAR)
   - localisation (VARCHAR)
   - nb_employes (INT)
   - classement (VARCHAR)

## ✅ Test de Connexion

Au démarrage de l'application, un message s'affichera:
- ✅ **Succès**: "Connexion à la base de données réussie!"
- ❌ **Échec**: "Échec de connexion à la base de données"

## 🔍 Débogage

Si la connexion échoue, vérifiez:
1. Le nom DSN est correct dans `connection.cpp`
2. Le service de base de données est démarré
3. Les identifiants sont corrects
4. Le firewall autorise la connexion
5. Le driver ODBC est bien installé

## 📚 Ressources

- [Qt SQL Documentation](https://doc.qt.io/qt-6/qtsql-index.html)
- [ODBC Configuration Guide](https://docs.microsoft.com/en-us/sql/odbc/)
