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
        
        Employee emp(db);
        Department dept(db);
        PayGrade pg(db);  // Create PayGrade instance
        Payroll pr(db);   // Create Payroll instance

        bool running = true;
        while (running) {
            int choice;
            std::cout << "Select an option:\n";
            std::cout << "1. Add Employee\n";
            std::cout << "2. Add Department\n";
            std::cout << "3. Manage Pay Grade\n";  // New option for managing pay grades
            std::cout << "4. Manage Payroll\n";    // New option for managing payroll
            std::cout << "5. Get Departments\n";
            std::cout << "6. Exit\n";
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
                    pg.inputGradeDetails();  // Input and save pay grade details
                    pg.saveToDatabase();
                    break;
                case 4:
                    pr.inputPayrollDetails();  // Input and calculate payroll details
                    pr.calculateNetSalary(pr.getGross() * 0.25); // Assume 25% deductions
                    pr.saveToDatabase();
                    break;
                case 5:
                    dept.getAllDepartmentNames();
                    break;
                case 6:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }
        std::cout << "Now you can perform operations on the Employee Payroll System.\n";
    } else {
        std::cout << "Login failed.\n";
    }

    return 0;
}
