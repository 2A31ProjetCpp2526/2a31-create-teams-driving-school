# Guide de dépannage - Connexion à la base de données Oracle

## Problèmes courants et solutions

### 1. Le DSN ODBC n'est pas configuré

**Symptôme:** Erreur "Data source name not found" ou similaire

**Solution:**
1. Ouvrez **ODBC Data Source Administrator** (64-bit):
   - Appuyez sur `Win + R`
   - Tapez `odbcad32.exe` et appuyez sur Entrée
   - OU allez dans: Panneau de configuration → Outils d'administration → Sources de données ODBC (64 bits)

2. Créez un nouveau DSN système:
   - Cliquez sur l'onglet **System DSN**
   - Cliquez sur **Add**
   - Sélectionnez le driver Oracle (ex: "Oracle dans OraDB23Home1" ou similaire)
   - Cliquez sur **Finish**

3. Configurez le DSN:
   - **Data Source Name:** `drivingschool`
   - **Description:** (optionnel) Base de données Driving School
   - **TNS Service Name:** Votre service Oracle (ex: `XE`, `ORCL`, etc.)
   - **User ID:** `drivingschool`
   - Cliquez sur **Test Connection** pour vérifier
   - Entrez le mot de passe: `123456`
   - Cliquez sur **OK**

### 2. Le driver Oracle ODBC n'est pas installé

**Symptôme:** Erreur "Driver not found" ou le driver n'apparaît pas dans la liste

**Solution:**
1. Installez **Oracle Instant Client** ou **Oracle Client complet**
   - Téléchargez depuis: https://www.oracle.com/database/technologies/instant-client/downloads.html
   - Installez la version correspondant à votre architecture (32-bit ou 64-bit)

2. Installez le **Oracle ODBC Driver**
   - Il est généralement inclus avec Oracle Client
   - Vérifiez dans ODBC Data Source Administrator que le driver apparaît

### 3. Oracle Client n'est pas installé ou mal configuré

**Symptôme:** Erreurs de connexion réseau ou "TNS: could not resolve"

**Solution:**
1. Vérifiez que Oracle Client est installé
2. Configurez le fichier `tnsnames.ora` si nécessaire:
   - Emplacement: `%ORACLE_HOME%\network\admin\tnsnames.ora`
   - Ajoutez une entrée pour votre base de données

### 4. Le service Oracle n'est pas démarré

**Symptôme:** Timeout ou "ORA-12541: TNS:no listener"

**Solution:**
1. Vérifiez que le service Oracle est démarré:
   - Ouvrez **Services** (Win + R, tapez `services.msc`)
   - Cherchez les services Oracle (ex: "OracleServiceXE", "OracleXETNSListener")
   - Assurez-vous qu'ils sont **Démarrés**

### 5. Identifiants incorrects

**Symptôme:** "ORA-01017: invalid username/password"

**Solution:**
1. Vérifiez les identifiants dans `connection.cpp`:
   - Username: `drivingschool`
   - Password: `123456`
2. Testez la connexion avec SQL*Plus ou SQL Developer

### 6. Alternative: Utiliser une chaîne de connexion directe

Si le DSN ne fonctionne pas, vous pouvez modifier `connection.cpp` pour utiliser une chaîne de connexion directe:

```cpp
// Remplacez cette ligne dans Connection::Connection():
db.setDatabaseName("DSN=drivingschool;UID=drivingschool;PWD=123456");

// Par (exemple):
db.setDatabaseName("Driver={Oracle dans OraDB23Home1};DBQ=localhost:1521/XE;UID=drivingschool;PWD=123456");
```

**Important:** 
- Remplacez `Oracle dans OraDB23Home1` par le nom exact de votre driver Oracle ODBC
- Remplacez `localhost:1521/XE` par votre `host:port/service_name`

### 7. Vérifier les drivers disponibles dans Qt

Le code amélioré affiche maintenant les drivers disponibles. Vérifiez la console de débogage pour voir quels drivers sont disponibles.

## Test de connexion

Pour tester votre connexion:

1. Utilisez **SQL*Plus**:
   ```bash
   sqlplus drivingschool/123456@drivingschool
   ```

2. Utilisez **SQL Developer** avec les mêmes identifiants

3. Testez dans **ODBC Data Source Administrator**:
   - Configurez le DSN
   - Cliquez sur **Test Connection**

## Notes importantes

- Assurez-vous que votre application Qt utilise la même architecture (32-bit ou 64-bit) que votre driver ODBC
- Si vous utilisez Qt 64-bit, vous devez configurer un DSN 64-bit
- Si vous utilisez Qt 32-bit, vous devez configurer un DSN 32-bit

