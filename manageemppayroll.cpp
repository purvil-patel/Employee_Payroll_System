#include "manageemppayroll.h"
#include "ui_manageemppayroll.h"
#include <iostream>
#include <QMessageBox>
#include "src/payroll/Payroll.h"
#include "src/database/Database.h"

ManageEmpPayroll::ManageEmpPayroll(QWidget *parent)
    : QWidget(parent), ui(new Ui::ManageEmpPayroll), payrollHandler(std::make_unique<Payroll>(Database::getInstance().getConn())) {
    ui->setupUi(this);
    ui->tableWidget_payroll->setColumnCount(8);  // Ensure you have 8 columns
    QStringList headers = {"Employee ID", "Employee Name", "Department", "Pay Grade", "Net Salary", "Salary Month", "Salary Year", "Salary Date"};
    ui->tableWidget_payroll->setHorizontalHeaderLabels(headers);
    ui->tableWidget_payroll->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_payroll->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Initially hide input fields and labels
    ui->lineEdit_date->setVisible(false);
    ui->comboBox_month->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->label_month->setVisible(false);
    ui->label_year->setVisible(false);
    ui->label_date->setVisible(false);
    ui->pushButton_submit->setVisible(false);
    ui->tableWidget_payroll->setVisible(false);

    // Connect buttons to their respective slots
    connect(ui->pushButton_add, &QPushButton::clicked, this, &ManageEmpPayroll::on_pushButton_add_clicked);
    connect(ui->pushButton_view, &QPushButton::clicked, this, &ManageEmpPayroll::on_pushButton_view_clicked);
}

ManageEmpPayroll::~ManageEmpPayroll() {
    delete ui;
}

void ManageEmpPayroll::on_pushButton_clicked()
{
    ui->lineEdit_date->setVisible(false);
    ui->comboBox_month->setVisible(false);
    ui->lineEdit_2->setVisible(false);

    ui->label_month->setVisible(false);
    ui->label_year->setVisible(false);
    ui->label_date->setVisible(false);
    ui->tableWidget_payroll->setVisible(false);

    ui->pushButton_submit->setVisible(false);
    std::cout << "Home button clicked." << std::endl;
    emit backHome();
}


void ManageEmpPayroll::on_pushButton_add_clicked()
{
    std::cout << "Add button clicked!" << std::endl;

    // Make input fields and labels visible
    ui->lineEdit_date->setVisible(true);
    ui->comboBox_month->setVisible(true);
    ui->lineEdit_2->setVisible(true);

    ui->label_month->setVisible(true);
    ui->label_year->setVisible(true);
    ui->label_date->setVisible(true);

    ui->pushButton_submit->setVisible(true);
}

void ManageEmpPayroll::on_pushButton_submit_clicked()
{
    // Retrieve data from the UI
    QString monthName = ui->comboBox_month->currentText();
    QString yearStr = ui->lineEdit_date->text();
    QString issueDate = ui->lineEdit_2->text();

    // Input validation
    if (yearStr.isEmpty() || issueDate.isEmpty() || monthName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields correctly.");
        return;
    }

    int year = yearStr.toInt();
    std::string issueDateStr = issueDate.toStdString();
    std::string monthStr = monthName.toStdString();

    // Set the payroll details in the Payroll object
    payrollHandler->setPayrollDetails(year, monthStr, issueDateStr);

    // Generate payroll based on the details
    payrollHandler->generatePayroll();

    // Save the generated payroll to the database
    payrollHandler->saveToDatabase();

    QMessageBox::information(this, "Success", "Payroll data submitted successfully.");

    // Optionally clear the fields or hide them again
    ui->lineEdit_date->clear();
    ui->lineEdit_2->clear();
    ui->comboBox_month->setCurrentIndex(0);

    ui->lineEdit_date->setVisible(false);
    ui->comboBox_month->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->label_month->setVisible(false);
    ui->label_year->setVisible(false);
    ui->label_date->setVisible(false);
    ui->pushButton_submit->setVisible(false);
}

void ManageEmpPayroll::on_pushButton_view_clicked() {
    std::cout << "View button clicked!" << std::endl;
    ui->lineEdit_date->setVisible(false);
    ui->comboBox_month->setVisible(false);
    ui->lineEdit_2->setVisible(false);

    ui->label_month->setVisible(false);
    ui->label_year->setVisible(false);
    ui->label_date->setVisible(false);

    ui->pushButton_submit->setVisible(false);
    ui->tableWidget_payroll->setVisible(true);
    ui->tableWidget_payroll->setRowCount(0);  // Clear existing rows

    std::vector<PayrollDetail> payrollRecords = payrollHandler->getPayrollTable("");
    if (payrollRecords.empty()) {
        std::cout << "No payroll records found." << std::endl;
    } else {
        for (const auto& record : payrollRecords) {
            std::cout << "ID: " << record.emp_id
                      << ", Name: " << record.emp_name
                      << ", Department: " << record.department_name
                      << ", Paygrade: " << record.paygrade_name
                      << ", Salary Issued: " << record.salary_issue_date
                      << ", Month: " << record.salary_month
                      << ", Year: " << record.salary_year
                      << ", Net Salary: $" << record.net_salary
                      << std::endl;
            int row = ui->tableWidget_payroll->rowCount();
            ui->tableWidget_payroll->insertRow(row);
            ui->tableWidget_payroll->setItem(row, 0, new QTableWidgetItem(QString::number(record.emp_id)));
            ui->tableWidget_payroll->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(record.emp_name)));
            ui->tableWidget_payroll->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(record.department_name)));
            ui->tableWidget_payroll->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(record.paygrade_name)));
            ui->tableWidget_payroll->setItem(row, 4, new QTableWidgetItem(QString::number(record.net_salary)));
            ui->tableWidget_payroll->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(record.salary_month)));
            ui->tableWidget_payroll->setItem(row, 6, new QTableWidgetItem(QString::number(record.salary_year)));
            ui->tableWidget_payroll->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(record.salary_issue_date)));
        }
        ui->tableWidget_payroll->resizeColumnsToContents();  // Adjust column sizes
    }
}
