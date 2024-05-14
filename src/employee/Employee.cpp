#include "Employee.h"
#include <iostream>
#include <vector>
#include <limits>
#include <map>
#include <algorithm>
#include <cctype>

Employee::Employee(sqlite3 *db, Department *dept, PayGrade *payGrade) : db(db), dept(dept), payGrade(payGrade)
{
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
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        std::cout << "Table 'employee' checked/created successfully.\n";
    }
}

Employee::Employee(sqlite3 *db):db(db){

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
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        std::cout << "Table 'employee' checked/created successfully.\n";
    }

}

bool Employee::addEmployee(const std::string& name, const std::string& dob, const std::string& doj, const std::string& mobileNo, const std::string& state, const std::string& city, const std::string& department, const std::string& gradeName) {
    char *zErrMsg = 0;
    std::string sql = "INSERT INTO employee (name, dob, doj, mobileNo, state, city, department, grade_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind all parameters to the prepared statement
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, dob.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, doj.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, mobileNo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, state.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, city.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, department.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 8, gradeName.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the statement
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Employee added successfully!\n";
            return true;
        } else {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
}


std::map<std::string, std::string> Employee::getEmployeeById(const std::string& empId) {
    std::map<std::string, std::string> employeeDetails;
    // Include empId in the SELECT query
    std::string sql = "SELECT empId, name, dob, doj, mobileNo, state, city, department, grade_name FROM employee WHERE empId = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error in prepare: " << sqlite3_errmsg(db) << std::endl;
        return employeeDetails;
    }

    sqlite3_bind_text(stmt, 1, empId.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string gradeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        employeeDetails["empId"] = std::to_string(sqlite3_column_int(stmt, 0));  // Get empId as integer and convert to string
        employeeDetails["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        employeeDetails["dob"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        employeeDetails["doj"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        employeeDetails["mobileNo"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        employeeDetails["state"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        employeeDetails["city"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        employeeDetails["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        employeeDetails["grade_name"] = gradeName;

        // Fetch pay grade details if available
        if (!gradeName.empty()) {
            sqlite3_stmt* pgStmt;
            std::string pgSql = "SELECT grade_basic, grade_da, grade_ta, grade_bonus FROM PayGrade WHERE grade_name = ?";
            if (sqlite3_prepare_v2(db, pgSql.c_str(), -1, &pgStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(pgStmt, 1, gradeName.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(pgStmt) == SQLITE_ROW) {
                    employeeDetails["grade_basic"] = std::to_string(sqlite3_column_double(pgStmt, 0));
                    employeeDetails["grade_da"] = std::to_string(sqlite3_column_double(pgStmt, 1));
                    employeeDetails["grade_ta"] = std::to_string(sqlite3_column_double(pgStmt, 2));
                    employeeDetails["grade_bonus"] = std::to_string(sqlite3_column_double(pgStmt, 3));
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
        std::cout << "No employee found with the empId: " << empId << std::endl;
    }

    sqlite3_finalize(stmt);
    return employeeDetails;
}



std::map<std::string, std::string> Employee::getEmployee(const std::string& name) {
    std::map<std::string, std::string> employeeDetails;
    // Include empId in the SELECT query
    std::string sql = "SELECT empId, name, dob, doj, mobileNo, state, city, department, grade_name FROM employee WHERE LOWER(name) = LOWER(?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error in prepare: " << sqlite3_errmsg(db) << std::endl;
        return employeeDetails;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string gradeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        employeeDetails["empId"] = std::to_string(sqlite3_column_int(stmt, 0));  // Get empId as integer and convert to string
        employeeDetails["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        employeeDetails["dob"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        employeeDetails["doj"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        employeeDetails["mobileNo"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        employeeDetails["state"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        employeeDetails["city"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        employeeDetails["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        employeeDetails["grade_name"] = gradeName;

        // Fetch pay grade details if available
        if (!gradeName.empty()) {
            sqlite3_stmt* pgStmt;
            std::string pgSql = "SELECT grade_basic, grade_da, grade_ta, grade_bonus FROM PayGrade WHERE grade_name = ?";
            if (sqlite3_prepare_v2(db, pgSql.c_str(), -1, &pgStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(pgStmt, 1, gradeName.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(pgStmt) == SQLITE_ROW) {
                    employeeDetails["grade_basic"] = std::to_string(sqlite3_column_double(pgStmt, 0));
                    employeeDetails["grade_da"] = std::to_string(sqlite3_column_double(pgStmt, 1));
                    employeeDetails["grade_ta"] = std::to_string(sqlite3_column_double(pgStmt, 2));
                    employeeDetails["grade_bonus"] = std::to_string(sqlite3_column_double(pgStmt, 3));
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


bool Employee::deleteEmployee(const std::string &empId) {
    // Check if employee with the given ID exists
    std::string sql = "SELECT COUNT(*) FROM employee WHERE empId = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, empId.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 1) {
            // Employee exists, proceed with deletion
            sqlite3_finalize(stmt);

            // Prepare the SQL DELETE statement using empId
            sql = "DELETE FROM employee WHERE empId = ?";
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, empId.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(stmt) == SQLITE_DONE) {
                    std::cout << "Employee successfully deleted.\n";
                    sqlite3_finalize(stmt);
                    return true; // Return true if deletion was successful
                } else {
                    // std::cerr << "Failed to delete employee: " << sqlite3_errmsg(db) << std::endl;
                    std::cerr << "Employee with ID " << empId << " does not exist.\n";
                }
                sqlite3_finalize(stmt);
            } else {
                std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
            }
        } else {
            // Employee with the given ID does not exist
            std::cerr << "Employee with ID " << empId << " does not exist.\n";
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
    }
    return false; // Return false if deletion was unsuccessful
}

bool Employee::updateEmployee(const std::string &empId, const std::string &name, const std::string &dob, const std::string &doj, const std::string &mobileNo, const std::string &state, const std::string &city, const std::string &department, const std::string &gradeName)
{
    // Construct the SQL update statement
    std::string sql = "UPDATE employee SET name = ?, dob = ?, doj = ?, mobileNo = ?, state = ?, city = ?, department = ?, grade_name = ? WHERE empId = ?";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        // Binding the values to the statement
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, dob.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, doj.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, mobileNo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, state.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, city.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, department.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 8, gradeName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 9, empId.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the update statement
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            std::cout << "Employee updated successfully.\n";
            sqlite3_finalize(stmt);
            return true; // Return true on successful update
        }
        else
        {
            std::cerr << "Failed to update employee: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
    }
    return false; // Return false if update fails
}


void Employee::handleEmployeeQuery()
{
    std::string employeeName;
    std::cin.ignore();
    std::cout << "Enter Employee Name: ";
    std::getline(std::cin, employeeName);
    getEmployee(employeeName);
}

void Employee::handleDeleteEmployee() {
    std::string employeeId;
    std::cin.ignore();
    std::cout << "Enter Employee ID: ";
    std::getline(std::cin, employeeId);
    deleteEmployee(employeeId);
}

void Employee::handleUpdateEmployee()
{

    std::string empId, name, dob, doj, mobileNo, state, city, department, gradeName;
    std::cout << "Enter ID: ";
    std::cin >> empId;
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
    std::cout << "Enter Department: ";
    std::cin >> department;
    std::cout << "Enter Grade Name: ";
    std::cin >> gradeName;
    updateEmployee(empId, name, dob, doj, mobileNo, state, city, department, gradeName);
}

void Employee::handleAddEmployee()
{
    std::string name, dob, doj, mobileNo, state, city, department, gradeName;

    // Fetch and list departments
    std::vector<std::string> departments = dept->getAllDepartmentNames();
    if (!departments.empty())
    {
        std::cout << "Select Department:" << std::endl;
        for (size_t i = 0; i < departments.size(); ++i)
        {
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

    for (const auto &gradeDetail : payGrades)
    {
        gradeNames.push_back(gradeDetail.grade_name);
    }

    if (!payGrades.empty())
    {
        std::cout << "Available Pay Grades for " << department << ":" << std::endl;
        for (size_t i = 0; i < payGrades.size(); ++i)
        {
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
    addEmployee(name, dob, doj, mobileNo, state, city, department, gradeName);
}

std::vector<std::map<std::string, std::string>> Employee::getAllEmployees() {
    std::vector<std::map<std::string, std::string>> employees;
    std::string sql = "SELECT empId, name, dob, doj, mobileNo, state, city, department, grade_name FROM employee";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error in prepare: " << sqlite3_errmsg(db) << std::endl;
        return employees;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> employeeDetails;

        // Fetch each column and store in the map
        employeeDetails["empId"] = std::to_string(sqlite3_column_int(stmt, 0));
        employeeDetails["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        employeeDetails["dob"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        employeeDetails["doj"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        employeeDetails["mobileNo"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        employeeDetails["state"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        employeeDetails["city"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        employeeDetails["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        employeeDetails["grade_name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));

        employees.push_back(employeeDetails);
    }

    sqlite3_finalize(stmt);
    return employees;
}
