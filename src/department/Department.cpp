#include "Department.h"
#include <iostream>

Department::Department(sqlite3 *db) : db(db) {

    char *zErrMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS department ("
                      "deptId TEXT PRIMARY KEY, "
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
    std::string deptId, name;
    std::cout << "Enter Department ID: ";
    std::cin >> deptId;
    std::cout << "Enter Department Name: ";
    std::cin.ignore(); // Ignore the newline character left in the input buffer
    std::getline(std::cin, name);

    char *zErrMsg = 0;
    std::string sql = "INSERT INTO department (deptId, name) VALUES ('" + deptId + "', '" + name + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Department added successfully!\n";
    }
}
