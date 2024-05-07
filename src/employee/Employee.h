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
    Employee(sqlite3 *db, Department *dept, PayGrade *payGrade);
    void addEmployee();
    std::map<std::string, std::string> getEmployee(const std::string& name);
    void handleEmployeeQuery(); 
    void deleteEmployee(const std::string& name);
    void handleDeleteEmployee();
    void updateEmployee(const std::string& empId, const std::string& name, const std::string& dob, const std::string& doj, const std::string& mobileNo, const std::string& state, const std::string& city, const std::string& department, const std::string& gradeName);
    void handleUpdateEmployee();
};

#endif // EMPLOYEE_H
