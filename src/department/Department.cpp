#include "Department.h"
#include <iostream>

Department::Department(sqlite3 *db) : db(db) {
    char *zErrMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS department ("
                      "deptId INTEGER PRIMARY KEY AUTOINCREMENT, " 
                      "name TEXT NOT NULL);";
    int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'department' checked/created successfully.\n";
    }
}

void Department::addDepartment() {
    std::string name;
    std::cout << "Enter Department Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);  // Directly read the name

    char *zErrMsg = 0;
    std::string sql = "INSERT INTO department (name) VALUES ('" + name + "');";  // Removed deptId from INSERT
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Department added successfully!\n";
    }
}

std::vector<std::string> Department::getAllDepartmentNames() const {
    std::vector<std::string> departmentNames;
    char *zErrMsg = 0;
    std::string sql = "SELECT name FROM department;";
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        std::vector<std::string>* names = static_cast<std::vector<std::string>*>(data);
        if (argc == 1 && argv[0]) {
            names->push_back(argv[0]);
        }
        return 0;
    };

    int rc = sqlite3_exec(db, sql.c_str(), callback, &departmentNames, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "Departments List:\n";
        for (const auto& name : departmentNames) {
            std::cout << name << std::endl;
        }
    }
    
    return departmentNames;
    
}

