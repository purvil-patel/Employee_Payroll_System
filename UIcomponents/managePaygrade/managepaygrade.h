#ifndef MANAGEPAYGRADE_H
#define MANAGEPAYGRADE_H

#include <QWidget>
#include <sqlite3.h>
#include "../../src/department/Department.h"
#include "../../src/pay_grade/PayGrade.h"
#include <QStandardItemModel>

namespace Ui {
class ManagePaygrade;
}

class ManagePaygrade : public QWidget {
    Q_OBJECT

public:
    explicit ManagePaygrade(QWidget *parent = nullptr);
    ~ManagePaygrade();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_clicked();
    // void on_pushButton_3_clicked();
    // void on_pushButton_update_clicked();
    void on_pushButton_view_clicked();

    void on_pushButton_update_clicked();
    void on_pushButton_update_fetch_clicked();
    void on_pushButton_update_submit_clicked();


signals:
    void backHome();

private:
    Ui::ManagePaygrade *ui;
    sqlite3 *db;
    std::unique_ptr<Department> department;
    std::unique_ptr<PayGrade> payGrade;
    QStandardItemModel *model_paygrades;
    void updateDepartmentComboBox();
    void addPaygrade();
};

#endif // MANAGEPAYGRADE_H
