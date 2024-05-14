#ifndef MANAGEEMPLOYEEWIDGET_H
#define MANAGEEMPLOYEEWIDGET_H

#include <QWidget>
#include "src/department/Department.h"
#include "src/pay_grade/PayGrade.h"
#include "src/employee/Employee.h"
#include <QStandardItemModel>

namespace Ui {
class ManageEmployeeWidget;
}

class ManageEmployeeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ManageEmployeeWidget(QWidget *parent = nullptr);
    ~ManageEmployeeWidget();

private slots:
    void on_pushButton_clicked();
    void on_addButton_clicked();
    void on_pushButton_view_clicked();
    void onDeleteButtonClicked();
private:
    Ui::ManageEmployeeWidget *ui;
    sqlite3* db;
    QStandardItemModel* model;
    std::unique_ptr<Employee> employee;
    std::unique_ptr<Department> department;
    std::unique_ptr<PayGrade> payGrade;
    void updateDepartmentComboBox();
    void updatePayGradeComboBox(const QString &departmentName);
    void addEmployee();

signals:
    void backHome();
};

#endif // MANAGEEMPLOYEEWIDGET_H
