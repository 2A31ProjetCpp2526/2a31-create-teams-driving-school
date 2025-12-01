#ifndef MONITEURSTAT_H
#define MONITEURSTAT_H

#include <QDialog>
#include <QMap>
#include <QString>

class QChartView;
class QPieSeries;

// Dialog pour afficher les statistiques des moniteurs par type de permis
class MoniteurStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoniteurStatDialog(QWidget *parent = nullptr);
    ~MoniteurStatDialog();

    void loadStatistics();

private:
    QChartView *chartView;
    
    QMap<QString, int> getPermisStatistics();
};

#endif // MONITEURSTAT_H
