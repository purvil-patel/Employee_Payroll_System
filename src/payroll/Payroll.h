#ifndef PAYROLL_H
#define PAYROLL_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>        // Include for high-resolution clock
#include <atomic>        // Include for atomic operations
#include <sstream>       // Include for string streaming

struct PayrollDetail {
    int emp_id;
    std::string emp_name;
    std::string department_name;
    std::string paygrade_name;
    std::string transaction_id;
    std::string salary_issue_date;
    int salary_month;
    int salary_year;
    float net_salary;
};

class Payroll {
private:
    sqlite3* db;
    std::vector<PayrollDetail> payrollDetails;
    int salary_month, salary_year;
    std::string salary_issue_date;
    static std::atomic<int> sequence_number;  // Static atomic counter for transaction IDs
    std::string generateUniqueId(int emp_id);  // Method to generate unique transaction IDs

public:
    Payroll(sqlite3* dbConnection);
    void inputSalaryDetails();
    void generatePayroll();
    void calculateNetSalary(PayrollDetail &detail, const std::string& payGradeName);
    void saveToDatabase();
    std::vector<PayrollDetail> getPayrollTable(const std::string& empName = "");
    void displayPayrollRecords(const std::string& empName = "");
    void handlePayrollQuery();
};

#endif
