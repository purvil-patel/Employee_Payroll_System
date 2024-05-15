#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include "../department/departmentwidget.h"
#include "../manageEmployee/manageemployeewidget.h"
#include "../managePaygrade/managepaygrade.h"
#include "../manageEmpPayroll/manageemppayroll.h"
#include "../generateReport/generatereport.h"

namespace Ui {
class DashboardWidget;
}

class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget *parent = nullptr);
    ~DashboardWidget();

private slots:

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void moveBackHome();

    void on_managePaygrade_clicked();

    void on_manageEmpPayrole_clicked();

    void on_generateReport_clicked();

private:
    Ui::DashboardWidget *ui;
    Departmentwidget departmentwidget;
    ManageEmployeeWidget manageempwidget;
    ManagePaygrade managePaygrade;
    ManageEmpPayroll manageEmpPayroll;
    GenerateReport generateReport;
    int currentIndex;

signals:
    void logout();
};



#endif // DASHBOARDWIDGET_H
