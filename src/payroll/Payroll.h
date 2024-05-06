#ifndef PAYROLL_H
#define PAYROLL_H

#include <sqlite3.h>
#include <string>

class Payroll {
private:
    int emp_id, payroll_id;
    float emp_gross, emp_net_salary;
    int emp_salary_year, emp_salary_month;
    std::string reimbursement_date, receipts;
    sqlite3* db;

public:
    Payroll(sqlite3* dbConnection);
    void inputPayrollDetails();
    void calculateNetSalary(float deductions);
    void saveToDatabase();
    float getGross() const;
};

#endif
