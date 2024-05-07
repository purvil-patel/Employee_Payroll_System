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

private slots:
    void on_pushButton_clicked();

private:
    Ui::Departmentwidget *ui;

signals:
    void backHome();
};

#endif // DEPARTMENTWIDGET_H
