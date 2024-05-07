#ifndef MANAGEEMPLOYEEWIDGET_H
#define MANAGEEMPLOYEEWIDGET_H

#include <QWidget>

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

private:
    Ui::ManageEmployeeWidget *ui;

signals:
    void backHome();
};

#endif // MANAGEEMPLOYEEWIDGET_H
