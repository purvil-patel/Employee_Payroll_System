#include "PayGrade.h"
#include "../department/Department.h" // Include Department class to use its methods
#include <iostream>
#include <limits> 
#include <iomanip>

PayGrade::PayGrade(sqlite3* dbConnection) : db(dbConnection) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS PayGrade ("
        "grade_id INTEGER PRIMARY KEY AUTOINCREMENT, " // Auto-increment
        "department_name TEXT NOT NULL, " // Include department name
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
    std::cin.clear(); // Clears any error flags.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discards the buffer up to the newline character.
}

void PayGrade::inputGradeDetails() {
    Department dept(db);
    auto departments = dept.getAllDepartmentNames();

    clearInputBuffer(); // Clear buffer before reading department name

    std::cout << "Select Department Name: ";
    std::getline(std::cin, department_name);

    std::cout << "Enter Grade Name: ";
    std::getline(std::cin, grade_name);

    std::cout << "Basic Pay: "; std::cin >> grade_basic; clearInputBuffer(); // Clear buffer after cin
    std::cout << "Dearness Allowance (DA): "; std::cin >> grade_da; clearInputBuffer(); // Clear buffer after cin
    std::cout << "Travel Allowance (TA): "; std::cin >> grade_ta; clearInputBuffer(); // Clear buffer after cin
    std::cout << "Bonus: "; std::cin >> grade_bonus; clearInputBuffer(); // Clear buffer after cin
}

void PayGrade::saveToDatabase() {
    std::string sql = "INSERT INTO PayGrade (department_name, grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus) VALUES ('"
                      + department_name + "', '" + grade_name + "', " + std::to_string(grade_basic) + ", " 
                      + std::to_string(grade_da) + ", " + std::to_string(grade_ta) + ", " + std::to_string(grade_pf)
                      + ", " + std::to_string(grade_bonus) + ");";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void PayGrade::listPayGradesByDepartment(const std::string& department) {
    std::string sql = "SELECT grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus FROM PayGrade WHERE department_name = '" + department + "';";
    char* zErrMsg = 0;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    std::cout << "Paygrades for Department: " << department << std::endl;
    std::cout << std::left << std::setw(15) << "Grade Name" << std::setw(10) << "Basic" << std::setw(10) << "DA"
              << std::setw(10) << "TA" << std::setw(10) << "PF" << std::setw(10) << "Bonus" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string grade_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        float basic = sqlite3_column_double(stmt, 1);
        float da = sqlite3_column_double(stmt, 2);
        float ta = sqlite3_column_double(stmt, 3);
        float pf = sqlite3_column_double(stmt, 4);
        float bonus = sqlite3_column_double(stmt, 5);

        std::cout << std::left << std::setw(15) << grade_name << std::setw(10) << basic << std::setw(10) << da
                  << std::setw(10) << ta << std::setw(10) << pf << std::setw(10) << bonus << std::endl;
    }

    sqlite3_finalize(stmt);
}
