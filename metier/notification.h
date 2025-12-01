#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

// Classe métier Notification pour le CRUD en base de données
class Notification
{
public:
    Notification();
    Notification(const QString &type, const QString &message, const QString &entite = QString());

    // CRUD
    bool ajouter();
    static bool supprimer(int id);
    static bool supprimerTout();
    static QSqlQueryModel* afficher();
    static int countNonLues();

    // Méthodes statiques pour ajouter des notifications automatiques
    static void notifierAjoutEmploye(const QString &nom, const QString &prenom);
    static void notifierModificationEmploye(const QString &nom, const QString &prenom);
    static void notifierSuppressionEmploye(const QString &nom, const QString &prenom);

    static void notifierAjoutMoniteur(const QString &nom, const QString &prenom);
    static void notifierModificationMoniteur(const QString &nom, const QString &prenom);
    static void notifierSuppressionMoniteur(const QString &nom, const QString &prenom);

    static void notifierAjoutClient(const QString &nom, const QString &prenom);
    static void notifierModificationClient(const QString &nom, const QString &prenom);
    static void notifierSuppressionClient(const QString &nom, const QString &prenom);

    static void notifierAjoutVehicule(const QString &immatriculation);
    static void notifierModificationVehicule(const QString &immatriculation);
    static void notifierSuppressionVehicule(const QString &immatriculation);

    static void notifierAjoutSeance(const QString &info);
    static void notifierAjoutCircuit(const QString &nom);

private:
    int id_notification;
    QString type;           // "AJOUT", "MODIFICATION", "SUPPRESSION"
    QString message;
    QString entite;         // "EMPLOYE", "MONITEUR", "CLIENT", "VEHICULE", etc.
    QDateTime dateHeure;
    bool lue;
};

// Dialog pour afficher les notifications
class NotificationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotificationDialog(QWidget *parent = nullptr);
    ~NotificationDialog();

    void refreshNotifications();

private slots:
    void onClearAllClicked();
    void onDeleteSelectedClicked();
    void onRefreshClicked();

private:
    QTableView *tableView;
    QPushButton *btnClearAll;
    QPushButton *btnDeleteSelected;
    QPushButton *btnRefresh;
    QPushButton *btnClose;
    QLabel *lblCount;
    QSqlQueryModel *model;
};

#endif // NOTIFICATION_H
