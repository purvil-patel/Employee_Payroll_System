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

std::pair<bool, std::string> Department::addDepartment(const std::string name) {
    //std::string name;
    std::cout << "Enter Department Name: ";
    //std::cin.ignore();
    //std::getline(std::cin, name);  // Directly read the name

    if (departmentExists(name)) {
        std::cerr << "Error: Department name already exists.\n";
        return {false ,"Error: Department name already exists."};
    }

    char *zErrMsg = 0;
    std::string sql = "INSERT INTO department (name) VALUES (?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Department added successfully!\n";
            return {true,name + " Department added successfully!\n"};
        } else {
            std::cerr << "SQL error in addDepartment: " << sqlite3_errmsg(db) << std::endl;
            return {false,"Error in adding new department."};
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL error in addDepartment: " << sqlite3_errmsg(db) << std::endl;
        return {false,"Error in adding new department."};
    }
}

std::vector<std::string> Department::getAllDepartmentNames() const {
    std::vector<std::string> departmentNames;
    char *zErrMsg = 0;
    std::string sql = "SELECT name FROM department;";
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        std::vector<std::string>* names = static_cast<std::vector<std::string>*>(data);
        if (argc == 1 && argv[0] && argv[0][0] != '\0') {
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
        std::cout << "Departments Fetched\n";
        // for (const auto& name : departmentNames) {
        //     std::cout << name << std::endl;
        // }
    }
    
    return departmentNames;
    
}

bool Department::departmentExists(const std::string& name) {
    std::string sql = "SELECT COUNT(name) FROM department WHERE name = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            return true;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL error in departmentExists: " << sqlite3_errmsg(db) << std::endl;
    }
    return false;
}
