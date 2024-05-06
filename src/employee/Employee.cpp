#include "Employee.h"
#include <iostream>
#include <vector>
#include <limits>

Employee::Employee(sqlite3 *db, Department *dept, PayGrade *payGrade) : db(db), dept(dept), payGrade(payGrade) {
    char *zErrMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS employee ("
                      "empId INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "dob TEXT NOT NULL, "
                      "doj TEXT NOT NULL, "
                      "mobileNo TEXT, "
                      "state TEXT, "
                      "city TEXT, "
                      "department TEXT, "
                      "grade_name TEXT);";  
    int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table 'employee' checked/created successfully.\n";
    }
}

void Employee::addEmployee() {
    std::string name, dob, doj, mobileNo, state, city, department, gradeName;

    // Fetch and list departments
    std::vector<std::string> departments = dept->getAllDepartmentNames();
    if (!departments.empty()) {
        std::cout << "Select Department:" << std::endl;
        for (size_t i = 0; i < departments.size(); ++i) {
            std::cout << i + 1 << ". " << departments[i] << std::endl;
        }
        int choice;
        std::cin >> choice;
        department = departments[choice - 1];
        std::cout << "Selected Department: " << department << std::endl;
    }

    // Fetch pay grades based on the selected department
    std::vector<PayGradeDetail> payGrades = payGrade->listPayGradesByDepartment(department);
    std::vector<std::string> gradeNames;

    for (const auto& gradeDetail : payGrades) {
        gradeNames.push_back(gradeDetail.grade_name);
    }

    if (!payGrades.empty()) {
        std::cout << "Available Pay Grades for " << department << ":" << std::endl;
        for (size_t i = 0; i < payGrades.size(); ++i) {
            std::cout << i + 1 << ". " << payGrades[i].grade_name << std::endl;
        }
        int gradeChoice;
        std::cin >> gradeChoice;
        gradeName = payGrades[gradeChoice - 1].grade_name;
        std::cout << "Selected Grade Name: " << gradeName << std::endl;
    }

    std::cout << "Enter Name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::getline(std::cin, name);
    std::cout << "Enter Date of Birth (YYYY-MM-DD): ";
    std::cin >> dob;
    std::cout << "Enter Date of Joining (YYYY-MM-DD): ";
    std::cin >> doj;
    std::cout << "Enter Mobile Number: ";
    std::cin >> mobileNo;
    std::cout << "Enter State: ";
    std::cin >> state;
    std::cout << "Enter City: ";
    std::cin >> city;

    char *zErrMsg = 0;
std::string sql = "INSERT INTO employee (name, dob, doj, mobileNo, state, city, department, grade_name) VALUES ('"
                      + name + "', '" + dob + "', '" + doj + "', '" + mobileNo + "', '" 
                      + state + "', '" + city + "', '" + department + "', '" + gradeName + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Employee added successfully!\n";
    }
}
