# Documentation : Recherche et Exportation des Circuits

## 📋 Table des matières
1. [Fonctionnalité de Recherche](#fonctionnalité-de-recherche)
2. [Fonctionnalité d'Exportation](#fonctionnalité-dexportation)
3. [Problèmes résolus](#problèmes-résolus)
4. [Guide d'utilisation](#guide-dutilisation)

---

## 🔍 Fonctionnalité de Recherche

### Vue d'ensemble
La fonctionnalité de recherche permet de trouver des circuits dans la base de données Oracle en utilisant différents critères de recherche.

### Localisation
- **Fichier**: `circuit.cpp`
- **Fonction**: `Circuit::rechercher(const QString& critere)`
- **Interface**: Onglet "🔎 Recherche et Tri" dans la section Circuit

### Fonctionnement technique

#### 1. **Syntaxe SQL Oracle**
La fonction utilise la syntaxe Oracle correcte pour convertir les types numériques en texte :
```cpp
// ❌ ANCIENNE VERSION (incorrecte pour Oracle)
CAST(ID_CIRCUIT AS TEXT) LIKE '%critere%'

// ✅ NOUVELLE VERSION (correcte pour Oracle)
TO_CHAR(ID_CIRCUIT) LIKE '%critere%'
```

#### 2. **Critères de recherche**
La recherche s'effectue sur **tous les champs** du circuit :
- **ID Circuit** : Recherche par numéro d'identification
- **Nom Circuit** : Recherche par nom (insensible à la casse)
- **Description** : Recherche dans la description (insensible à la casse)
- **Distance** : Recherche par distance en kilomètres
- **Durée** : Recherche par durée estimée en minutes
- **Difficulté** : Recherche par niveau de difficulté (Facile, Moyen, Difficile)
- **Immatricule** : Recherche par immatriculation du véhicule (insensible à la casse)

#### 3. **Protection contre les injections SQL**
La fonction échappe automatiquement les apostrophes dans le critère de recherche :
```cpp
QString critereEscaped = critere;
critereEscaped.replace("'", "''"); // Échapper les apostrophes
```

#### 4. **Logs de débogage**
Des logs détaillés sont ajoutés pour faciliter le diagnostic :
- Critère de recherche utilisé
- Requête SQL générée
- Nombre de résultats trouvés
- Erreurs éventuelles

### Exemple de requête SQL générée
```sql
SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE 
FROM CIRCUIT 
WHERE TO_CHAR(ID_CIRCUIT) LIKE '%1234%' 
   OR UPPER(NOM_CIRCUIT) LIKE UPPER('%1234%') 
   OR UPPER(DESCRIPTION) LIKE UPPER('%1234%') 
   OR TO_CHAR(DISTANCE) LIKE '%1234%' 
   OR TO_CHAR(DUREE_ESTIMEE) LIKE '%1234%' 
   OR UPPER(DIFFICULTE) LIKE UPPER('%1234%') 
   OR UPPER(IMMATRICULE) LIKE UPPER('%1234%')
```

### Utilisation dans l'interface
1. Aller dans la section **Circuit** (icône de voiture)
2. Cliquer sur l'onglet **"🔎 Recherche et Tri"**
3. Entrer le critère de recherche dans le champ **"Rechercher un circuit :"**
4. Cliquer sur le bouton **"Rechercher"** (icône de loupe)
5. Les résultats s'affichent dans le tableau ci-dessous

---

## 📤 Fonctionnalité d'Exportation

### Vue d'ensemble
La fonctionnalité d'exportation permet de sauvegarder les données des circuits dans un fichier CSV (Comma-Separated Values) pour analyse, archivage ou traitement externe.

### Localisation
- **Fichier**: `mainwindow.cpp`
- **Fonction**: `MainWindow::on_pushButton_export_circuit_clicked()`
- **Fonction utilitaire**: `MainWindow::escapeCsvField(const QString& field)`
- **Interface**: Bouton "Exporter" dans la section Circuit

### Fonctionnement technique

#### 1. **Détermination de la source de données**
La fonction détecte automatiquement quel tableau utiliser :
- **Onglet "Accueil"** : Exporte tous les circuits du tableau principal
- **Onglet "Recherche et Tri"** : Exporte uniquement les circuits filtrés/recherchés

```cpp
if (ui->tabWidget_circuit->currentIndex() == 0) {
    model = ui->tableView_circuits->model(); // Tableau principal
} else {
    model = ui->tableView_circuits_recherche->model(); // Tableau de recherche
}
```

#### 2. **Format CSV**
Le fichier CSV généré respecte les standards :
- **Séparateur** : Virgule (`,`)
- **Encodage** : UTF-8 (support des accents et caractères spéciaux)
- **En-tête** : Première ligne contient les noms des colonnes
- **Échappement** : Les valeurs contenant des virgules ou guillemets sont protégées

#### 3. **Échappement des champs CSV**
La fonction `escapeCsvField()` protège les valeurs spéciales :
- Si le champ contient une virgule, un guillemet ou un saut de ligne → entouré de guillemets
- Les guillemets doubles dans le champ sont doublés (`"` devient `""`)

**Exemple** :
```
Nom normal: Circuit A
Nom avec virgule: "Circuit A, version longue"
Nom avec guillemets: "Circuit ""Spécial"""
```

#### 4. **Nom de fichier par défaut**
Le nom de fichier inclut la date et l'heure pour éviter les écrasements :
```
circuits_export_20250107_143022.csv
Format: circuits_export_YYYYMMDD_HHMMSS.csv
```

#### 5. **Structure du fichier CSV**
```csv
ID Circuit,Nom Circuit,Description,Distance (km),Durée (min),Difficulté,Immatricule
123,Circuit A,Description du circuit,10.5,45,Moyen,ABC-123
124,Circuit B,Autre description,15.2,60,Facile,DEF-456
```

### Utilisation dans l'interface
1. Afficher les circuits à exporter (soit tous, soit après une recherche)
2. Cliquer sur le bouton **"Exporter"** (icône d'exportation)
3. Choisir l'emplacement et le nom du fichier dans la boîte de dialogue
4. Confirmer la sauvegarde
5. Un message de confirmation affiche le nombre de circuits exportés

### Compatibilité
Le fichier CSV généré est compatible avec :
- ✅ Microsoft Excel
- ✅ LibreOffice Calc
- ✅ Google Sheets
- ✅ OpenOffice Calc
- ✅ Tout tableur supportant le format CSV

---

## 🔧 Problèmes résolus

### Problème 1 : Recherche ne fonctionnait pas
**Symptôme** : La recherche affichait "Aucun circuit trouvé" même quand des circuits existaient.

**Cause** : Utilisation de la syntaxe SQL incorrecte pour Oracle :
- `CAST(ID_CIRCUIT AS TEXT)` n'est pas valide en Oracle
- Oracle nécessite `TO_CHAR()` pour convertir en texte

**Solution** : Remplacement de toutes les occurrences de `CAST(... AS TEXT)` par `TO_CHAR(...)` dans la requête SQL.

**Fichiers modifiés** :
- `circuit.cpp` : Fonction `rechercher()`

### Problème 2 : Fonctionnalité d'exportation manquante
**Symptôme** : Le bouton d'exportation affichait seulement un message "Fonctionnalité d'exportation à implémenter".

**Solution** : Implémentation complète de l'exportation CSV avec :
- Détection automatique du tableau source
- Échappement correct des champs CSV
- Support UTF-8 pour les caractères spéciaux
- Messages d'erreur et de confirmation

**Fichiers modifiés** :
- `mainwindow.cpp` : Fonction `on_pushButton_export_circuit_clicked()` et `escapeCsvField()`
- `mainwindow.h` : Déclaration de `escapeCsvField()`

### Améliorations supplémentaires
1. **Logs de débogage** : Ajout de logs détaillés pour faciliter le diagnostic
2. **Gestion d'erreurs** : Messages d'erreur clairs pour l'utilisateur
3. **Documentation inline** : Commentaires détaillés dans le code expliquant le fonctionnement

---

## 📖 Guide d'utilisation

### Scénario 1 : Rechercher un circuit par ID
1. Aller dans **Circuit** → **Recherche et Tri**
2. Entrer `1234` dans le champ de recherche
3. Cliquer sur **Rechercher**
4. Le circuit avec l'ID 1234 s'affiche (s'il existe)

### Scénario 2 : Rechercher par nom partiel
1. Entrer `AZE` dans le champ de recherche
2. Tous les circuits dont le nom contient "AZE" s'affichent (ex: "AZE", "AZER", "MAZE")

### Scénario 3 : Exporter tous les circuits
1. Aller dans **Circuit** → **Accueil**
2. S'assurer que tous les circuits sont affichés
3. Cliquer sur **Exporter**
4. Choisir l'emplacement et sauvegarder

### Scénario 4 : Exporter les résultats d'une recherche
1. Effectuer une recherche (ex: circuits avec difficulté "Moyen")
2. Cliquer sur **Exporter**
3. Seuls les circuits filtrés seront exportés

### Scénario 5 : Analyser les données exportées
1. Ouvrir le fichier CSV dans Excel
2. Utiliser les fonctionnalités d'Excel pour :
   - Trier les données
   - Créer des graphiques
   - Filtrer par colonnes
   - Calculer des statistiques

---

## 🐛 Dépannage

### La recherche ne trouve rien
1. Vérifier que la base de données est connectée
2. Vérifier les logs de débogage dans la console
3. Vérifier que le critère de recherche correspond aux données
4. Essayer une recherche plus large (ex: "A" au lieu de "AZER")

### L'exportation échoue
1. Vérifier les permissions d'écriture dans le dossier cible
2. Vérifier qu'il y a des données à exporter
3. Vérifier l'espace disque disponible
4. Essayer un autre emplacement de sauvegarde

### Le fichier CSV ne s'ouvre pas correctement
1. Vérifier que le fichier a l'extension `.csv`
2. Ouvrir avec un autre programme (Excel, LibreOffice)
3. Vérifier l'encodage (doit être UTF-8)

---

## 📝 Notes techniques

### Dépendances
- Qt 6.x (QFileDialog, QTextStream, QStandardPaths)
- Base de données Oracle avec driver ODBC
- Support UTF-8 pour les caractères spéciaux

### Performance
- La recherche utilise des index SQL pour optimiser les performances
- L'exportation est linéaire : O(n) où n = nombre de circuits
- Pour de grandes quantités de données (>1000 circuits), l'exportation peut prendre quelques secondes

### Sécurité
- Protection contre les injections SQL via échappement des apostrophes
- Validation des entrées utilisateur
- Gestion des erreurs pour éviter les crashes

---

## 📞 Support

Pour toute question ou problème :
1. Consulter les logs de débogage dans la console
2. Vérifier la documentation Oracle pour la syntaxe SQL
3. Tester avec des données simples pour isoler le problème

---

**Dernière mise à jour** : 2025-01-07
**Version** : 1.0


