#ifndef DEPARTMENTWIDGET_H
#define DEPARTMENTWIDGET_H

#include <QWidget>

namespace Ui {
class Departmentwidget;
}

class Departmentwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Departmentwidget(QWidget *parent = nullptr);
    ~Departmentwidget();

private:
    Ui::Departmentwidget *ui;
};

#endif // DEPARTMENTWIDGET_H
