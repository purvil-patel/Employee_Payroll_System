#include "Payroll.h"
#include <chrono>
#include <random>
#include <sstream>
#include <atomic>

Payroll::Payroll(sqlite3* dbConnection) : db(dbConnection) {
    // SQL statement to create the Payroll table if it does not exist
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS Payroll ("
        "emp_id INTEGER, "
        "emp_name TEXT, "
        "department_name TEXT, "
        "paygrade_name TEXT, "
        "transaction_id TEXT PRIMARY KEY, "
        "salary_issue_date TEXT, "
        "salary_month INTEGER, "
        "salary_year INTEGER, "
        "emp_net_salary REAL, "
        "FOREIGN KEY(emp_id) REFERENCES employee(empId));"; // Assuming employee table has empId

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating Payroll table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void Payroll::inputSalaryDetails() {
    std::cout << "Enter Salary Month: "; std::cin >> salary_month;
    std::cout << "Enter Salary Year: "; std::cin >> salary_year;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
    std::cout << "Enter Salary Issue Date (YYYY-MM-DD): ";
    std::getline(std::cin, salary_issue_date);
}

std::atomic<int> Payroll::sequence_number(0);

std::string Payroll::generateUniqueId(int emp_id) {
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    int next_sequence = ++sequence_number;  // Atomically increment the sequence number

    std::stringstream ss;
    ss << emp_id << "-" << nanos << "-" << next_sequence;
    return ss.str();
}

void Payroll::generatePayroll() {
    // Query to fetch employee data
    std::string sql = "SELECT empId, name, department, grade_name FROM employee;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            PayrollDetail detail;
            detail.emp_id = sqlite3_column_int(stmt, 0);
            detail.emp_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            detail.department_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            detail.paygrade_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            detail.transaction_id = generateUniqueId(detail.emp_id);

            // Set other details and calculate net salary
            detail.salary_issue_date = salary_issue_date;
            detail.salary_month = salary_month;
            detail.salary_year = salary_year;
            calculateNetSalary(detail, detail.paygrade_name);

            // Push back to vector and later insert into the database
            payrollDetails.push_back(detail);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
}

void Payroll::calculateNetSalary(PayrollDetail &detail, const std::string& payGradeName) {
    std::string sql = "SELECT grade_basic, grade_da, grade_ta, grade_pf, grade_bonus FROM PayGrade WHERE grade_name = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, payGradeName.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float basic = sqlite3_column_double(stmt, 0);
            float da = sqlite3_column_double(stmt, 1);
            float ta = sqlite3_column_double(stmt, 2);
            float pf = sqlite3_column_double(stmt, 3);
            float bonus = sqlite3_column_double(stmt, 4);

            detail.net_salary = basic + da + ta + pf + bonus;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
}

void Payroll::saveToDatabase() {
    for (const auto& detail : payrollDetails) {
        std::string sql = "INSERT INTO Payroll (emp_id, emp_name, department_name, paygrade_name, transaction_id, salary_issue_date, salary_month, salary_year, emp_net_salary) VALUES ("
                          + std::to_string(detail.emp_id) + ", '" + detail.emp_name + "', '" + detail.department_name + "', '" 
                          + detail.paygrade_name + "', '" + detail.transaction_id + "', '" + detail.salary_issue_date + "', " 
                          + std::to_string(detail.salary_month) + ", " + std::to_string(detail.salary_year) + ", " 
                          + std::to_string(detail.net_salary) + ");";
        char *errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "SQL error during insert: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Payroll record for " << detail.emp_name << " saved successfully.\n";
        }
    }
}

#include "Payroll.h"

std::vector<PayrollDetail> Payroll::getPayrollTable(const std::string& empName) {
    std::vector<PayrollDetail> payrollDetails;  // This will store the fetched payroll records

    // Prepare SQL query based on whether a specific employee name is provided
    std::string sql;
    if (empName.empty()) {
        sql = "SELECT emp_id, emp_name, department_name, paygrade_name, transaction_id, salary_issue_date, salary_month, salary_year, emp_net_salary FROM Payroll;";
    } else {
        sql = "SELECT emp_id, emp_name, department_name, paygrade_name, transaction_id, salary_issue_date, salary_month, salary_year, emp_net_salary FROM Payroll WHERE emp_name = ?;";
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (!empName.empty()) {
            sqlite3_bind_text(stmt, 1, empName.c_str(), -1, SQLITE_STATIC);
        }

        // Execute the SQL query and populate the vector with the results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            PayrollDetail detail;
            detail.emp_id = sqlite3_column_int(stmt, 0);
            detail.emp_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            detail.department_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            detail.paygrade_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            detail.transaction_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            detail.salary_issue_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            detail.salary_month = sqlite3_column_int(stmt, 6);
            detail.salary_year = sqlite3_column_int(stmt, 7);
            detail.net_salary = sqlite3_column_double(stmt, 8);
            payrollDetails.push_back(detail);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    return payrollDetails;
}

void Payroll::displayPayrollRecords(const std::string& empName) {
    auto payrollRecords = getPayrollTable(empName);  // Use existing method to fetch records
    if (payrollRecords.empty()) {
        std::cout << "No payroll records found." << std::endl;
        return;
    }
    for (const auto& record : payrollRecords) {
        std::cout << "ID: " << record.emp_id
                  << ", Name: " << record.emp_name
                  << ", Department: " << record.department_name
                  << ", Paygrade: " << record.paygrade_name
                  << ", Salary Issued: " << record.salary_issue_date
                  << ", Month: " << record.salary_month
                  << ", Year: " << record.salary_year
                  << ", Net Salary: $" << record.net_salary
                  << std::endl;
    }
}

void Payroll::handlePayrollQuery() {
    std::string empName;
    std::cin.ignore(); 
    std::cout << "Enter Employee Name (leave blank for all records): ";
    std::getline(std::cin, empName);
    displayPayrollRecords(empName);
}
