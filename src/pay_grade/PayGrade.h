#ifndef PAYGRADE_H
#define PAYGRADE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>

// Define a structure to store paygrade details
struct PayGradeDetail {
    std::string department_name;
    std::string grade_name;
    float basic_pay;
    float dearness_allowance;
    float travel_allowance;
    float bonus;
};

class PayGrade {
private:
    sqlite3* db;

public:
    PayGrade(sqlite3* dbConnection);
    void handleAddPaygrade();
    void addPaygradeToDatabase(const std::string& department_name, const std::string& grade_name,
                           float grade_basic, float grade_da, float grade_ta, float grade_bonus);
    std::vector<PayGradeDetail> listPayGradesByDepartment(const std::string& department);
    void printPayGrades(const std::vector<PayGradeDetail>& payGrades);
    void handleDepartmentSelectionAndDisplay();
    void collectUpdateInputs(std::string& gradeToUpdate, float& basic, float& da, float& ta, float& bonus);
    void updatePayGradeDetails(const std::string& gradeToUpdate, float basic, float da, float ta, float bonus);
    PayGradeDetail getPayGradeDetailsByName(const std::string& grade_name);

    // Function to collect grade name from user input
    std::string collectGradeName();
};

#endif // PAYGRADE_H
