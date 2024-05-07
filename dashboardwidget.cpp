#include "dashboardwidget.h"
#include "ui_dashboardwidget.h"

DashboardWidget::DashboardWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardWidget)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(&departmentwidget);
    ui->stackedWidget->addWidget(&manageempwidget);
    ui->stackedWidget->addWidget(&managePaygrade);
    ui->stackedWidget->addWidget(&manageEmpPayroll);


    currentIndex = ui->stackedWidget->currentIndex();

    connect(&manageempwidget,SIGNAL(backHome()),this,SLOT(moveBackHome()));
    connect(&departmentwidget,SIGNAL(backHome()),this,SLOT(moveBackHome()));
    connect(&managePaygrade,SIGNAL(backHome()),this,SLOT(moveBackHome()));
    connect(&manageEmpPayroll,SIGNAL(backHome()),this,SLOT(moveBackHome()));
}

DashboardWidget::~DashboardWidget()
{
    delete ui;
}


void DashboardWidget::on_pushButton_7_clicked()
{
    emit logout();
}


void DashboardWidget::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(&departmentwidget);
}


void DashboardWidget::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(&manageempwidget);
}

void DashboardWidget::moveBackHome()
{
    ui->stackedWidget->setCurrentIndex(currentIndex);
}


void DashboardWidget::on_managePaygrade_clicked()
{
    ui->stackedWidget->setCurrentWidget(&managePaygrade);
}


void DashboardWidget::on_manageEmpPayrole_clicked()
{
    ui->stackedWidget->setCurrentWidget(&manageEmpPayroll);
}

