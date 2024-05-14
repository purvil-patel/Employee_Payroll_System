#include "manageemployeewidget.h"
#include "ui_manageemployeewidget.h"
#include "src/database/Database.h"
#include <QMessageBox>
#include <QInputDialog>
#include <iostream>

ManageEmployeeWidget::ManageEmployeeWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ManageEmployeeWidget)
    ,employee(std::make_unique<Employee>(Database::getInstance().getConn()))
    ,department(std::make_unique<Department>(Database::getInstance().getConn()))
, payGrade(std::make_unique<PayGrade>(Database::getInstance().getConn()))
{
    ui->setupUi(this);

    ui->tableWidget_employees->setColumnCount(8);
    QStringList headers = {"ID", "Name", "DOB", "DOJ", "Mobile No", "State", "City", "Department", "Grade Name"};
    ui->tableWidget_employees->setHorizontalHeaderLabels(headers);

    // std::vector<std::string> departments = department->getAllDepartmentNames();

    // for (const std::string& name : departments) {
    //     ui->comboBox->addItem(QString::fromStdString(name));
    // }

    // Initially hide the input fields, labels, and Add Employee button
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->lineEdit_7->setVisible(false);
    ui->lineEdit_8->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    // Hiding the labels
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->tableWidget_employees->setVisible(false);

    ui->pushButton_4->setVisible(false);
    ui->label_4->setVisible(false);
    ui->lineEdit_3->setVisible(false);

    ui->pushButton_6->setVisible(false);

    // Connect the Add button's clicked signal to the slot
    connect(ui->pushButton_2, &QPushButton::clicked, this, &ManageEmployeeWidget::on_addButton_clicked);

    //connect(ui->comboBox, &QComboBox::highlighted, this, &ManageEmployeeWidget::updateDepartmentComboBox);
    //connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updatePayGradeComboBox(QString)));
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &ManageEmployeeWidget::updatePayGradeComboBox);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &ManageEmployeeWidget::addEmployee);
    connect(ui->pushButton_view, &QPushButton::clicked, this, &ManageEmployeeWidget::on_pushButton_view_clicked);
    connect(ui->pushButton_Delete, &QPushButton::clicked, this, &ManageEmployeeWidget::onDeleteButtonClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &ManageEmployeeWidget::on_pushButton_update_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &ManageEmployeeWidget::on_pushButton_update_fetch_clicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &ManageEmployeeWidget::on_pushButton_update_submit_clicked);
}

ManageEmployeeWidget::~ManageEmployeeWidget()
{
    delete ui;
}

void ManageEmployeeWidget::updateDepartmentComboBox() {
    ui->comboBox->clear(); // Clear existing items
    std::vector<std::string> departments = department->getAllDepartmentNames();
    for (const std::string &name : departments) {
        ui->comboBox->addItem(QString::fromStdString(name));
    }
}

void ManageEmployeeWidget::on_pushButton_update_clicked()
{

    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->lineEdit_7->setVisible(false);
    ui->lineEdit_8->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    // Hiding the labels
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->tableWidget_employees->setVisible(false);
    ui->pushButton_4->setVisible(true);
    ui->label_4->setVisible(true);
    ui->lineEdit_3->setVisible(true);
    ui->pushButton_6->setVisible(false);
}

void ManageEmployeeWidget::on_pushButton_update_fetch_clicked() {

    std::string empId = ui->lineEdit_3->text().toStdString();

    // Fetch the employee details
    std::map<std::string, std::string> details = employee->getEmployee(empId);

    // Check if the employee was found and update the UI accordingly
    if (!details.empty()) {
        // Set UI elements with fetched employee details
        ui->lineEdit->setText(QString::fromStdString(details["name"]));
        ui->lineEdit_2->setText(QString::fromStdString(details["state"]));
        ui->lineEdit_6->setText(QString::fromStdString(details["dob"]));
        ui->lineEdit_7->setText(QString::fromStdString(details["mobileNo"]));
        ui->lineEdit_5->setText(QString::fromStdString(details["doj"]));
        ui->lineEdit_8->setText(QString::fromStdString(details["city"]));
        ui->comboBox->setCurrentText(QString::fromStdString(details["department"]));
        ui->comboBox_2->setCurrentText(QString::fromStdString(details["grade_name"]));

        ui->lineEdit->setVisible(true);
        ui->lineEdit_2->setVisible(true);
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);
        ui->lineEdit_7->setVisible(true);
        ui->lineEdit_8->setVisible(true);
        ui->comboBox->setVisible(true);
        ui->comboBox_2->setVisible(true);
        ui->pushButton_3->setVisible(false);
        // Toggling the labels' visibility
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_6->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->pushButton_6->setVisible(true);
        ui->label_4->setVisible(false);
        ui->lineEdit_3->setVisible(false);


        // Enable the submit button
        ui->pushButton_4->setVisible(false);
    } else {
        QMessageBox::warning(this, "Not Found", "Employee not found. Please check the ID and try again.");
    }
}

void ManageEmployeeWidget::on_pushButton_update_submit_clicked() {
    // std::string gradeName = ui->lineEdit_name->text().toStdString();
    std::string empId = ui->lineEdit_3->text().toStdString();
    // float basic = ui->lineEdit_basic->text().toFloat();
    // float da = ui->lineEdit_DA->text().toFloat();
    // float ta = ui->lineEdit_TA->text().toFloat();
    // float bonus = ui->lineEdit_bonus->text().toFloat();

    QString name = ui->lineEdit->text();
    QString dob = ui->lineEdit_6->text();
    QString doj = ui->lineEdit_5->text();
    QString mobileNo = ui->lineEdit_7->text();
    QString state = ui->lineEdit_2->text();
    QString city = ui->lineEdit_8->text();
    QString department = ui->comboBox->currentText();
    QString gradeName = ui->comboBox_2->currentText();

    // Call the backend function to update the paygrade
    // employee->updateEmployee(gradeName, basic, da, ta, bonus);

    // Call the backend function to add an employee
    if (!name.isEmpty() && !dob.isEmpty() && !doj.isEmpty()) { // Minimal validation example
        bool result = employee->updateEmployee(empId,name.toStdString(), dob.toStdString(), doj.toStdString(),
                                            mobileNo.toStdString(), state.toStdString(), city.toStdString(),
                                            department.toStdString(), gradeName.toStdString());

        if(result){
            QMessageBox::information(this, "Success", "Record Updated!!");
        }else{
            QMessageBox::warning(this, "Failure", "Error Occured!!!");
        }


    } else {
        // Handle the error case or invalid input
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
    }

    ui->lineEdit->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_8->clear();
    ui->comboBox->currentText();
    ui->comboBox_2->currentText();

}

void ManageEmployeeWidget::onDeleteButtonClicked()
{
    // Open a dialog to get employee ID input
    bool ok;
    QString text = QInputDialog::getText(this, tr("Delete Employee"),
                                         tr("Enter Employee ID:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        // Call deleteEmployee() function with the employee ID
        std::string employeeId = text.toStdString();
        bool deletionSuccess = employee->deleteEmployee(employeeId);

        // Show message box to inform user if deletion was successful
        if (deletionSuccess) {
            on_pushButton_view_clicked();
            QMessageBox::information(this, tr("Employee Deleted"), tr("Employee has been deleted."));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Employee with ID %1 does not exist.").arg(text));
        }
    }
}

void ManageEmployeeWidget::updatePayGradeComboBox(const QString &departmentName) {
    ui->comboBox_2->clear(); // Clear existing items
    qDebug() << "Department selected:" << departmentName;
    // Fetch the details of pay grades based on the selected department
    std::vector<PayGradeDetail> payGrades = payGrade->listPayGradesByDepartment(departmentName.toStdString());
    std::vector<std::string> gradeNames;

    // Extract just the names from the PayGradeDetail structures
    for (const auto &gradeDetail : payGrades) {
        gradeNames.push_back(gradeDetail.grade_name);
        std::cout<<"EEEEEEEEE"<<gradeDetail.grade_name;
    }

    // Populate the ComboBox with grade names
    for (const std::string &name : gradeNames) {
        ui->comboBox_2->addItem(QString::fromStdString(name));
    }
}

// In ManageEmployeeWidget.cpp
void ManageEmployeeWidget::addEmployee() {
    // Retrieve data from UI elements
    QString name = ui->lineEdit->text();
    QString dob = ui->lineEdit_6->text();
    QString doj = ui->lineEdit_5->text();
    QString mobileNo = ui->lineEdit_7->text();
    QString state = ui->lineEdit_2->text();
    QString city = ui->lineEdit_8->text();
    QString department = ui->comboBox->currentText();
    QString gradeName = ui->comboBox_2->currentText();

    // Call the backend function to add an employee
    if (!name.isEmpty() && !dob.isEmpty() && !doj.isEmpty()) { // Minimal validation example
        bool result = employee->addEmployee(name.toStdString(), dob.toStdString(), doj.toStdString(),
                              mobileNo.toStdString(), state.toStdString(), city.toStdString(),
                              department.toStdString(), gradeName.toStdString());

        if(result){
            QMessageBox::information(this, "Success", "Record Inserted!!");
        }else{
            QMessageBox::warning(this, "Failure", "Error Occured!!!");
        }


    } else {
        // Handle the error case or invalid input
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
    }

    ui->lineEdit->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_8->clear();
    ui->comboBox->currentText();
    ui->comboBox_2->currentText();
}


void ManageEmployeeWidget::on_pushButton_clicked()
{
    // Initially hide the input fields, labels, and Add Employee button
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->lineEdit_7->setVisible(false);
    ui->lineEdit_8->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    // Hiding the labels
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->label_4->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    emit backHome();
}

void ManageEmployeeWidget::on_addButton_clicked()
{

    // Toggle the visibility of the input fields, labels, and Add Employee button
    ui->tableWidget_employees->setVisible(false);
    ui->lineEdit->setVisible(true);
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_5->setVisible(true);
    ui->lineEdit_6->setVisible(true);
    ui->lineEdit_7->setVisible(true);
    ui->lineEdit_8->setVisible(true);
    ui->comboBox->setVisible(true);
    ui->comboBox_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    // Toggling the labels' visibility
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_5->setVisible(true);
    ui->label_6->setVisible(true);
    ui->label_7->setVisible(true);
    ui->label_8->setVisible(true);
    ui->label_9->setVisible(true);
    ui->label_10->setVisible(true);
    ui->pushButton_4->setVisible(false);
    ui->label_4->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->pushButton_6->setVisible(false);

    if(ui->comboBox->isVisible()){
        updateDepartmentComboBox();

        // For Testing
        updatePayGradeComboBox("Dummy Department");
    }
}

void ManageEmployeeWidget::on_pushButton_view_clicked() {
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->lineEdit_7->setVisible(false);
    ui->lineEdit_8->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    // Hiding the labels
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->label_4->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->tableWidget_employees->setVisible(true);
    // std::cout << "View button clicked for Employee!" << std::endl;

    // Clear any existing rows
    ui->tableWidget_employees->setRowCount(0);

    std::vector<std::map<std::string, std::string>> allEmployees = employee->getAllEmployees();

    // Check if any employees were fetched
    if (allEmployees.empty()) {
        std::cout << "No employees found in the database." << std::endl;
    } else {
        std::cout << "Number of employees found: " << allEmployees.size() << std::endl;
    }

    // Add new rows with the fetched data
    for (size_t row = 0; row < allEmployees.size(); ++row) {
        // Use `const auto&` to safely access the `std::map`
        const auto &details = allEmployees[row];
        ui->tableWidget_employees->insertRow(static_cast<int>(row));

        // Safely access elements in the const map using `find`
        auto setTableItem = [&](int column, const std::string &key) {
            auto it = details.find(key);
            if (it != details.end()) {
                ui->tableWidget_employees->setItem(static_cast<int>(row), column, new QTableWidgetItem(QString::fromStdString(it->second)));
            } else {
                ui->tableWidget_employees->setItem(static_cast<int>(row), column, new QTableWidgetItem(""));
            }
        };

        // Fill columns based on the keys expected
        setTableItem(0, "empId");
        setTableItem(1, "name");
        setTableItem(2, "dob");
        setTableItem(3, "doj");
        setTableItem(4, "mobileNo");
        setTableItem(5, "state");
        setTableItem(6, "city");
        setTableItem(7, "department");
        setTableItem(8, "grade_name");
    }

    // Adjust column sizes
    std::cout << "Adjusting column sizes to fit content." << std::endl;
    ui->tableWidget_employees->resizeColumnsToContents();
}
