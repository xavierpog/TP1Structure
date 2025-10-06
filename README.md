# Projet TP - Gestion des Dossiers Professeurs avec Listes Chaînées

## 📋 Description du Projet

Ce projet implémente un système de gestion des dossiers de professeurs utilisant des listes chaînées en C++. Le programme lit des données depuis un fichier texte, crée une structure de listes chaînées en mémoire, et permet d'effectuer diverses opérations sur ces données.

## 🚀 Configuration dans Visual Studio 2022

### Étapes d'installation:

1. **Créer un nouveau projet C++:**
   - Ouvrir Visual Studio 2022
   - File → New → Project
   - Sélectionner "Console App" (C++)
   - Nommer le projet (ex: "GestionProfesseurs")
   - Choisir l'emplacement
   - Cliquer "Create"

2. **Ajouter les fichiers au projet:**
   - Copier `main.cpp`, `PF.txt`, et `FT.txt` dans le dossier du projet
   - Dans Solution Explorer, clic droit sur "Source Files" → Add → Existing Item
   - Sélectionner `main.cpp`
   - Pour les fichiers .txt: clic droit sur le projet → Add → Existing Item

3. **Configuration du projet:**
   - Clic droit sur le projet → Properties
   - Configuration: All Configurations
   - C/C++ → General → Additional Include Directories: ajuster si nécessaire
   - Linker → System → Console (/SUBSYSTEM:CONSOLE)

## 📁 Structure des Fichiers

### PF.txt (Fichier de données des professeurs)
Format:
```
nom_professeur
ancienneté
cours1
cours2
...
&
étudiant1
étudiant2
...
&

[professeur suivant...]
```

### FT.txt (Fichier des transactions)
Commandes disponibles:
- `*` : Afficher le cours le plus demandé
- `#` : Afficher le professeur avec le plus d'étudiants
- `nom_professeur` : Supprimer tous les professeurs avec ce nom
- `%` suivi de `nom_cours` : Afficher le nombre de professeurs pour ce cours
- `$` : Recopier la liste mise à jour dans PF.txt

## 🛠️ Structures de Données Implémentées

### Structures C++:
```cpp
struct Cours {
    string nomCours;
    Cours* suivant;
};

struct Etudiants {
    string nomEtudiant;
    Etudiants* suivant;
};

struct Professeur {
    string nom;
    int ancien;
    Cours* listecours;
    Etudiants* listeetudiants;
    Professeur* suivant;
};
```

### Classe DossierProfesseur:
- **Constructeur**: Charge les données depuis PF.txt
- **Destructeur**: Libère toute la mémoire allouée
- **supprimer()**: Supprime les professeurs avec un nom donné
- **afficherleprofplusetudiant()**: Trouve le prof avec le plus d'étudiants
- **affichercoursplusdemande()**: Trouve le cours le plus demandé
- **affichernbreprofpouruncours()**: Compte les profs pour un cours
- **recopier()**: Sauvegarde la liste dans un fichier

## 🔧 Fonctionnalités Implémentées

### ✅ Gestion Mémoire Complète
- Destructeurs automatiques pour éviter les fuites mémoire
- Libération de toutes les listes chaînées (cours, étudiants, professeurs)
- Gestion d'erreurs pour l'ouverture des fichiers

### ✅ Opérations sur les Listes Chaînées
- Insertion en tête de liste
- Suppression de nœuds avec gestion des pointeurs
- Traversée et recherche dans les listes
- Comptage d'éléments dans les sous-listes

### ✅ Lecture/Écriture de Fichiers
- Parsing ligne par ligne avec getline()
- Gestion des séparateurs (&)
- Écriture formatée dans les fichiers de sortie
- Validation des données d'entrée

### ✅ Algorithmes de Recherche et Tri
- Recherche du maximum avec critères multiples (étudiants + ancienneté)
- Comptage de fréquences pour les cours
- Algorithmes de comparaison avec tie-breaking

## 🚦 Compilation et Exécution

### Dans Visual Studio 2022:
1. Appuyer sur F5 ou cliquer "Start Debugging"
2. Ou Ctrl+F5 pour "Start Without Debugging"

### En ligne de commande (optionnel):
```bash
g++ -std=c++11 main.cpp -o gestion_profs
./gestion_profs
```

## 📊 Exemple d'Exécution

Le programme affiche:
1. Chargement des données depuis PF.txt
2. Liste initiale des professeurs
3. Exécution des transactions depuis FT.txt
4. Résultats de chaque opération
5. Liste finale mise à jour

## 🐛 Gestion d'Erreurs

Le programme inclut une gestion robuste des erreurs:
- Vérification de l'ouverture des fichiers
- Validation des données d'entrée
- Gestion des listes vides
- Protection contre les accès mémoire invalides

## 📚 Concepts C++ Utilisés

- **Pointeurs et références**
- **Allocation dynamique (new/delete)**
- **Classes et encapsulation**
- **Destructeurs automatiques**
- **Flux de fichiers (ifstream/ofstream)**
- **Manipulation de chaînes (string, stringstream)**
- **Structures de données chaînées**

## 🎯 Points Clés du Projet

1. **Respect de l'architecture imposée**: Structures exactes du cahier des charges
2. **Gestion mémoire sans fuites**: Destructeurs complets
3. **Robustesse**: Le programme ne plante jamais
4. **Lisibilité**: Code bien commenté et structuré
5. **Fonctionnalité complète**: Toutes les opérations demandées

## 📝 Notes d'Implémentation

- Les listes sont implémentées comme des listes simplement chaînées
- L'insertion se fait en tête pour l'efficacité (O(1))
- La suppression gère tous les cas (tête, milieu, fin)
- Les critères de tie-breaking sont respectés (ancienneté)
- Le format de fichier est strictement respecté

Ce projet démontre une maîtrise complète des pointeurs, des listes chaînées, et de la gestion mémoire en C++, tout en étant parfaitement fonctionnel dans Visual Studio 2022.
```