#include "login.h"
#include "mainwindow.h"
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QApplication>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
 LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Connexion - Driving School");
    setFixedSize(800, 500);
    setModal(true);
    
    // Créer le layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Créer le frame principal avec style
    QFrame *mainFrame = new QFrame();
    mainFrame->setStyleSheet("background-color: rgb(85, 170, 255); border-radius: 10px;");
    mainFrame->setFixedSize(671, 441);
    
    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    
    // Titre
    QLabel *titleLabel = new QLabel("S'identifier:");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(titleLabel);
    
    // Formulaire de connexion
    QFormLayout *formLayout = new QFormLayout();
    
    // Champ nom d'utilisateur
    QLabel *usernameLabel = new QLabel("🔠ID ou Nom:");
    usernameLabel->setStyleSheet("font-size: 12pt; color: white;");
    usernameLineEdit = new QLineEdit();
    usernameLineEdit->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ccc;");
    usernameLineEdit->setPlaceholderText("Entrez votre nom d'utilisateur");
    formLayout->addRow(usernameLabel, usernameLineEdit);
    
    // Champ mot de passe
    QLabel *passwordLabel = new QLabel("🔑Mot de passe:");
    passwordLabel->setStyleSheet("font-size: 12pt; color: white;");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ccc;");
    passwordLineEdit->setPlaceholderText("Entrez votre mot de passe");
    formLayout->addRow(passwordLabel, passwordLineEdit);
    
    frameLayout->addLayout(formLayout);
    
    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    loginButton = new QPushButton("Log in");
    loginButton->setStyleSheet(
        "background-color: #00008B; "
        "color: white; "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-size: 12pt; "
        "font-weight: bold;"
    );
    loginButton->setFixedSize(171, 40);
    
    forgotPasswordButton = new QPushButton("Récupérer mot de passe");
    forgotPasswordButton->setStyleSheet(
        "background-color: #00008B; "
        "color: white; "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-size: 12pt; "
        "font-weight: bold;"
    );
    forgotPasswordButton->setFixedSize(171, 40);
    
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(forgotPasswordButton);
    
    frameLayout->addLayout(buttonLayout);
    
    // Centrer le frame
    mainLayout->addStretch();
    mainLayout->addWidget(mainFrame, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    
    // Connecter les signaux
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginDialog::onForgotPasswordClicked);


    // Permettre la connexion avec Entrée
    connect(usernameLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);

    // Initialize network manager for SMS sending and connect finished handler
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &LoginDialog::replyFinished);
}
void LoginDialog::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Erreur SMS", reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QMessageBox::information(this, "SMS", "SMS envoyé avec succès !");
    reply->deleteLater();
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::onLoginClicked()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text().trimmed();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
        accept(); // Ferme la boîte de dialogue avec succès
    } else {
        QMessageBox::critical(this, "Erreur", "Nom d'utilisateur ou mot de passe incorrect.");
            // SID et auth token de Twilio
            QString sid = "ACec8afb432bfb55c899baa95a7661bdb8";
            QString authToken = "bc79c93cb8403829a486b040a3bd340e";

            // Construire l'URL de l'API Twilio
            QString url = "https://api.twilio.com/2010-04-01/Accounts/" + sid + "/Messages.json";

    // SID et auth token de Twilio
    QString sid = "ACec8afb432bfb55c899baa95a7661bdb8";
    QString authToken = "bc79c93cb8403829a486b040a3bd340e";

    // Construire l'URL de l'API Twilio
    QString url = "https://api.twilio.com/2010-04-01/Accounts/" + sid + "/Messages.json";

    // Créer le gestionnaire de réseau
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);
     connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    // Construire les données de la requête
    QByteArray postData;
    postData.append("To=" + destinataire.toUtf8());
    postData.append("&From=+12084358393");
    postData.append("&Body=" + message.toUtf8());

    // Créer la requête HTTP
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(sid).arg(authToken).toUtf8()).toBase64());


    // Envoyer la requête en utilisant le membre networkAccessManager
    if (networkAccessManager)
        networkAccessManager->post(request, postData);

    // Inform the user that the request was initiated
    QMessageBox::information(this, "Envoi SMS", "La requête d'envoi SMS a été initiée.");

}



bool LoginDialog::sendSMS(const QString &phoneNumber, const QString &password, 
                          const QString &nom, const QString &prenom)
{
    // Prépare un message simple et utilise envoyerSMS qui effectue la requête réseau
    if (phoneNumber.trimmed().isEmpty()) return false;

    QString message = QString("Bonjour %1 %2,\nVotre nouveau mot de passe est : %3\nCordialement.")
                          .arg(nom)
                          .arg(prenom)
                          .arg(password);

    // appeler la méthode qui envoie via Twilio
    envoyerSMS(phoneNumber, message);
    return true;

}

bool LoginDialog::authenticateUser(const QString &username, const QString &password)
{
    // Authenticate against the EMPLOYE table using ID + MOT_DE_PASSEE
    // Expectation: the user will enter the numeric ID in the username field

    // Ensure DB connection is open
    if (!Connection::isDatabaseOpen()) {
        QMessageBox::critical(this, "Erreur de connexion",
                              "La connexion à la base de données n'est pas active.\nVeuillez contacter l'administrateur.");
        return false;
    }

    bool ok;
    int id = username.toInt(&ok);
    if (!ok) {
        // If the user did not enter a numeric ID, reject and ask to use ID
        QMessageBox::warning(this, "Format ID requis", "Veuillez entrer votre ID numérique comme identifiant.");
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE FROM EMPLOYE WHERE ID_EMPLOYE = :id AND MOT_DE_PASSEE = :pwd");
    query.bindValue(":id", id);
    query.bindValue(":pwd", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur DB", "Erreur lors de la vérification des identifiants :\n" + query.lastError().text());
        return false;
    }

    if (query.next()) {
        int foundId = query.value("ID_EMPLOYE").toInt();
        QString nom = query.value("NOM_EMPLOYE").toString();
        QString prenom = query.value("PRENOM").toString();
        QString poste = query.value("POSTE").toString();

        // Store authenticated user info in Connection singleton for later use
        Connection::instance()->setCurrentUser(foundId, poste);

        qDebug() << "[Login] Authenticated:" << foundId << nom << prenom << poste;
        return true;
    }

    return false;
}

void LoginDialog::onForgotPasswordClicked()
{
    bool ok;  // <-- déclaration correcte
    int idEmploye = QInputDialog::getInt(
        this,
        "Récupération de mot de passe",
        "Veuillez entrer l'ID de l'employé :",
        0,      // valeur par défaut
        0,      // min
        100000, // max
        1,      // step
        &ok     // pointeur vers bool
        );

    if (!ok) return;

    if (!Connection::isDatabaseOpen()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base impossible !");
        return;
    }

    // 2) Récupérer le nom et prénom de l'employé par ID
    QSqlQuery query;
    query.prepare("SELECT NOM_EMPLOYE, PRENOM FROM EMPLOYE WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", idEmploye);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Erreur SQL : " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Non trouvé", "Aucun employé trouvé avec cet ID.");
        return;
    }

    QString nom = query.value("NOM_EMPLOYE").toString();
    QString prenom = query.value("PRENOM").toString();

    // 3) Générer nouveau mot de passe
    QString newPassword;
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 8; ++i) {
        newPassword += chars[QRandomGenerator::global()->bounded(chars.length())];
    }

    // 4) Mise à jour du mot de passe dans la base
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE EMPLOYE SET MOT_DE_PASSEE = :password WHERE ID_EMPLOYE = :id");
    updateQuery.bindValue(":password", newPassword);
    updateQuery.bindValue(":id", idEmploye);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec mise à jour : " + updateQuery.lastError().text());
        return;
    }

    // 5) Envoi SMS à TON numéro fixe
    QString fullPhone = "+21695503367";  // <<< TON NUMÉRO FIXE
    QString smsMessage =
        "Bonjour " + nom + " " + prenom +
        ",\nVotre nouveau mot de passe est : " + newPassword +
        "\nMerci.";

    envoyerSMS(fullPhone, smsMessage);  // ou sendSMS(fullPhone, newPassword, nom, prenom);

    // 6) Confirmation
    QMessageBox::information(
        this, "Mot de passe réinitialisé",
        "Le nouveau mot de passe a été généré et envoyé à TON numéro .\n\n"
        "Vérifie ton téléphone."
        );
}

