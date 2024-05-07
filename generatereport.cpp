#include "generatereport.h"
#include "ui_generatereport.h"
#include "src/employee/Employee.h"
#include "src/payroll/Payroll.h"
#include "src/department/Department.h" // Assuming these are correctly included
#include "src/pay_grade/PayGrade.h"
#include "src/database/Database.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>

GenerateReport::GenerateReport(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::GenerateReport),
                                                  department(std::make_unique<Department>(Database::getInstance().getConn())),
                                                  employee(std::make_unique<Employee>(Database::getInstance().getConn()))
{
    ui->setupUi(this);
    connect(ui->searchButton, &QPushButton::clicked, this, &GenerateReport::on_searchButton_clicked);
    std::cout << "GenerateReport constructor called." << std::endl;

    // Set up the headers for the QTableWidget
    ui->tableWidget_report->setColumnCount(9); // Set the number of columns to 9
    QStringList headers = {"City", "Department", "Date of Birth", "Date of Joining", "Employee ID", "Grade", "Phone No", "Name", "State"};
    ui->tableWidget_report->setHorizontalHeaderLabels(headers);

    // Optional: Adjust column widths for better visibility
    ui->tableWidget_report->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

GenerateReport::~GenerateReport()
{
    delete ui;
}

void GenerateReport::on_searchButton_clicked()
{
    QString employeeName = ui->searchLineEdit->text();
    if (!employeeName.isEmpty())
    {
        auto employeeDetails = employee->getEmployee(employeeName.toStdString());

        if (!employeeDetails.empty())
        {
            ui->tableWidget_report->setRowCount(1);
            int column = 0;
            for (const auto &detail : employeeDetails)
            {
                ui->tableWidget_report->setItem(0, column++, new QTableWidgetItem(QString::fromStdString(detail.second)));
            }
        }
        else
        {
            QMessageBox::information(this, "No Results", "No employee found with the given name.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Input Error", "Please enter an employee name.");
    }
}


void GenerateReport::on_pushButton_clicked()
{
    emit backHome();
}

