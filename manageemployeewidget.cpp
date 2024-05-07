#include "manageemployeewidget.h"
#include "ui_manageemployeewidget.h"

ManageEmployeeWidget::ManageEmployeeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManageEmployeeWidget)
{
    ui->setupUi(this);
}

ManageEmployeeWidget::~ManageEmployeeWidget()
{
    delete ui;
}

void ManageEmployeeWidget::on_pushButton_clicked()
{
    emit backHome();
}

