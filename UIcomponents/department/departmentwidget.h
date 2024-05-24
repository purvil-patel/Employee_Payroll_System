#ifndef DEPARTMENTWIDGET_H
#define DEPARTMENTWIDGET_H

#include <QWidget>
#include "../../src/department/Department.h"
#include <QStandardItemModel>


namespace Ui {
class Departmentwidget;
}

class Departmentwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Departmentwidget(QWidget *parent = nullptr);
    ~Departmentwidget();

private slots:
    void on_pushButton_clicked();

    void on_submitNewDepartmentButton_clicked();

    void checkInput(const QString &text);

private:
    Ui::Departmentwidget *ui;
    sqlite3* db;
    QStandardItemModel* model;
    std::unique_ptr<Department> department;

signals:
    void backHome();
};

#endif // DEPARTMENTWIDGET_H
