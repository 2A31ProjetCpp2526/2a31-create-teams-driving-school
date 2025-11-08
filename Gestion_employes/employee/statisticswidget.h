#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QProgressBar>
#include <QFrame>
#include "employeeservice.h"

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(EmployeeService *service, QWidget *parent = nullptr);

private slots:
    void refreshStatistics();
    void onDataChanged();

private:
    void setupUI();
    void connectSignals();
    void updateStatistics();

    EmployeeService *m_service;
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QGridLayout *m_statsLayout;
    
    QLabel *m_titleLabel;
    QLabel *m_totalEmployeesLabel;
    QLabel *m_totalEmployeesValue;
    
    // Statistiques par poste
    QFrame *m_posteFrame;
    QVBoxLayout *m_posteLayout;
    QLabel *m_posteTitleLabel;
    
    // Progress bars pour chaque poste
    QList<QProgressBar*> m_posteProgressBars;
    QList<QLabel*> m_posteLabels;
};

#endif // STATISTICSWIDGET_H
