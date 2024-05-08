#ifndef GENERATEREPORT_H
#define GENERATEREPORT_H

#include <QWidget>
#include "ui_generatereport.h"
#include <sqlite3.h>
#include "src/department/Department.h"
#include "src/pay_grade/PayGrade.h"
#include "src/employee/Employee.h"
#include <QStandardItemModel>

namespace Ui
{
    class GenerateReport;
}

class GenerateReport : public QWidget
{
    Q_OBJECT

public:
    explicit GenerateReport(QWidget *parent = nullptr);
    ~GenerateReport();

private slots:
    void on_searchButton_clicked(); // This slot is triggered when the search button is clicked

    void on_pushButton_clicked();

signals:
    void backHome();

private:
    Ui::GenerateReport *ui;
    sqlite3 *db;

    QStandardItemModel *model;
    std::unique_ptr<Department> department;
    std::unique_ptr<PayGrade> payGrade;
    QStandardItemModel *model_paygrades;
    std::unique_ptr<Employee> employee;
    void updateDepartmentComboBox();
    void addPaygrade();
    void updatePayGradeComboBox(const QString &departmentName);
    void addEmployee();
};

#endif // GENERATEREPORT_H
