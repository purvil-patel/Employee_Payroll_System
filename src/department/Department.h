#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <sqlite3.h>

class Department {
private:
    sqlite3 *db;

public:
    Department(sqlite3 *db);
    void addDepartment();
};

#endif // DEPARTMENT_H
