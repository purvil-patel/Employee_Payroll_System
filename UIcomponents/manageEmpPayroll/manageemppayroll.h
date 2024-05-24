#ifndef MANAGEEMPPAYROLL_H
#define MANAGEEMPPAYROLL_H

#include <QWidget>
#include <memory>
#include <sqlite3.h>
#include "../../src/payroll/Payroll.h"

namespace Ui {
class ManageEmpPayroll;
}

class ManageEmpPayroll : public QWidget {
    Q_OBJECT

public:
    explicit ManageEmpPayroll(QWidget *parent = nullptr);
    ~ManageEmpPayroll();

private slots:
    void on_pushButton_clicked();  // Existing Home button
    void on_pushButton_submit_clicked();
    void on_pushButton_add_clicked();
    void on_pushButton_view_clicked();  // View records button

private:
    Ui::ManageEmpPayroll *ui;
    sqlite3 *db;  // Database connection
    std::unique_ptr<Payroll> payrollHandler;  // Corrected name

signals:
    void backHome();
};

#endif // MANAGEEMPPAYROLL_H
