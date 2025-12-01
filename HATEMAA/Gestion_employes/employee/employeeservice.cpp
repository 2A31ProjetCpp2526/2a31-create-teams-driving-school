#include "employeeservice.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

EmployeeService::EmployeeService(QObject *parent)
    : QObject(parent)
{
    loadSampleData();
}

bool EmployeeService::addEmployee(const Employee &employee)
{
    if (!employee.isValid()) {
        qWarning() << "Tentative d'ajout d'un employé invalide";
        return false;
    }

    // Vérifier si l'ID existe déjà
    for (const Employee &emp : m_employees) {
        if (emp.getId() == employee.getId()) {
            qWarning() << "Un employé avec cet ID existe déjà:" << employee.getId();
            return false;
        }
    }

    m_employees.append(employee);
    emit employeeAdded(employee);
    emit dataChanged();
    return true;
}

bool EmployeeService::removeEmployee(const QString &id)
{
    for (int i = 0; i < m_employees.size(); ++i) {
        if (m_employees[i].getId() == id) {
            m_employees.removeAt(i);
            emit employeeRemoved(id);
            emit dataChanged();
            return true;
        }
    }
    return false;
}

bool EmployeeService::updateEmployee(const Employee &employee)
{
    for (int i = 0; i < m_employees.size(); ++i) {
        if (m_employees[i].getId() == employee.getId()) {
            m_employees[i] = employee;
            emit employeeUpdated(employee);
            emit dataChanged();
            return true;
        }
    }
    return false;
}

Employee EmployeeService::getEmployee(const QString &id) const
{
    for (const Employee &employee : m_employees) {
        if (employee.getId() == id) {
            return employee;
        }
    }
    return Employee(); // Retourne un employé vide si non trouvé
}

QList<Employee> EmployeeService::getAllEmployees() const
{
    return m_employees;
}

QList<Employee> EmployeeService::searchEmployee(const QString &searchTerm) const
{
    QList<Employee> results;
    QString searchLower = searchTerm.toLower();

    for (const Employee &employee : m_employees) {
        if (employee.getId().toLower().contains(searchLower) ||
            employee.getNom().toLower().contains(searchLower) ||
            employee.getPrenom().toLower().contains(searchLower) ||
            employee.getPoste().toLower().contains(searchLower)) {
            results.append(employee);
        }
    }
    return results;
}

QList<Employee> EmployeeService::sortEmployees(const QString &sortBy) const
{
    QList<Employee> sortedEmployees = m_employees;

    if (sortBy == "ID") {
        std::sort(sortedEmployees.begin(), sortedEmployees.end(),
                  [](const Employee &a, const Employee &b) {
                      return a.getId() < b.getId();
                  });
    } else if (sortBy == "Poste") {
        std::sort(sortedEmployees.begin(), sortedEmployees.end(),
                  [](const Employee &a, const Employee &b) {
                      return a.getPoste() < b.getPoste();
                  });
    } else if (sortBy == "Nom") {
        std::sort(sortedEmployees.begin(), sortedEmployees.end(),
                  [](const Employee &a, const Employee &b) {
                      return a.getNom() < b.getNom();
                  });
    }

    return sortedEmployees;
}

int EmployeeService::getTotalEmployees() const
{
    return m_employees.size();
}

QStringList EmployeeService::getUniquePostes() const
{
    QStringList postes;
    for (const Employee &employee : m_employees) {
        if (!postes.contains(employee.getPoste())) {
            postes.append(employee.getPoste());
        }
    }
    return postes;
}

int EmployeeService::getEmployeesByPoste(const QString &poste) const
{
    int count = 0;
    for (const Employee &employee : m_employees) {
        if (employee.getPoste() == poste) {
            count++;
        }
    }
    return count;
}

QString EmployeeService::exportToCSV() const
{
    QString csv = "ID,Nom,Prenom,Poste,Mot de passe,Telephone,Date d'embauche\n";
    
    for (const Employee &employee : m_employees) {
        csv += QString("%1,%2,%3,%4,%5,%6,%7\n")
               .arg(employee.getId())
               .arg(employee.getNom())
               .arg(employee.getPrenom())
               .arg(employee.getPoste())
               .arg(employee.getMotDePasse())
               .arg(employee.getTelephone())
               .arg(employee.getDateEmbauche().toString("yyyy-MM-dd"));
    }
    
    return csv;
}

QString EmployeeService::exportToJSON() const
{
    QJsonArray jsonArray;
    
    for (const Employee &employee : m_employees) {
        QJsonObject jsonObj;
        jsonObj["id"] = employee.getId();
        jsonObj["nom"] = employee.getNom();
        jsonObj["prenom"] = employee.getPrenom();
        jsonObj["poste"] = employee.getPoste();
        jsonObj["motDePasse"] = employee.getMotDePasse();
        jsonObj["telephone"] = employee.getTelephone();
        jsonObj["dateEmbauche"] = employee.getDateEmbauche().toString("yyyy-MM-dd");
        jsonArray.append(jsonObj);
    }
    
    QJsonDocument doc(jsonArray);
    return doc.toJson(QJsonDocument::Indented);
}

void EmployeeService::loadSampleData()
{
    // Ajouter quelques employés d'exemple
    addEmployee(Employee("EMP001", "Dupont", "Jean", "Développeur", "password123", "+21612345678", QDate(2023, 1, 15)));
    addEmployee(Employee("EMP002", "Martin", "Marie", "Designer", "password456", "+21687654321", QDate(2023, 2, 20)));
    addEmployee(Employee("EMP003", "Bernard", "Pierre", "Manager", "password789", "+21611111111", QDate(2023, 3, 10)));
}
