#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <sqlite3.h>
#include <map>
#include "../department/Department.h"
#include "../pay_grade/PayGrade.h"

class Employee {
private:
    sqlite3 *db;
    Department *dept;
    PayGrade *payGrade;

public:
    Employee(sqlite3 *db);
    Employee(sqlite3 *db, Department *dept, PayGrade *payGrade);
    bool addEmployee(const std::string& name, const std::string& dob, const std::string& doj, const std::string& mobileNo, const std::string& state, const std::string& city, const std::string& department, const std::string& gradeName);
    std::map<std::string, std::string> getEmployee(const std::string& name);
    std::map<std::string, std::string> getEmployeeById(const std::string& empId);
    void handleEmployeeQuery(); 
    bool deleteEmployee(const std::string& name);
    void handleDeleteEmployee();
    bool updateEmployee(const std::string& empId, const std::string& name, const std::string& dob, const std::string& doj, const std::string& mobileNo, const std::string& state, const std::string& city, const std::string& department, const std::string& gradeName);
    void handleUpdateEmployee();
    void handleAddEmployee();
    std::vector<std::map<std::string, std::string>> getAllEmployees();
};

#endif // EMPLOYEE_H
