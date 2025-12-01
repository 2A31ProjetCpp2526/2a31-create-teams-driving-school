#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

class QNetworkAccessManager;
class QNetworkReply;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString role() const;

private slots:
    void onLoginClicked();
    void onForgotPasswordClicked();
    void envoyerSMS(const QString &destinataire, const QString &message);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *forgotPasswordButton;
    QPushButton *bypassButton;
    QString m_role;
    QNetworkAccessManager *networkAccessManager = nullptr;

    bool authenticateUser(const QString &username, const QString &password);
    bool sendSMS(const QString &phoneNumber, const QString &password, 
                 const QString &nom, const QString &prenom);

};

#endif // LOGIN_H
