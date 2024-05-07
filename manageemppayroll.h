#ifndef MANAGEEMPPAYROLL_H
#define MANAGEEMPPAYROLL_H

#include <QWidget>

namespace Ui {
class ManageEmpPayroll;
}

class ManageEmpPayroll : public QWidget
{
    Q_OBJECT

public:
    explicit ManageEmpPayroll(QWidget *parent = nullptr);
    ~ManageEmpPayroll();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ManageEmpPayroll *ui;

signals:
    void backHome();
};

#endif // MANAGEEMPPAYROLL_H
