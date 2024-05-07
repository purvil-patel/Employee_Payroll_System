#include "managepaygrade.h"
#include "ui_managepaygrade.h"

ManagePaygrade::ManagePaygrade(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManagePaygrade)
{
    ui->setupUi(this);
}

ManagePaygrade::~ManagePaygrade()
{
    delete ui;
}

void ManagePaygrade::on_pushButton_clicked()
{
    emit backHome();
}

