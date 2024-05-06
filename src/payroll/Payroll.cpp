#include "Payroll.h"
#include <iostream>

Payroll::Payroll(sqlite3* dbConnection) : db(dbConnection) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS Payroll ("
        "payroll_id INTEGER PRIMARY KEY, "
        "emp_id INTEGER, "
        "emp_gross REAL, "
        "emp_net_salary REAL, "
        "emp_salary_year INTEGER, "
        "emp_salary_month INTEGER, "
        "reimbursement_date TEXT, "
        "receipts TEXT, "
        "FOREIGN KEY(emp_id) REFERENCES Employee(emp_id));";  // Assuming Employee table with emp_id exists

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating Payroll table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void Payroll::inputPayrollDetails() {
    std::cout << "Enter Payroll details:\n";
    std::cout << "Employee ID: "; std::cin >> emp_id;
    std::cout << "Payroll ID: "; std::cin >> payroll_id;
    std::cout << "Gross Pay: "; std::cin >> emp_gross;
    std::cout << "Year: "; std::cin >> emp_salary_year;
    std::cout << "Month: "; std::cin >> emp_salary_month;
    std::cout << "Reimbursement Date: "; std::cin.ignore(); std::getline(std::cin, reimbursement_date);
    std::cout << "Receipts: "; std::getline(std::cin, receipts);
}

void Payroll::calculateNetSalary(float deductions) {
    emp_net_salary = emp_gross - deductions;
}

void Payroll::saveToDatabase() {
    std::string sql = "INSERT INTO Payroll (emp_id, payroll_id, emp_gross, emp_net_salary, emp_salary_year, emp_salary_month, reimbursement_date, receipts) VALUES ("
                      + std::to_string(emp_id) + ", " + std::to_string(payroll_id) + ", " + std::to_string(emp_gross) 
                      + ", " + std::to_string(emp_net_salary) + ", " + std::to_string(emp_salary_year) + ", " 
                      + std::to_string(emp_salary_month) + ", '" + reimbursement_date + "', '" + receipts + "');";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

float Payroll::getGross() const {
    return emp_gross;
}
