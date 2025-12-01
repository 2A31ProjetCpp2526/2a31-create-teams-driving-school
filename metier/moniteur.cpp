#include "moniteur.h"
#include "connection.h"
#include "notification.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Moniteur::Moniteur(){}

Moniteur::Moniteur(QString id, QString cin, QString nom, QString prenom, QString tel, QString mail, QString perm)
{
    this->id_moniteur = id.trimmed();
    this->cin_moniteur = cin.toUpper().trimmed();
    this->nom_moniteur = nom.trimmed();
    this->prenom_moniteur = prenom.trimmed();
    this->tel_moniteur = tel.trimmed();
    this->email = mail.trimmed();
    this->permis = perm.trimmed();
}

bool Moniteur::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }
    
    QSqlQuery query;

    // Vérifier que tous les champs sont remplis (SAUF ID qui sera auto-généré)
    if (cin_moniteur.isEmpty() || nom_moniteur.isEmpty() || prenom_moniteur.isEmpty() ||
        tel_moniteur.isEmpty() || email.isEmpty() || permis.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // Vérifier si le CIN existe déjà (CIN doit être unique)
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE CIN_MONITEUR = :cin");
    checkQuery.bindValue(":cin", cin_moniteur);
    
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Un moniteur avec ce CIN existe déjà!");
        qDebug() << "❌ CIN déjà existant:" << cin_moniteur;
        return false;
    }

    // Vérifier si l'email existe déjà (EMAIL doit être unique)
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE EMAIL = :email");
    checkQuery.bindValue(":email", email);
    
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Un moniteur avec cet EMAIL existe déjà!");
        qDebug() << "❌ EMAIL déjà existant:" << email;
        return false;
    }

    // INSERTION avec séquence pour l'ID
    query.prepare("INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS) "
                  "VALUES (SEQ_MONITEUR.NEXTVAL, :cin, :nom, :prenom, :tel, :email, :permis)");

    query.bindValue(":cin", cin_moniteur);
    query.bindValue(":nom", nom_moniteur);
    query.bindValue(":prenom", prenom_moniteur);
    query.bindValue(":tel", tel_moniteur);
    query.bindValue(":email", email);
    query.bindValue(":permis", permis);

    qDebug() << "🔧 Tentative d'ajout moniteur - CIN:" << cin_moniteur << ", Email:" << email;

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout moniteur:" << query.lastError().text();
        QString errorMsg = query.lastError().text();
        
        if (errorMsg.contains("unique constraint")) {
            QMessageBox::critical(nullptr, "Erreur", "Violation de contrainte d'unicité.\nVérifiez que le CIN et l'EMAIL ne sont pas déjà utilisés!");
        } else {
            QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout:\n" + errorMsg);
        }
        return false;
    }

    // Ajouter notification automatique
    Notification::notifierAjoutMoniteur(nom_moniteur, prenom_moniteur);

    QMessageBox::information(nullptr, "Succès", "Moniteur ajouté avec succès ✓");
    return true;
}

bool Moniteur::supprimer(QString id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM MONITEUR WHERE ID_MONITEUR = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun moniteur trouvé avec cet ID!");
        return false;
    }

    // Ajouter notification automatique
    Notification::notifierSuppressionMoniteur("ID:" + id, "");

    QMessageBox::information(nullptr, "Succès", "Moniteur supprimé avec succès ✓");
    return true;
}

bool Moniteur::modifier(QString id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifier()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }
    
    QSqlQuery query;

    // Vérifier que tous les champs sont remplis
    if (cin_moniteur.isEmpty() || nom_moniteur.isEmpty() || prenom_moniteur.isEmpty() ||
        tel_moniteur.isEmpty() || email.isEmpty() || permis.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // Vérifier que le CIN n'existe pas déjà (pour un autre moniteur)
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE CIN_MONITEUR = :cin AND ID_MONITEUR != :id");
    checkQuery.bindValue(":cin", cin_moniteur);
    checkQuery.bindValue(":id", id);
    
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Ce CIN est déjà utilisé par un autre moniteur!");
        qDebug() << "❌ CIN déjà utilisé:" << cin_moniteur;
        return false;
    }

    // Vérifier que l'EMAIL n'existe pas déjà (pour un autre moniteur)
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE EMAIL = :email AND ID_MONITEUR != :id");
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":id", id);
    
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Cet EMAIL est déjà utilisé par un autre moniteur!");
        qDebug() << "❌ EMAIL déjà utilisé:" << email;
        return false;
    }

    // MODIFICATION avec les bons noms de colonnes
    query.prepare("UPDATE MONITEUR "
                  "SET CIN_MONITEUR = :cin, "
                  "NOM_MONITEUR = :nom, "
                  "PRENOM_MONITEUR = :prenom, "
                  "TEL_MONITEUR = :tel, "
                  "EMAIL = :email, "
                  "PERMIS = :permis "
                  "WHERE ID_MONITEUR = :id");

    query.bindValue(":cin", cin_moniteur);
    query.bindValue(":nom", nom_moniteur);
    query.bindValue(":prenom", prenom_moniteur);
    query.bindValue(":tel", tel_moniteur);
    query.bindValue(":email", email);
    query.bindValue(":permis", permis);
    query.bindValue(":id", id);

    qDebug() << "🔧 Tentative de modification moniteur - ID:" << id << ", CIN:" << cin_moniteur;

    if (!query.exec()) {
        qDebug() << "Erreur SQL modification:" << query.lastError().text();
        QString errorMsg = query.lastError().text();
        
        if (errorMsg.contains("unique constraint")) {
            QMessageBox::critical(nullptr, "Erreur", "Violation de contrainte d'unicité.\nVérifiez que le CIN et l'EMAIL ne sont pas déjà utilisés!");
        } else {
            QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + errorMsg);
        }
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun moniteur trouvé avec cet ID!");
        return false;
    }

    // Ajouter notification automatique
    Notification::notifierModificationMoniteur(nom_moniteur, prenom_moniteur);

    QMessageBox::information(nullptr, "Succès", "Moniteur modifié avec succès ✓");
    return true;
}

QSqlQueryModel* Moniteur::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }
    
    QSqlQueryModel* model = new QSqlQueryModel();

    // AFFICHAGE avec les bons noms de colonnes
    QString queryStr = "SELECT ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, "
                       "TEL_MONITEUR, EMAIL, PERMIS "
                       "FROM MONITEUR ORDER BY ID_MONITEUR";

    model->setQuery(queryStr);

    // Définir les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Permis"));

    return model;
}

QSqlQueryModel* Moniteur::rechercher(const QString &debut)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Recherche contenant dans nom, prenom, CIN ou email
    query.prepare("SELECT ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS "
                  "FROM MONITEUR WHERE UPPER(NOM_MONITEUR) LIKE :pattern OR UPPER(PRENOM_MONITEUR) LIKE :pattern "
                  "OR CIN_MONITEUR LIKE :patternSimple OR EMAIL LIKE :patternSimple ORDER BY NOM_MONITEUR");

    QString pattern = "%" + debut.trimmed().toUpper() + "%";
    QString patternSimple = "%" + debut.trimmed() + "%";
    query.bindValue(":pattern", pattern);
    query.bindValue(":patternSimple", patternSimple);

    query.exec();
    model->setQuery(query);

    // Définir en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Permis"));

    return model;
}
