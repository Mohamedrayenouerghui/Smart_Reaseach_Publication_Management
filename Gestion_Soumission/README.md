# Gestion des Soumissions - Projet Standalone

Ce dossier contient un projet Qt **standalone** complet pour l'interface de gestion des soumissions.

## Fichiers inclus:

1. **CMakeLists.txt** - Configuration du projet CMake
2. **main.cpp** - Point d'entrée de l'application
3. **submission.h** - Fichier d'en-tête de la classe Submission
4. **submission.cpp** - Implémentation de la classe Submission
5. **submission.ui** - Interface graphique créée avec Qt Designer

## Comment ouvrir le projet:

1. Ouvrez Qt Creator
2. Fichier → Ouvrir un fichier ou un projet
3. Naviguez vers ce dossier `Gestion_Soumission`
4. Sélectionnez **CMakeLists.txt**
5. Configurez avec votre kit (Desktop Qt 6.7.3 MinGW 64-bit)
6. Cliquez sur "Configurer le projet"
7. Compilez et exécutez (Ctrl+R)

## Fonctionnalités de l'interface:

### CRUD (Fonctionnalités de base):
- **Create**: Enregistrer une nouvelle soumission
- **Read**: Voir toutes les soumissions dans le tableau
- **Update**: Changer le statut, attribuer des relecteurs
- **Delete**: Archiver une soumission traitée

### Métiers basiques (Boutons non-fonctionnels):
- **Trier**: Par date ou par titre
- **Rechercher**: Par titre ou par domaine
- **Exporter PDF**: Exporter la liste des soumissions
- **Statistiques**: Afficher les statistiques de performance

## Caractéristiques de l'interface:

- Design propre et professionnel
- Effets de survol sur tous les boutons (transparence 70% → 100%)
- Champ de date en lecture seule (pas de popup calendrier)
- Tableau avec couleurs alternées
- Formulaire de saisie avec validation
- Données d'exemple incluses

## Structure du projet:

```
Gestion_Soumission/
├── CMakeLists.txt      # Configuration CMake
├── main.cpp            # Point d'entrée
├── submission.h        # Header
├── submission.cpp      # Implémentation
├── submission.ui       # Interface Qt Designer
└── README.md           # Ce fichier
```

## Note:

Tous les boutons sont connectés mais les slots sont vides (pas de fonctionnalité).
C'est une interface visuelle uniquement, prête pour l'implémentation future.

Ce projet est **indépendant** et peut être compilé et exécuté seul, sans le projet principal.
