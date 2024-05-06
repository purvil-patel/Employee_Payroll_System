#ifndef PAYGRADE_H
#define PAYGRADE_H

#include <sqlite3.h>
#include <string>

class PayGrade {
private:
    int grade_id; // This will now be set to auto-increment in the database, so we don't need to input it directly.
    std::string grade_name;
    float grade_basic, grade_da, grade_ta, grade_pf, grade_bonus;
    std::string department_name; // New field to store department name
    sqlite3* db;

public:
    PayGrade(sqlite3* dbConnection);
    void inputGradeDetails();
    void saveToDatabase();
    void listPayGradesByDepartment(const std::string& department);
};

#endif
