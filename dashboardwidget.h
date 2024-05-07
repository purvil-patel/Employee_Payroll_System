#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include "departmentwidget.h"
#include "manageemployeewidget.h"
#include "managepaygrade.h"

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

private:
    Ui::DashboardWidget *ui;
    Departmentwidget departmentwidget;
    ManageEmployeeWidget manageempwidget;
    ManagePaygrade managePaygrade;
    int currentIndex;

signals:
    void logout();
};



#endif // DASHBOARDWIDGET_H
