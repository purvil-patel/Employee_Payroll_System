#include "PayGrade.h"
#include "../department/Department.h"  
#include <iostream>
#include <limits>
#include <iomanip>

PayGrade::PayGrade(sqlite3* dbConnection) : db(dbConnection) {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS PayGrade ("
        "grade_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "department_name TEXT NOT NULL, "
        "grade_name TEXT NOT NULL, "
        "grade_basic REAL, "
        "grade_da REAL, "
        "grade_ta REAL, "
        "grade_pf REAL, "
        "grade_bonus REAL);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating PayGrade table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void clearInputBuffer() {
    std::cin.clear();  // Clears any error flags.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discards the buffer up to the newline character.
}

void PayGrade::handleAddPaygrade() {
    Department dept(db);
    auto departments = dept.getAllDepartmentNames();

    clearInputBuffer();  // Clear buffer before reading department name

    std::string department_name;
    std::string grade_name;
    float grade_basic, grade_da, grade_ta, grade_pf, grade_bonus;

    std::cout << "Select Department Name: ";
    std::getline(std::cin, department_name);

    std::cout << "Enter Grade Name: ";
    std::getline(std::cin, grade_name);

    std::cout << "Basic Pay: "; std::cin >> grade_basic; clearInputBuffer();
    std::cout << "Dearness Allowance (DA): "; std::cin >> grade_da; clearInputBuffer();
    std::cout << "Travel Allowance (TA): "; std::cin >> grade_ta; clearInputBuffer();
    std::cout << "Provident Fund (PF): "; std::cin >> grade_pf; clearInputBuffer();
    std::cout << "Bonus: "; std::cin >> grade_bonus; clearInputBuffer();

    // Pass the input data to the database function
    addPaygradeToDatabase(department_name, grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus);
}

void PayGrade::addPaygradeToDatabase(const std::string &department_name, const std::string &grade_name,
                                     float grade_basic, float grade_da, float grade_ta, float grade_pf, float grade_bonus) {
    std::string sql = "INSERT INTO PayGrade (department_name, grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus) VALUES ('"
                      + department_name + "', '" + grade_name + "', " + std::to_string(grade_basic) + ", "
                      + std::to_string(grade_da) + ", " + std::to_string(grade_ta) + ", " + std::to_string(grade_pf)
                      + ", " + std::to_string(grade_bonus) + ");";
    char *errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Pay grade successfully added to the database.\n";
    }
}

std::vector<PayGradeDetail> PayGrade::listPayGradesByDepartment(const std::string& department) {
    std::vector<PayGradeDetail> payGrades;

    std::string sql;
    if (department == "all") {
        sql = "SELECT department_name, grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus FROM PayGrade ORDER BY department_name;";
    } else {
        sql = "SELECT grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus FROM PayGrade WHERE department_name = '" + department + "';";
    }

    char* zErrMsg = 0;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return payGrades;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PayGradeDetail detail;
        if (department == "all") {
            detail.department_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            detail.grade_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            detail.basic_pay = sqlite3_column_double(stmt, 2);
            detail.dearness_allowance = sqlite3_column_double(stmt, 3);
            detail.travel_allowance = sqlite3_column_double(stmt, 4);
            detail.provident_fund = sqlite3_column_double(stmt, 5);
            detail.bonus = sqlite3_column_double(stmt, 6);
        } else {
            detail.department_name = department; // Use the provided department name directly
            detail.grade_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            detail.basic_pay = sqlite3_column_double(stmt, 1);
            detail.dearness_allowance = sqlite3_column_double(stmt, 2);
            detail.travel_allowance = sqlite3_column_double(stmt, 3);
            detail.provident_fund = sqlite3_column_double(stmt, 4);
            detail.bonus = sqlite3_column_double(stmt, 5);
        }
        payGrades.push_back(detail);
    }

    sqlite3_finalize(stmt);
    return payGrades;
}

void PayGrade::printPayGrades(const std::vector<PayGradeDetail>& payGrades) {
    if (payGrades.empty()) {
        std::cout << "No pay grades found.\n";
        return;
    }
    
    std::cout << "Paygrades Details:" << std::endl;
    std::cout << std::left << std::setw(20) << "Department" << std::setw(15) << "Grade Name" << std::setw(10) << "Basic"
              << std::setw(10) << "DA" << std::setw(10) << "TA" << std::setw(10) << "PF" << std::setw(10) << "Bonus" << std::endl;
    
    for (const auto& detail : payGrades) {
        std::cout << std::left << std::setw(20) << detail.department_name << std::setw(15) << detail.grade_name 
                  << std::setw(10) << detail.basic_pay << std::setw(10) << detail.dearness_allowance 
                  << std::setw(10) << detail.travel_allowance << std::setw(10) << detail.provident_fund 
                  << std::setw(10) << detail.bonus << std::endl;
    }
}

void PayGrade::handleDepartmentSelectionAndDisplay() {
    std::string departmentName;
    std::cin.ignore(); // Clear input buffer
    std::cout << "Enter Department Name (or 'all' for all departments): ";
    std::getline(std::cin, departmentName);  // Ensure this line executes.
    std::cout << "Received input: " << departmentName << std::endl;  // Debug print

    auto payGrades = listPayGradesByDepartment(departmentName);
    if (payGrades.empty()) {
        std::cout << "No pay grades found." << std::endl;
    } else {
        printPayGrades(payGrades);
    }
}

void PayGrade::collectUpdateInputs(std::string& gradeToUpdate, float& basic, float& da, float& ta, float& pf, float& bonus) {
    std::cout << "Enter the name of the pay grade to update: ";
    std::getline(std::cin >> std::ws, gradeToUpdate);

    std::cout << "Enter new Basic Pay: "; std::cin >> basic; clearInputBuffer();
    std::cout << "Enter new Dearness Allowance (DA): "; std::cin >> da; clearInputBuffer();
    std::cout << "Enter new Travel Allowance (TA): "; std::cin >> ta; clearInputBuffer();
    std::cout << "Enter new Provident Fund (PF): "; std::cin >> pf; clearInputBuffer();
    std::cout << "Enter new Bonus: "; std::cin >> bonus; clearInputBuffer();
}

void PayGrade::updatePayGradeDetails(const std::string& gradeToUpdate, float basic, float da, float ta, float pf, float bonus) {
    // Verify if the pay grade exists
    std::string checkSql = "SELECT grade_name FROM PayGrade WHERE grade_name = ?";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    sqlite3_bind_text(checkStmt, 1, gradeToUpdate.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(checkStmt) != SQLITE_ROW) {
        std::cout << "Pay grade \"" << gradeToUpdate << "\" not found.\n";
        sqlite3_finalize(checkStmt);
        return;
    }
    sqlite3_finalize(checkStmt);

    // Prepare the update statement
    std::string sql = "UPDATE PayGrade SET grade_basic = ?, grade_da = ?, grade_ta = ?, grade_pf = ?, grade_bonus = ? WHERE grade_name = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the new values to the statement
    sqlite3_bind_double(stmt, 1, basic);
    sqlite3_bind_double(stmt, 2, da);
    sqlite3_bind_double(stmt, 3, ta);
    sqlite3_bind_double(stmt, 4, pf);
    sqlite3_bind_double(stmt, 5, bonus);
    sqlite3_bind_text(stmt, 6, gradeToUpdate.c_str(), -1, SQLITE_STATIC);

    // Execute the update statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Pay grade \"" << gradeToUpdate << "\" updated successfully.\n";
    }

    sqlite3_finalize(stmt);
}

// Collect grade name input from the user
std::string PayGrade::collectGradeName() {
    std::string gradeName;
    std::cout << "Enter the name of the pay grade: ";
    std::getline(std::cin >> std::ws, gradeName);
    return gradeName;
}

PayGradeDetail PayGrade::getPayGradeDetailsByName(const std::string& grade_name) {
    PayGradeDetail detail;
    std::string sql = "SELECT department_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus "
                      "FROM PayGrade WHERE grade_name = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return detail; // Return an empty structure if there is an error
    }

    sqlite3_bind_text(stmt, 1, grade_name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        detail.department_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        detail.grade_name = grade_name;
        detail.basic_pay = sqlite3_column_double(stmt, 1);
        detail.dearness_allowance = sqlite3_column_double(stmt, 2);
        detail.travel_allowance = sqlite3_column_double(stmt, 3);
        detail.provident_fund = sqlite3_column_double(stmt, 4);
        detail.bonus = sqlite3_column_double(stmt, 5);
    } else {
        std::cerr << "Pay grade '" << grade_name << "' not found.\n";
    }

    sqlite3_finalize(stmt);
    return detail;
}