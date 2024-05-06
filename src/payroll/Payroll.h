#ifndef PAYROLL_H
#define PAYROLL_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "uuid/uuid.h"  // You need to have uuid library available

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
