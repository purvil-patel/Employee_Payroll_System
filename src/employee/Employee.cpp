#include "Employee.h"
#include <iostream>
#include <vector>
#include <limits>
#include <map>
#include <algorithm>
#include <cctype>

Employee::Employee(sqlite3 *db, Department *dept, PayGrade *payGrade) : db(db), dept(dept), payGrade(payGrade) {
    char *zErrMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS employee ("
                      "empId INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "dob TEXT NOT NULL, "
                      "doj TEXT NOT NULL, "
                      "mobileNo TEXT, "
                      "state TEXT, "
                      "city TEXT, "
                      "department TEXT, "
                      "grade_name TEXT);";  
    int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'employee' checked/created successfully.\n";
    }
}

void Employee::addEmployee() {
    std::string name, dob, doj, mobileNo, state, city, department, gradeName;

    // Fetch and list departments
    std::vector<std::string> departments = dept->getAllDepartmentNames();
    if (!departments.empty()) {
        std::cout << "Select Department:" << std::endl;
        for (size_t i = 0; i < departments.size(); ++i) {
            std::cout << i + 1 << ". " << departments[i] << std::endl;
        }
        int choice;
        std::cin >> choice;
        department = departments[choice - 1];
        std::cout << "Selected Department: " << department << std::endl;
    }

    // Fetch pay grades based on the selected department
    std::vector<PayGradeDetail> payGrades = payGrade->listPayGradesByDepartment(department);
    std::vector<std::string> gradeNames;

    for (const auto& gradeDetail : payGrades) {
        gradeNames.push_back(gradeDetail.grade_name);
    }

    if (!payGrades.empty()) {
        std::cout << "Available Pay Grades for " << department << ":" << std::endl;
        for (size_t i = 0; i < payGrades.size(); ++i) {
            std::cout << i + 1 << ". " << payGrades[i].grade_name << std::endl;
        }
        int gradeChoice;
        std::cin >> gradeChoice;
        gradeName = payGrades[gradeChoice - 1].grade_name;
        std::cout << "Selected Grade Name: " << gradeName << std::endl;
    }

    std::cout << "Enter Name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
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
std::string sql = "INSERT INTO employee (name, dob, doj, mobileNo, state, city, department, grade_name) VALUES ('"
                      + name + "', '" + dob + "', '" + doj + "', '" + mobileNo + "', '" 
                      + state + "', '" + city + "', '" + department + "', '" + gradeName + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Employee added successfully!\n";
    }
}

std::map<std::string, std::string> Employee::getEmployee(const std::string& name) {
    std::map<std::string, std::string> employeeDetails;
    std::string sql = "SELECT name, dob, doj, mobileNo, state, city, department, grade_name FROM employee WHERE LOWER(name) = LOWER(?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error in prepare: " << sqlite3_errmsg(db) << std::endl;
        return employeeDetails;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string gradeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        employeeDetails["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        employeeDetails["dob"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        employeeDetails["doj"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        employeeDetails["mobileNo"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        employeeDetails["state"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        employeeDetails["city"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        employeeDetails["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        employeeDetails["grade_name"] = gradeName;

        // Now fetch pay grade details
        if (!gradeName.empty()) {
            sqlite3_stmt* pgStmt;
            std::string pgSql = "SELECT grade_basic, grade_da, grade_ta, grade_pf, grade_bonus FROM PayGrade WHERE grade_name = ?";
            if (sqlite3_prepare_v2(db, pgSql.c_str(), -1, &pgStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(pgStmt, 1, gradeName.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(pgStmt) == SQLITE_ROW) {
                    employeeDetails["grade_basic"] = std::to_string(sqlite3_column_double(pgStmt, 0));
                    employeeDetails["grade_da"] = std::to_string(sqlite3_column_double(pgStmt, 1));
                    employeeDetails["grade_ta"] = std::to_string(sqlite3_column_double(pgStmt, 2));
                    employeeDetails["grade_pf"] = std::to_string(sqlite3_column_double(pgStmt, 3));
                    employeeDetails["grade_bonus"] = std::to_string(sqlite3_column_double(pgStmt, 4));
                }
                sqlite3_finalize(pgStmt);
            } else {
                std::cerr << "SQL error in prepare pay grade details: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        // Print each detail
        std::cout << "Employee Details:" << std::endl;
        for (const auto& detail : employeeDetails) {
            std::cout << detail.first << ": " << detail.second << std::endl;
        }
    } else {
        std::cout << "No employee found with the name: " << name << std::endl;
    }

    sqlite3_finalize(stmt);
    return employeeDetails;
}

void Employee::deleteEmployee(const std::string& name) {
    // Check if the employee exists
    if (!getEmployee(name).empty()) {
        std::string sql = "DELETE FROM employee WHERE name = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                std::cout << "Employee successfully deleted.\n";
            } else {
                std::cerr << "Failed to delete employee: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_finalize(stmt);
        } else {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        std::cout << "No employee found with the name: " << name << std::endl;
    }
}


void Employee::handleEmployeeQuery() {
    std::string employeeName;
    std::cin.ignore(); 
    std::cout << "Enter Employee Name: ";
    std::getline(std::cin, employeeName);
    getEmployee(employeeName);
}

void Employee::handleDeleteEmployee() {
    std::string employeeName;
    std::cin.ignore(); 
    std::cout << "Enter Employee Name: ";
    std::getline(std::cin, employeeName);
    deleteEmployee(employeeName);
}