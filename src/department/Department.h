#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <vector>
#include <string>
#include <sqlite3.h>
#include <utility>

class Department {
private:
    sqlite3 *db;

public:
    Department(sqlite3 *db);
    std::pair<bool, std::string> addDepartment(const std::string name);
    std::vector<std::string> getAllDepartmentNames() const;
    bool departmentExists(const std::string& name);
};

#endif // DEPARTMENT_H
