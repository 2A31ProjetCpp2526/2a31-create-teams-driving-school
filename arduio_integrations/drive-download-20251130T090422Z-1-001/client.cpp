#include "client.h"
#include "connection.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>
#include <QRegularExpression>
#include <QMessageBox>

client::client()
{
    id_client = 0;
    nom_client = "";
    prenom_client = "";
    cin_client = "";
    date_naissance = QDate();
    adresse = "";
    tel_client = 0;
    email = "";
    latitude = 0.0;
    longitude = 0.0;
}

client::client(QString nom_client, QString prenom_client, QString cin_client,
               QDate date_naissance, QString adresse, int tel_client,
               QString email, double latitude, double longitude)
{
    this->id_client = 0;
    this->nom_client = nom_client;
    this->prenom_client = prenom_client;
    this->cin_client = cin_client;
    this->date_naissance = date_naissance;
    this->adresse = adresse;
    this->tel_client = tel_client;
    this->email = email;
    this->latitude = latitude;
    this->longitude = longitude;
}

int client::getid_client() const { return id_client; }
QString client::getnom_client() const { return nom_client; }
QString client::getprenom_client() const { return prenom_client; }
QString client::getcin_client() const { return cin_client; }
QDate client::getdate_naissance() const { return date_naissance; }
QString client::getadresse() const { return adresse; }
int client::gettel_client() const { return tel_client; }
QString client::getemail() const { return email; }
double client::getlatitude() const { return latitude; }
double client::getlongitude() const { return longitude; }

void client::setid_client(int id_client) { this->id_client = id_client; }
void client::setnom_client(QString nom_client) { this->nom_client = nom_client; }
void client::setprenom_client(QString prenom_client) { this->prenom_client = prenom_client; }
void client::setcin_client(QString cin_client) { this->cin_client = cin_client; }
void client::setdate_naissance(QDate date_naissance) { this->date_naissance = date_naissance; }
void client::setadresse(QString adresse) { this->adresse = adresse; }
void client::settel_client(int tel_client) { this->tel_client = tel_client; }
void client::setemail(QString email) { this->email = email; }
void client::setlatitude(double latitude) { this->latitude = latitude; }
void client::setlongitude(double longitude) { this->longitude = longitude; }

bool client::validerDonnees() const
{
    QRegularExpression alphaRegex("^[a-zA-Z]+$");
    if (nom_client.isEmpty() || prenom_client.isEmpty()) {
        qDebug() << "Erreur : Nom ou prénom vide.";
        return false;
    }
    if (!alphaRegex.match(nom_client).hasMatch() || !alphaRegex.match(prenom_client).hasMatch()) {
        qDebug() << "Erreur : Le nom et le prénom doivent contenir uniquement des lettres.";
        return false;
    }

    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin_client).hasMatch()) {
        qDebug() << "Erreur : CIN doit être une chaîne de 8 chiffres.";
        return false;
    }

    if (tel_client < 10000000 || tel_client > 99999999) {
        qDebug() << "Erreur : Téléphone doit être un nombre à 8 chiffres.";
        return false;
    }

    return true;
}

bool client::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    // Validation détaillée avec messages spécifiques
    QString validationError = "";

    // Vérifier nom et prénom
    QRegularExpression alphaRegex("^[a-zA-Z]+$");
    if (nom_client.isEmpty() || prenom_client.isEmpty()) {
        validationError = "Le nom et le prénom sont obligatoires!";
    } else if (!alphaRegex.match(nom_client).hasMatch()) {
        validationError = QString("Le nom '%1' contient des caractères invalides.\nLe nom doit contenir uniquement des lettres (a-z, A-Z).").arg(nom_client);
    } else if (!alphaRegex.match(prenom_client).hasMatch()) {
        validationError = QString("Le prénom '%1' contient des caractères invalides.\nLe prénom doit contenir uniquement des lettres (a-z, A-Z).").arg(prenom_client);
    }

    // Vérifier CIN
    if (validationError.isEmpty()) {
        QRegularExpression cinRegex("^[0-9]{8}$");
        if (!cinRegex.match(cin_client).hasMatch()) {
            validationError = QString("Le CIN '%1' est invalide.\nLe CIN doit être une chaîne de 8 chiffres.").arg(cin_client);
        }
    }

    // Vérifier téléphone
    if (validationError.isEmpty()) {
        if (tel_client < 10000000 || tel_client > 99999999) {
            validationError = QString("Le numéro de téléphone '%1' est invalide.\nLe téléphone doit être un nombre à 8 chiffres.").arg(tel_client);
        }
    }

    if (!validationError.isEmpty()) {
        qDebug() << "❌ Validation échouée:" << validationError;
        QMessageBox::warning(nullptr, "Erreur de validation", validationError);
        return false;
    }

    qDebug() << "=== Tentative d'ajout d'un client ===";
    qDebug() << "CIN à vérifier:" << cin_client;

    // Vérifier que le CIN est unique
    bool cinExists = checkIfCinExists(cin_client);
    qDebug() << "Le CIN existe-t-il?" << cinExists;

    if (cinExists) {
        qDebug() << "❌ Erreur : Ce CIN existe déjà dans la base de données:" << cin_client;
        QMessageBox::warning(nullptr, "CIN déjà existant",
                             QString("Le CIN '%1' existe déjà dans la base de données!\n\n"
                                     "Veuillez utiliser un autre CIN.").arg(cin_client));
        return false;
    }

    QSqlQuery query;
    // Récupérer le prochain ID_CLIENT (séquence ou MAX)
    int nextId = 1;
    QSqlQuery idQuery;
    if (idQuery.exec("SELECT NVL(MAX(ID_CLIENT), 0) + 1 FROM CLIENT")) {
        if (idQuery.next()) {
            nextId = idQuery.value(0).toInt();
            qDebug() << "Prochain ID_CLIENT:" << nextId;
        }
    } else {
        qDebug() << "Erreur lors de la récupération du MAX(ID_CLIENT):" << idQuery.lastError().text();
    }

    // Vérifier dynamiquement la présence des colonnes LATITUDE / LONGITUDE
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    if (hasLatitudeLongitude) {
        query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8, LATITUDE, LONGITUDE) "
                      "VALUES (:id_client, :nom_client, :prenom_client, :cin_client, :date_naissance, :adresse, :tel_client, :email, :latitude, :longitude)");
    } else {
        qDebug() << "⚠️ Colonnes LATITUDE / LONGITUDE absentes dans CLIENT, insertion sans coordonnées GPS.";
        query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8) "
                      "VALUES (:id_client, :nom_client, :prenom_client, :cin_client, :date_naissance, :adresse, :tel_client, :email)");
    }

    query.bindValue(":id_client", nextId);
    query.bindValue(":nom_client", nom_client);
    query.bindValue(":prenom_client", prenom_client);
    query.bindValue(":cin_client", cin_client);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel_client", tel_client);
    query.bindValue(":email", email);
    if (hasLatitudeLongitude) {
        if (latitude == 0.0 && longitude == 0.0) {
            query.bindValue(":latitude", QVariant(QVariant::Double));
            query.bindValue(":longitude", QVariant(QVariant::Double));
        } else {
            query.bindValue(":latitude", latitude);
            query.bindValue(":longitude", longitude);
        }
    }

    qDebug() << "Exécution de l'INSERT...";
    qDebug() << "Données à insérer:";
    qDebug() << "  ID:" << nextId;
    qDebug() << "  Nom:" << nom_client;
    qDebug() << "  Prénom:" << prenom_client;
    qDebug() << "  CIN:" << cin_client;
    qDebug() << "  Téléphone:" << tel_client;

    if (!query.exec()) {
        QString errorText = query.lastError().text();
        QString errorCode = query.lastError().nativeErrorCode();
        QString errorDatabase = query.lastError().databaseText();

        qDebug() << "❌ Erreur lors de l'insertion:";
        qDebug() << "   Message:" << errorText;
        qDebug() << "   Code d'erreur natif:" << errorCode;
        qDebug() << "   Erreur database:" << errorDatabase;
        qDebug() << "   Requête:" << query.lastQuery();

        // Message d'erreur détaillé
        QString detailedError = QString("Erreur lors de l'ajout du client!\n\n"
                                        "Message: %1\n"
                                        "Code d'erreur: %2")
                                    .arg(errorText)
                                    .arg(errorCode.isEmpty() ? "N/A" : errorCode);

        if (!errorDatabase.isEmpty()) {
            detailedError += "\n\nDétails Oracle:\n" + errorDatabase;
        }

        // Détecter les erreurs communes
        if (errorDatabase.contains("ORA-00001") || errorText.contains("unique constraint")) {
            detailedError += "\n\n⚠️ Cette erreur indique généralement qu'un enregistrement avec ces données existe déjà (CIN, email, etc.)";
        } else if (errorDatabase.contains("ORA-01400") || errorText.contains("NOT NULL")) {
            detailedError += "\n\n⚠️ Un champ obligatoire est manquant ou vide";
        } else if (errorDatabase.contains("ORA-02291") || errorText.contains("foreign key")) {
            detailedError += "\n\n⚠️ Erreur de clé étrangère - vérifiez les références";
        }

        QMessageBox::critical(nullptr, "Erreur SQL", detailedError);
        return false;
    }

    qDebug() << "Client ajouté avec succès! ID:" << nextId;
    // Mettre à jour l'ID du client après insertion
    id_client = nextId;
    QMessageBox::information(nullptr, "Succès", "Client ajouté avec succès ✓");
    return true;
}

bool client::modifierE()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifierE()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    if (id_client <= 0 || !validerDonnees()) {
        QMessageBox::warning(nullptr, "Attention", "ID invalide ou données invalides!");
        return false;
    }

    // Vérifier que le CIN est unique (en excluant le client actuel)
    if (checkIfCinExists(cin_client, id_client)) {
        qDebug() << "Erreur : Ce CIN existe déjà pour un autre client.";
        QMessageBox::warning(nullptr, "Attention", "Ce CIN existe déjà pour un autre client!");
        return false;
    }

    QSqlQuery query;

    // Vérifier dynamiquement la présence des colonnes LATITUDE / LONGITUDE
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    if (hasLatitudeLongitude) {
        query.prepare("UPDATE CLIENT SET "
                      "NOM_CLIENT=:nom_client, PRENOM_CLIENT=:prenom_client, CIN_CLIENT=:cin_client, "
                      "DATE_NAISSANCE=:date_naissance, ADRESSE=:adresse, TEL_CLIENT=:tel_client, COLUMN8=:email, "
                      "LATITUDE=:latitude, LONGITUDE=:longitude "
                      "WHERE ID_CLIENT=:id_client");
    } else {
        qDebug() << "⚠️ Colonnes LATITUDE / LONGITUDE absentes dans CLIENT, mise à jour sans coordonnées GPS.";
        query.prepare("UPDATE CLIENT SET "
                      "NOM_CLIENT=:nom_client, PRENOM_CLIENT=:prenom_client, CIN_CLIENT=:cin_client, "
                      "DATE_NAISSANCE=:date_naissance, ADRESSE=:adresse, TEL_CLIENT=:tel_client, COLUMN8=:email "
                      "WHERE ID_CLIENT=:id_client");
    }

    query.bindValue(":id_client", id_client);
    query.bindValue(":nom_client", nom_client);
    query.bindValue(":prenom_client", prenom_client);
    query.bindValue(":cin_client", cin_client);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel_client", tel_client);
    query.bindValue(":email", email);
    if (hasLatitudeLongitude) {
        if (latitude == 0.0 && longitude == 0.0) {
            query.bindValue(":latitude", QVariant(QVariant::Double));
            query.bindValue(":longitude", QVariant(QVariant::Double));
        } else {
            query.bindValue(":latitude", latitude);
            query.bindValue(":longitude", longitude);
        }
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Client modifié avec succès ✓");
    return true;
}

QSqlQueryModel* client::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();

    // Vérifier dynamiquement si les colonnes LATITUDE / LONGITUDE existent
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    // Récupérer les colonnes dans le bon ordre : ID, Nom, Prénom, CIN, Date, Adresse, Téléphone, Email (+ Latitude/Longitude si disponibles)
    QString queryStr;
    if (hasLatitudeLongitude) {
        queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8, LATITUDE, LONGITUDE FROM CLIENT ORDER BY ID_CLIENT";
    } else {
        queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT ORDER BY ID_CLIENT";
    }

    qDebug() << "Requête d'affichage:" << queryStr;
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage:" << model->lastError().text();
    } else {
        qDebug() << "Nombre de clients affichés:" << model->rowCount();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));
    if (model->columnCount() > 8)
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Latitude"));
    if (model->columnCount() > 9)
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Longitude"));
    return model;
}

QSqlQueryModel* client::afficher(QString orderBy)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();

    // Vérifier dynamiquement si les colonnes LATITUDE / LONGITUDE existent
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    // Récupérer les colonnes dans le bon ordre : ID, Nom, Prénom, CIN, Date, Adresse, Téléphone, Email (+ Latitude/Longitude si disponibles)
    QString queryStr;
    if (hasLatitudeLongitude) {
        queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8, LATITUDE, LONGITUDE FROM CLIENT ORDER BY " + orderBy;
    } else {
        queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT ORDER BY " + orderBy;
    }

    qDebug() << "Requête d'affichage avec tri:" << queryStr;
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage:" << model->lastError().text();
    } else {
        qDebug() << "Nombre de clients affichés:" << model->rowCount();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));
    if (model->columnCount() > 8)
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Latitude"));
    if (model->columnCount() > 9)
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Longitude"));
    return model;
}

bool client::supprimer(int id_client)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id_client");
    query.bindValue(":id_client", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun client trouvé avec cet ID!");
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Client supprimé avec succès ✓");
    return true;
}

bool client::checkIfClientExists(int id_client)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans checkIfClientExists()";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE ID_CLIENT = :id_client");
    query.bindValue(":id_client", id_client);
    return query.exec() && query.next();
}

bool client::checkIfCinExists(QString cin_client, int excludeId)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans checkIfCinExists()";
        return false;
    }

    QSqlQuery query;
    if (excludeId > 0) {
        // Vérifier si le CIN existe en excluant l'ID spécifié (pour la modification)
        query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE CIN_CLIENT = :cin_client AND ID_CLIENT != :exclude_id");
        query.bindValue(":cin_client", cin_client);
        query.bindValue(":exclude_id", excludeId);
    } else {
        // Vérifier si le CIN existe (pour l'ajout)
        query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE CIN_CLIENT = :cin_client");
        query.bindValue(":cin_client", cin_client);
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification du CIN:" << query.lastError().text();
        qDebug() << "Requête SQL:" << query.lastQuery();
        return false;
    }

    if (query.next()) {
        qDebug() << "CIN trouvé avec ID:" << query.value(0).toInt();
        return true; // Le CIN existe déjà
    }
    qDebug() << "CIN non trouvé, peut être ajouté";
    return false; // Le CIN n'existe pas
}

QSqlQueryModel* client::rechercheParNom(QString nom)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans rechercheParNom()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();

    // Vérifier dynamiquement si les colonnes LATITUDE / LONGITUDE existent
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    QSqlQuery query;
    QString sql;
    if (hasLatitudeLongitude) {
        sql = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8, LATITUDE, LONGITUDE FROM CLIENT WHERE NOM_CLIENT LIKE :nom ORDER BY ID_CLIENT";
    } else {
        sql = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT WHERE NOM_CLIENT LIKE :nom ORDER BY ID_CLIENT";
    }

    query.prepare(sql);
    query.bindValue(":nom", "%" + nom + "%");
    if (query.exec()) {
        // Utiliser move() au lieu de copier pour éviter le warning de dépréciation
        model->setQuery(std::move(query));
    } else {
        qDebug() << "Erreur recherche :" << query.lastError().text();
        return model;
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));
    if (model->columnCount() > 8)
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Latitude"));
    if (model->columnCount() > 9)
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Longitude"));

    return model;
}
