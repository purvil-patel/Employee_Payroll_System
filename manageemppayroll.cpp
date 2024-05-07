#include "manageemppayroll.h"
#include "ui_manageemppayroll.h"

ManageEmpPayroll::ManageEmpPayroll(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManageEmpPayroll)
{
    ui->setupUi(this);

}

ManageEmpPayroll::~ManageEmpPayroll()
{
    delete ui;
}

void ManageEmpPayroll::on_pushButton_clicked()
{
    emit backHome();
}


void ManageEmpPayroll::on_pushButton_2_clicked()
{

}

