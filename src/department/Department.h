#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <vector>
#include <string>
#include <sqlite3.h>

class Department {
private:
    sqlite3 *db;

public:
    Department(sqlite3 *db);
    void addDepartment();
    std::vector<std::string> getAllDepartmentNames() const;
    bool departmentExists(const std::string& name);
};

#endif // DEPARTMENT_H
