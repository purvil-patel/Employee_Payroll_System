#include "departmentwidget.h"
#include "ui_departmentwidget.h"

Departmentwidget::Departmentwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Departmentwidget)
{
    ui->setupUi(this);
}

Departmentwidget::~Departmentwidget()
{
    delete ui;
}

void Departmentwidget::on_pushButton_clicked()
{
    emit backHome();
}

