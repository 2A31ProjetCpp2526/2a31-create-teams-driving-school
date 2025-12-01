# Guide d'Intégration du Travail d'un Membre du Groupe

## 📋 Méthodes d'Intégration

### Méthode 1 : Fichiers individuels (copier-coller)

Si votre collègue vous a envoyé des fichiers individuels (`.cpp`, `.h`, `.ui`, etc.) :

#### Étapes :

1. **Identifier les fichiers reçus**
   - Fichiers sources (`.cpp`)
   - Fichiers headers (`.h`)
   - Fichiers UI (`.ui`)
   - Fichiers SQL (`.sql`)
   - Autres fichiers de configuration

2. **Sauvegarder votre travail actuel**
   ```bash
   # Créer une sauvegarde
   cp -r . ../backup_moemen_$(date +%Y%m%d)
   ```

3. **Copier les fichiers reçus**
   - Copiez les fichiers dans le répertoire approprié
   - Remplacez les fichiers existants si nécessaire
   - **ATTENTION** : Vérifiez les différences avant de remplacer

4. **Mettre à jour le fichier `.pro`**
   - Si de nouveaux fichiers ont été ajoutés, mettez à jour `moemen.pro` :
   ```pro
   SOURCES += \
       connection.cpp \
       login.cpp \
       main.cpp \
       mainwindow.cpp \
       seance.cpp \
       vehicule.cpp \
       circuit.cpp \
       nouveau_fichier.cpp  # Ajouter ici
   
   HEADERS += \
       connection.h \
       login.h \
       mainwindow.h \
       seance.h \
       vehicule.h \
       circuit.h \
       nouveau_fichier.h  # Ajouter ici
   ```

5. **Vérifier les dépendances**
   - Vérifiez que tous les includes sont corrects
   - Vérifiez que les noms de classes/fonctions correspondent

---

### Méthode 2 : Dossier complet du projet

Si votre collègue vous a envoyé tout le dossier du projet :

#### Étapes :

1. **Comparer les structures**
   - Comparez les fichiers entre les deux versions
   - Identifiez les fichiers nouveaux, modifiés, ou supprimés

2. **Fusion manuelle recommandée**
   - Ne remplacez pas tout le projet d'un coup
   - Fusionnez fichier par fichier pour éviter de perdre votre travail

3. **Outils de comparaison**
   - Utilisez un outil de comparaison (WinMerge, Beyond Compare, VS Code)
   - Comparez les fichiers un par un
   - Fusionnez les changements manuellement

---

### Méthode 3 : Utilisation de Git (Recommandé)

Si vous utilisez Git pour le versionnement :

#### Étapes :

1. **Créer une branche pour l'intégration**
   ```bash
   git checkout -b integration-collegue
   ```

2. **Ajouter les fichiers du collègue**
   ```bash
   # Si fichiers individuels
   git add fichier1.cpp fichier2.h
   
   # Si dossier complet
   git add .
   ```

3. **Vérifier les changements**
   ```bash
   git status
   git diff
   ```

4. **Résoudre les conflits**
   - Si conflits, éditez les fichiers manuellement
   - Marquez comme résolu : `git add fichier_resolu.cpp`

5. **Tester la compilation**
   ```bash
   qmake moemen.pro
   make  # ou mingw32-make sur Windows
   ```

6. **Fusionner dans la branche principale**
   ```bash
   git checkout main
   git merge integration-collegue
   ```

---

### Méthode 4 : Patch/Diff

Si votre collègue vous a envoyé un patch :

#### Étapes :

1. **Appliquer le patch**
   ```bash
   git apply patch.diff
   # ou
   patch -p1 < patch.diff
   ```

2. **Vérifier les changements**
   - Examinez les modifications
   - Testez la compilation

---

## 🔍 Vérifications Avant Intégration

### Checklist de sécurité :

- [ ] **Sauvegarde créée** : Votre travail actuel est sauvegardé
- [ ] **Fichiers identifiés** : Vous savez quels fichiers ont changé
- [ ] **Dépendances vérifiées** : Tous les includes sont corrects
- [ ] **Compilation testée** : Le projet compile sans erreurs
- [ ] **Fonctionnalités testées** : Les fonctionnalités existantes fonctionnent toujours

---

## 🛠️ Résolution de Conflits

### Si vous avez des conflits de code :

1. **Identifier le conflit**
   ```cpp
   <<<<<<< HEAD
   // Votre code
   =======
   // Code du collègue
   >>>>>>> integration
   ```

2. **Décider quelle version garder**
   - Garder votre version
   - Garder la version du collègue
   - Fusionner les deux versions

3. **Résoudre manuellement**
   - Supprimez les marqueurs de conflit (`<<<<<<<`, `=======`, `>>>>>>>`)
   - Gardez le code approprié

---

## 📝 Exemple d'Intégration : Nouveau Module

### Scénario : Votre collègue a créé un module "Client"

#### Fichiers reçus :
- `client.cpp`
- `client.h`

#### Étapes :

1. **Copier les fichiers**
   ```bash
   cp client.cpp .
   cp client.h .
   ```

2. **Mettre à jour `moemen.pro`**
   ```pro
   SOURCES += \
       ...
       circuit.cpp \
       client.cpp  # Ajouter
   
   HEADERS += \
       ...
       circuit.h \
       client.h  # Ajouter
   ```

3. **Intégrer dans `mainwindow.h`**
   ```cpp
   #include "client.h"  // Ajouter
   
   class MainWindow {
       ...
       Client C;  // Ajouter instance
   };
   ```

4. **Compiler et tester**
   ```bash
   qmake moemen.pro
   make
   ```

---

## ⚠️ Points d'Attention

### Problèmes courants :

1. **Conflits de noms**
   - Vérifiez que les noms de classes/fonctions ne sont pas en conflit
   - Utilisez des namespaces si nécessaire

2. **Versions de Qt différentes**
   - Vérifiez la version de Qt utilisée
   - Adaptez le code si nécessaire

3. **Chemins de fichiers**
   - Vérifiez les chemins relatifs/absolus
   - Adaptez selon votre structure

4. **Base de données**
   - Vérifiez que les schémas de base de données sont compatibles
   - Exécutez les scripts SQL si nécessaire

---

## 🧪 Tests Après Intégration

### Tests à effectuer :

1. **Compilation**
   ```bash
   qmake moemen.pro
   make clean
   make
   ```

2. **Fonctionnalités existantes**
   - Tester toutes les fonctionnalités qui fonctionnaient avant
   - Vérifier qu'aucune régression n'a été introduite

3. **Nouvelles fonctionnalités**
   - Tester les nouvelles fonctionnalités ajoutées par votre collègue

4. **Base de données**
   - Vérifier la connexion
   - Tester les requêtes SQL

---

## 📞 Support

Si vous rencontrez des problèmes :

1. **Vérifier les logs**
   - Console de débogage
   - Fichiers de log

2. **Comparer avec la version originale**
   - Utiliser un outil de diff
   - Identifier les différences

3. **Demander de l'aide**
   - Contacter votre collègue
   - Vérifier la documentation

---

## 📌 Exemple Pratique : Intégration d'un Module Circuit

### Fichiers reçus :
- `circuit.cpp` (modifié)
- `circuit.h` (modifié)
- `create_circuit_table.sql` (nouveau)

### Commandes :

```bash
# 1. Sauvegarder
cp circuit.cpp circuit.cpp.backup
cp circuit.h circuit.h.backup

# 2. Copier les nouveaux fichiers
cp /chemin/vers/nouveau/circuit.cpp .
cp /chemin/vers/nouveau/circuit.h .
cp /chemin/vers/create_circuit_table.sql .

# 3. Vérifier les différences
diff circuit.cpp circuit.cpp.backup

# 4. Compiler
qmake moemen.pro
make clean
make

# 5. Tester
./moemen.exe
```

---

**Dernière mise à jour** : 2025-01-07


