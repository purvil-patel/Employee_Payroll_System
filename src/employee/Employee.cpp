#include "Employee.h"
#include <iostream>

Employee::Employee(sqlite3 *db) : db(db) {
    char *zErrMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS employee ("
                      "empId TEXT PRIMARY KEY, "
                      "name TEXT NOT NULL, "
                      "dob TEXT NOT NULL, "
                      "doj TEXT NOT NULL, "
                      "mobileNo TEXT, "
                      "state TEXT, "
                      "city TEXT);";
    int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'employee' checked/created successfully.\n";
    }
}

void Employee::addEmployee() {
    std::string empId, name, dob, doj, mobileNo, state, city;
    std::cout << "Enter Employee ID: ";
    std::cin >> empId;
    std::cout << "Enter Name: ";
    std::cin.ignore(); // Ignore the newline character left in the input buffer
    std::getline(std::cin, name);
    std::cout << "Enter Date of Birth (YYYY-MM-DD): ";
    std::cin >> dob;
    std::cout << "Enter Date of Joining (YYYY-MM-DD): ";
    std::cin >> doj;
    std::cout << "Enter Mobile Number: ";
    std::cin >> mobileNo;
    std::cout << "Enter State: ";
    std::cin >> state;
    std::cout << "Enter City: ";
    std::cin >> city;

    char *zErrMsg = 0;
    std::string sql = "INSERT INTO employee (empId, name, dob, doj, mobileNo, state, city) VALUES ('" 
                      + empId + "', '" + name + "', '" + dob + "', '" + doj + "', '" + mobileNo + "', '" 
                      + state + "', '" + city + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Employee added successfully!\n";
    }
}
