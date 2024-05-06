#include "PayGrade.h"
#include <iostream>

PayGrade::PayGrade(sqlite3* dbConnection) : db(dbConnection) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS PayGrade ("
        "grade_id INTEGER PRIMARY KEY, "
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

void PayGrade::inputGradeDetails() {
    std::cout << "Enter Pay Grade details:\n";
    std::cout << "Grade ID: "; std::cin >> grade_id;
    std::cout << "Grade Name: "; std::cin.ignore(); std::getline(std::cin, grade_name);
    std::cout << "Basic Pay: "; std::cin >> grade_basic;
    std::cout << "Dearness Allowance (DA): "; std::cin >> grade_da;
    std::cout << "Travel Allowance (TA): "; std::cin >> grade_ta;
    std::cout << "Provident Fund (PF): "; std::cin >> grade_pf;
    std::cout << "Bonus: "; std::cin >> grade_bonus;
}

void PayGrade::saveToDatabase() {
    std::string sql = "INSERT INTO PayGrade (grade_id, grade_name, grade_basic, grade_da, grade_ta, grade_pf, grade_bonus) VALUES ("
                      + std::to_string(grade_id) + ", '" + grade_name + "', " + std::to_string(grade_basic) + ", " 
                      + std::to_string(grade_da) + ", " + std::to_string(grade_ta) + ", " + std::to_string(grade_pf) 
                      + ", " + std::to_string(grade_bonus) + ");";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
