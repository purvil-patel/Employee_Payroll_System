#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <sqlite3.h>

class Employee {
private:
    sqlite3 *db;

public:
    Employee(sqlite3 *db);
    void addEmployee();
};

#endif // EMPLOYEE_H
