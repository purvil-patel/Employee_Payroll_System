#include <iostream>
#include <sqlite3.h>
#include <string>
#include "src/user_authentication/Login.h"
#include "src/employee/Employee.h"
#include "src/department/Department.h"
#include "src/database/Database.h"
#include "src/pay_grade/PayGrade.h"
#include "src/payroll/Payroll.h"

int main() {
    sqlite3* db = Database::getInstance().getConn();
    std::string username, password;
    std::cout << "Please enter your username: ";
    std::cin >> username;
    std::cout << "Please enter your password: ";
    std::cin >> password;

    Login login(db);
    if (login.authenticate(username, password)) {
        std::cout << "Logged in successfully!\n";
        
        Department dept(db);
        PayGrade pg(db);  // Create PayGrade instance
        Employee emp(db, &dept, &pg);
        Payroll pr(db);   // Create Payroll instance

        bool running = true;
        std::string departmentName;
        std::string employeeName;
        std::vector<PayGradeDetail> payGrades; 
        while (running) {
            int choice;
            std::cout << "Select an option:\n";
            std::cout << "1. Add Employee\n";
            std::cout << "2. Add Department\n";
            std::cout << "3. Manage Pay Grade\n";
            std::cout << "4. Manage Payroll\n";
            std::cout << "5. Get Departments\n";
            std::cout << "6. List Paygrades by Department\n";
            std::cout << "7. Get Employee Details\n"; 
            std::cout << "8. Exit\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    emp.addEmployee();
                    break;
                case 2:
                    dept.addDepartment();
                    break;
                case 3:
                    pg.inputGradeDetails();
                    pg.saveToDatabase();
                    break;
                case 4:
                    pr.inputPayrollDetails();
                    pr.calculateNetSalary(pr.getGross() * 0.25); // Assume 25% deductions
                    pr.saveToDatabase();
                    break;
                case 5:
                    dept.getAllDepartmentNames();
                    break;
                case 6:
                    std::cin.ignore(); // Ignore any leftover newline in the buffer
                    std::cout << "Enter Department Name (or 'all' for all departments): ";
                    std::getline(std::cin, departmentName);
                    payGrades = pg.listPayGradesByDepartment(departmentName);
                    pg.printPayGrades(payGrades); // Use the new function
                    break;
                case 7:
                    std::cin.ignore(); // Clear buffer
                    std::cout << "Enter Employee Name: ";
                    std::getline(std::cin, employeeName);
                    emp.getEmployee(employeeName); // Method to fetch and display employee details
                    break;
                case 8:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }
        std::cout << "Exiting Employee Payroll System.\n";
    } else {
        std::cout << "Login failed.\n";
    }

    return 0;
}
