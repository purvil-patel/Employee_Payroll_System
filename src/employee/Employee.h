#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <sqlite3.h>
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
};

#endif // EMPLOYEE_H
