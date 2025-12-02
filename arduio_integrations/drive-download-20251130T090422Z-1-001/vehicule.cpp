#include "vehicule.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QTableView>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QPieSlice>
#include <QBrush>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QTextEdit>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QScrollArea>

Vehicule::Vehicule(){}

Vehicule::Vehicule(QString imm, QString mod, QString typ, QString trans, QString et, QDate date)
{
    this->immatricule = imm.toUpper().trimmed();
    this->modele = mod.trimmed();
    this->type_vehicule = typ;
    this->transmission = trans;
    this->etat = et;
    this->date_assurence = date;
}

bool Vehicule::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    QSqlQuery query;

    // Vérifier si le véhicule existe déjà
    query.prepare("SELECT COUNT(*) FROM VEHICULE WHERE IMMATRICULE = :immat");
    query.bindValue(":immat", immatricule);
    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Un véhicule avec cette immatriculation existe déjà!");
        return false;
    }

    // Vérifier que tous les champs sont remplis
    if (immatricule.isEmpty() || modele.isEmpty() || type_vehicule.isEmpty() ||
        transmission.isEmpty() || etat.isEmpty() || !date_assurence.isValid()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // INSERTION avec les bons noms de colonnes
    query.prepare("INSERT INTO VEHICULE (IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, DATE_ASSURENCE) "
                  "VALUES (:immat, :modele, :type, :transmission, :etat, TO_DATE(:date_assurence, 'DD/MM/YYYY'))");

    query.bindValue(":immat", immatricule);
    query.bindValue(":modele", modele);
    query.bindValue(":type", type_vehicule);
    query.bindValue(":transmission", transmission);
    query.bindValue(":etat", etat);
    query.bindValue(":date_assurence", date_assurence.toString("dd/MM/yyyy"));

    qDebug() << "🔧 Tentative d'ajout véhicule - Immat:" << immatricule;
    qDebug() << "📅 Date envoyée:" << date_assurence.toString("dd/MM/yyyy");

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout véhicule:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout:\n" + query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule ajouté avec succès ✓");
    return true;
}

bool Vehicule::supprimer(QString id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE IMMATRICULE = :id");
    query.bindValue(":id", id.toUpper());

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun véhicule trouvé avec cette immatriculation!");
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule supprimé avec succès ✓");
    return true;
}

bool Vehicule::modifier(QString id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifier()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    QSqlQuery query;

    // Vérifier que tous les champs sont remplis
    if (modele.isEmpty() || type_vehicule.isEmpty() || transmission.isEmpty() ||
        etat.isEmpty() || !date_assurence.isValid()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // MODIFICATION avec les bons noms de colonnes
    query.prepare("UPDATE VEHICULE "
                  "SET MODELE = :mod, "
                  "TYPE_VEHICULE = :typ, "
                  "TRANSMISSION = :trans, "
                  "ETAT = :et, "
                  "DATE_ASSURENCE = TO_DATE(:date, 'DD/MM/YYYY') "
                  "WHERE IMMATRICULE = :id");

    query.bindValue(":mod", modele);
    query.bindValue(":typ", type_vehicule);
    query.bindValue(":trans", transmission);
    query.bindValue(":et", etat);
    query.bindValue(":date", date_assurence.toString("dd/MM/yyyy"));
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun véhicule trouvé avec cette immatriculation!");
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule modifié avec succès ✓");
    return true;
}

QSqlQueryModel* Vehicule::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();

    // AFFICHAGE avec les bons noms de colonnes
    QString queryStr = "SELECT IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, "
                       "TO_CHAR(DATE_ASSURENCE, 'DD/MM/YYYY') AS DATE_ASSURENCE "
                       "FROM VEHICULE ORDER BY IMMATRICULE";

    model->setQuery(queryStr);

    // Définir les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Transmission"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Assurance"));

    return model;
}

QSqlQueryModel* Vehicule::rechercher(const QString &debut)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM VEHICULE WHERE IMMATRICULE LIKE :immat");
    query.bindValue(":immat", debut + "%");    // 🔥 Commence par

    query.exec();
    model->setQuery(query);

    return model;
}
QSqlQueryModel* Vehicule::trier(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString query = "SELECT * FROM VEHICULE ORDER BY " + critere;

    model->setQuery(query);

    // Définir les en-têtes pour un affichage clair
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Transmission"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Assurance"));

    return model;
}

bool Vehicule::exportV(const QString& filePath, QTableView* tableView)
{
    if (!tableView) return false;

    QPixmap pixmap(tableView->size());
    tableView->render(&pixmap);

    if (pixmap.save(filePath)) {
        qDebug() << "✅ Image exportée avec succès dans :" << filePath;
        return true;
    } else {
        qDebug() << "❌ Erreur : impossible d’enregistrer l’image.";
        return false;
    }
}


QChartView* Vehicule::statistiquesV()
{
    int voiture = 0, camion = 0, moto = 0;
    int automatique = 0, manuelle = 0;

    // 🔹 Lecture du type
    QSqlQuery queryType;
    if (queryType.exec("SELECT type_vehicule FROM VEHICULE")) {
        while (queryType.next()) {
            QString type = queryType.value(0).toString().toLower();
            if (type.contains("voiture")) voiture++;
            else if (type.contains("camion")) camion++;
            else if (type.contains("moto")) moto++;
        }
    }

    // 🔹 Lecture de la transmission
    QSqlQuery queryTrans;
    if (queryTrans.exec("SELECT transmission FROM VEHICULE")) {
        while (queryTrans.next()) {
            QString trans = queryTrans.value(0).toString().toLower();
            if (trans.contains("auto")) automatique++;
            else manuelle++;
        }
    }

    int totalType = voiture + camion + moto;
    int totalTrans = automatique + manuelle;

    // 🥧 Série Type
    QPieSeries *seriesType = new QPieSeries();
    if (totalType > 0) {
        seriesType->append("🚗 Voiture", voiture);
        seriesType->append("🚛 Camion", camion);
        seriesType->append("🏍️ Moto", moto);
    } else {
        seriesType->append("Aucune donnée", 1);
    }

    QStringList couleursType = {"#2986cc", "#6aa84f", "#f6b26b"};
    for (int i = 0; i < seriesType->slices().size() && i < couleursType.size(); ++i) {
        QPieSlice *slice = seriesType->slices().at(i);
        slice->setBrush(QColor(couleursType[i]));
        slice->setLabel(QString("%1").arg(slice->label()));
        slice->setLabelFont(QFont("Arial", 10, QFont::Bold));
        slice->setLabelColor(Qt::black);
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelArmLengthFactor(0.25); // 🔹 flèche plus jolie
    }

    // 🥧 Série Transmission
    QPieSeries *seriesTrans = new QPieSeries();
    if (totalTrans > 0) {
        seriesTrans->append("⚙️ Auto", automatique);
        seriesTrans->append("🧰 Manu", manuelle);
    } else {
        seriesTrans->append("Aucune donnée", 1);
    }

    QStringList couleursTrans = {"#e06666", "#ffd966"};
    for (int i = 0; i < seriesTrans->slices().size() && i < couleursTrans.size(); ++i) {
        QPieSlice *slice = seriesTrans->slices().at(i);
        slice->setBrush(QColor(couleursTrans[i]));
        slice->setLabel(QString("%1").arg(slice->label()));
        slice->setLabelFont(QFont("Arial", 10, QFont::Bold));
        slice->setLabelColor(Qt::black);
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelArmLengthFactor(0.25);
    }

    // 📊 Chart Type
    QChart *chartType = new QChart();
    chartType->addSeries(seriesType);
    chartType->setTitle("🚘 Répartition par Type de Véhicule");
    chartType->setAnimationOptions(QChart::AllAnimations);
    chartType->setTitleFont(QFont("Arial", 13, QFont::Bold));
    chartType->legend()->setFont(QFont("Arial", 11));
    chartType->legend()->setAlignment(Qt::AlignBottom);
    chartType->legend()->setMarkerShape(QLegend::MarkerShapeRectangle); // ✅ Carrés colorés

    // 📊 Chart Transmission
    QChart *chartTrans = new QChart();
    chartTrans->addSeries(seriesTrans);
    chartTrans->setTitle("⚙️ Répartition par Transmission");
    chartTrans->setAnimationOptions(QChart::AllAnimations);

    chartTrans->setTitleFont(QFont("Arial", 13, QFont::Bold));
    chartTrans->legend()->setFont(QFont("Arial", 11));
    chartTrans->legend()->setAlignment(Qt::AlignBottom);
    chartTrans->legend()->setMarkerShape(QLegend::MarkerShapeRectangle);

    // 🧾 Calcul pourcentages + labels sous les cercles
    QLabel *labelType = new QLabel(QString("🚗 Voiture : %1% | 🚛 Camion : %2% | 🏍️ Moto : %3%")
                                       .arg(QString::number((voiture * 100.0 / totalType), 'f', 1))
                                       .arg(QString::number((camion * 100.0 / totalType), 'f', 1))
                                       .arg(QString::number((moto * 100.0 / totalType), 'f', 1)));
    labelType->setAlignment(Qt::AlignCenter);
    labelType->setFont(QFont("Arial", 11, QFont::Bold));

    QLabel *labelTrans = new QLabel(QString("⚙️ Automatique : %1% | 🧰 Manuelle : %2%")
                                        .arg(QString::number((automatique * 100.0 / totalTrans), 'f', 1))
                                        .arg(QString::number((manuelle * 100.0 / totalTrans), 'f', 1)));
    labelTrans->setAlignment(Qt::AlignCenter);
    labelTrans->setFont(QFont("Arial", 11, QFont::Bold));

    // 🧩 Layout
    QWidget *container = new QWidget();
    QVBoxLayout *layoutType = new QVBoxLayout();
    QVBoxLayout *layoutTrans = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout(container);

    QChartView *chartViewType = new QChartView(chartType);
    QChartView *chartViewTrans = new QChartView(chartTrans);
    chartViewType->setRenderHint(QPainter::Antialiasing);
    chartViewTrans->setRenderHint(QPainter::Antialiasing);

    layoutType->addWidget(chartViewType);
    layoutType->addWidget(labelType);

    layoutTrans->addWidget(chartViewTrans);
    layoutTrans->addWidget(labelTrans);

    mainLayout->addLayout(layoutType);
    mainLayout->addLayout(layoutTrans);

    QChartView *chartView = new QChartView();
    chartView->setLayout(mainLayout);
    chartView->resize(1200, 700);

    return chartView;

}

QWidget* Vehicule::afficherHistorique(const QString &immat)
{
    QWidget *w = new QWidget;
    w->setWindowTitle("📘 Historique du véhicule : " + immat);
    w->resize(700, 600);

    QVBoxLayout *layout = new QVBoxLayout(w);
    QTextEdit *txt = new QTextEdit;
    txt->setReadOnly(true);

    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                       + "/historique_vehicules.txt";

    QFile file(filePath);
    QString html = "<div style='font-family:Segoe UI; font-size:14px;'>"
                   "<h2>📘 Historique du véhicule <b>" + immat + "</b></h2><hr>";

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        html += "<p><i>Impossible d’ouvrir le fichier.</i></p></div>";
        txt->setHtml(html);
        layout->addWidget(txt);
        return w;
    }

    QTextStream in(&file);
    bool found = false;

    while (!in.atEnd()) {

        QString line = in.readLine().trimmed();

        // Cherche exactement la ligne Immatricule
        if (line == "Immatricule: " + immat)
        {
            found = true;

            // Lire lignes suivantes en sécurité
            QString actionLine = in.readLine().trimmed();
            QString dateLine   = in.readLine().trimmed();
            in.readLine(); // skip ----

            // Vérifier que les lignes sont valides
            if (!actionLine.startsWith("Action:"))
                continue;
            if (!dateLine.startsWith("Date:"))
                continue;

            QString action = actionLine.mid(QString("Action: ").length());
            QString date   = dateLine.mid(QString("Date: ").length());

            html += "<p style='margin-bottom:10px;'>"
                    "<b>🔹 Action :</b> " + action + "<br>"
                               "<b>⏱️ Date :</b> " + date +
                    "</p><hr>";
        }
    }

    if (!found)
        html += "<p><i>Aucune action pour ce véhicule.</i></p>";

    html += "</div>";
    txt->setHtml(html);
    layout->addWidget(txt);

    return w;
}
