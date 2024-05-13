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
    ui->tableWidget_report->setColumnCount(13); // Adjust number of columns
    QStringList headers = {"Employee ID", "Name", "Department", "Grade", "City", "State", "DOB", "DOJ", "Phone No", "BP", "DA", "TA", "Bonus"};
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
    QString employeeName = ui->searchLineEdit->text().trimmed();
    if (!employeeName.isEmpty())
    {
        auto employeeDetails = employee->getEmployee(employeeName.toStdString());

        if (!employeeDetails.empty())
        {
            ui->tableWidget_report->setRowCount(0);
            ui->tableWidget_report->insertRow(ui->tableWidget_report->rowCount());
            int column = 0;
            // List of keys used to align data with headers
            QStringList keys = {"empId", "name", "department", "grade_name", "city", "state", "dob", "doj", "mobileNo", "grade_basic", "grade_da", "grade_ta", "grade_bonus"};
            for (const QString &key : keys) {
                // Convert QString key to std::string before using it as map index
                std::string stdKey = key.toStdString();
                if (employeeDetails.find(stdKey) != employeeDetails.end()) {
                    ui->tableWidget_report->setItem(0, column++, new QTableWidgetItem(QString::fromStdString(employeeDetails[stdKey])));
                } else {
                    // Ensure there's a placeholder if no data found for a key
                    ui->tableWidget_report->setItem(0, column++, new QTableWidgetItem("N/A"));
                }
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

