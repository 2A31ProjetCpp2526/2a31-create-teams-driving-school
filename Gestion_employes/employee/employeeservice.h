#ifndef EMPLOYEESERVICE_H
#define EMPLOYEESERVICE_H

#include "employee.h"
#include <QObject>
#include <QList>
#include <QStringList>

class EmployeeService : public QObject
{
    Q_OBJECT

public:
    explicit EmployeeService(QObject *parent = nullptr);

    // Opérations CRUD
    bool addEmployee(const Employee &employee);
    bool removeEmployee(const QString &id);
    bool updateEmployee(const Employee &employee);
    Employee getEmployee(const QString &id) const;
    QList<Employee> getAllEmployees() const;

    // Opérations de recherche et tri
    QList<Employee> searchEmployee(const QString &searchTerm) const;
    QList<Employee> sortEmployees(const QString &sortBy) const;

    // Statistiques
    int getTotalEmployees() const;
    QStringList getUniquePostes() const;
    int getEmployeesByPoste(const QString &poste) const;

    // Export
    QString exportToCSV() const;
    QString exportToJSON() const;

signals:
    void employeeAdded(const Employee &employee);
    void employeeRemoved(const QString &id);
    void employeeUpdated(const Employee &employee);
    void dataChanged();

private:
    QList<Employee> m_employees;
    void loadSampleData(); // Pour les tests
};

#endif // EMPLOYEESERVICE_H
