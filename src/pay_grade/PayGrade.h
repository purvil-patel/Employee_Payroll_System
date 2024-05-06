#ifndef PAYGRADE_H
#define PAYGRADE_H

#include <sqlite3.h>
#include <string>

class PayGrade {
private:
    int grade_id;
    std::string grade_name;
    float grade_basic, grade_da, grade_ta, grade_pf, grade_bonus;
    sqlite3* db;

public:
    PayGrade(sqlite3* dbConnection);
    void inputGradeDetails();
    void saveToDatabase();
};

#endif
