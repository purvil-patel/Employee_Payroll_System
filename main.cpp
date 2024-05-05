#include <iostream>
#include <sqlite3.h>
#include <string>
#include "src/user_authentication/Login.h"
#include "src/employee/Employee.h"
#include "src/department/Department.h"

int main()
{
    sqlite3 *db;
    sqlite3_open("payroll_system.db", &db);
    std::string username, password;
    std::cout << "Please enter your username: ";
    std::cin >> username;
    std::cout << "Please enter your password: ";
    std::cin >> password;

    Login login(db);
    if (login.authenticate(username, password)) {
        std::cout << "Logged in successfully!\n";
        // Add additional functionality post login
        Employee emp(db);
        Department dept(db);

        bool running = true;
        while (running) {
            int choice;
            std::cout << "Select an option:\n";
            std::cout << "1. Add Employee\n";
            std::cout << "2. Add Department\n";
            std::cout << "3. Exit\n";
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
