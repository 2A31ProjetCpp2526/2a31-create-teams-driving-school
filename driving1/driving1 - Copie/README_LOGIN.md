# Système de Login Intégré - Driving School

## Description
Le système de login a été intégré avec succès dans le projet drivingschool. L'application affiche maintenant une boîte de dialogue de connexion avant d'accéder à l'interface principale.

## Fonctionnalités

### Interface de Connexion
- **Champ Nom d'utilisateur** : Pour saisir l'ID ou le nom
- **Champ Mot de passe** : Pour saisir le mot de passe (masqué)
- **Bouton "Log in"** : Pour valider la connexion
- **Bouton "Récupérer mot de passe"** : Pour la récupération de mot de passe

### Identifiants de Test
Pour tester le système, utilisez l'un de ces identifiants :

1. **Administrateur** :
   - Nom d'utilisateur : `admin`
   - Mot de passe : `admin123`

2. **Utilisateur standard** :
   - Nom d'utilisateur : `user`
   - Mot de passe : `user123`

3. **École de conduite** :
   - Nom d'utilisateur : `drivingschool`
   - Mot de passe : `driving123`

## Fichiers Modifiés/Ajoutés

### Nouveaux Fichiers
- `login.h` : Déclaration de la classe LoginDialog
- `login.cpp` : Implémentation de la boîte de dialogue de connexion

### Fichiers Modifiés
- `main.cpp` : Intégration du système de login
- `drivingschool.pro` : Mise à jour des fichiers sources

## Architecture

### Flux d'Application
1. **Démarrage** : L'application lance la boîte de dialogue de connexion
2. **Authentification** : L'utilisateur saisit ses identifiants
3. **Validation** : Vérification des identifiants
4. **Accès** : Si valide, ouverture de la fenêtre principale
5. **Fermeture** : Si annulé, fermeture de l'application

### Sécurité
- Les mots de passe sont masqués lors de la saisie
- Validation côté client (à étendre avec une base de données)
- Gestion des erreurs de connexion

## Compilation

Pour compiler le projet avec Qt :

```bash
# Générer le Makefile
qmake drivingschool.pro

# Compiler le projet
mingw32-make
```

## Améliorations Futures

1. **Base de données** : Intégrer une vraie base de données pour l'authentification
2. **Chiffrement** : Chiffrer les mots de passe stockés
3. **Sessions** : Implémenter un système de sessions
4. **Récupération** : Développer la fonctionnalité de récupération de mot de passe
5. **Logs** : Ajouter un système de logs des connexions

## Utilisation

1. Lancez l'application
2. Saisissez vos identifiants dans la boîte de dialogue
3. Cliquez sur "Log in" ou appuyez sur Entrée
4. Si la connexion est réussie, vous accédez à l'interface principale
5. Si la connexion échoue, un message d'erreur s'affiche

Le système est maintenant prêt à être utilisé et peut être étendu selon les besoins de l'école de conduite.
