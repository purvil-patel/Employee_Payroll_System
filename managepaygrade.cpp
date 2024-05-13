#include "managepaygrade.h"
#include "ui_managepaygrade.h"
#include <iostream>
#include "src/pay_grade/PayGrade.h"
#include "src/database/Database.h"
#include <QMessageBox>

ManagePaygrade::ManagePaygrade(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManagePaygrade)
    , department(std::make_unique<Department>(Database::getInstance().getConn()))
    , payGrade(std::make_unique<PayGrade>(Database::getInstance().getConn())) // Initialize the dbConnection member
{
    ui->setupUi(this);

    // Log when the constructor is called
    std::cout << "ManagePaygrade constructor called." << std::endl;

    // Set up the headers for the QTableWidget
    ui->tableWidget_paygrades->setColumnCount(6);
    QStringList headers = {"Department", "Grade Name", "Basic", "DA", "TA", "Bonus"};
    ui->tableWidget_paygrades->setHorizontalHeaderLabels(headers);

    // Set table properties
    ui->tableWidget_paygrades->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_paygrades->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->lineEdit_name->setVisible(false);
    ui->lineEdit_basic->setVisible(false);
    ui->lineEdit_TA->setVisible(false);
    ui->lineEdit_DA->setVisible(false);
    ui->lineEdit_bonus->setVisible(false);

    ui->label_Department->setVisible(false);
    ui->label_grade->setVisible(false);
    ui->label_basic->setVisible(false);
    ui->label_TA->setVisible(false);
    ui->label_DA->setVisible(false);
    ui->label_bonus->setVisible(false);
    ui->pushButton_update_submit->setVisible(false);

    ui->comboBox_departmentlist->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_update_fetch->setVisible(false);

    ui->tableWidget_paygrades->setVisible(false);

    // Log connection setup
    std::cout << "Connecting 'View' button signal to slot." << std::endl;
    connect(ui->pushButton_6, &QPushButton::clicked, this, &ManagePaygrade::on_pushButton_view_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &ManagePaygrade::addPaygrade);
    connect(ui->pushButton_update, &QPushButton::clicked, this, &ManagePaygrade::on_pushButton_update_clicked);
    // connect(ui->pushButton_update_fetch, &QPushButton::clicked, this, &ManagePaygrade::on_pushButton_update_fetch_clicked);
    // connect(ui->pushButton_update_submit, &QPushButton::clicked, this, &ManagePaygrade::on_pushButton_update_submit_clicked);


}

ManagePaygrade::~ManagePaygrade()
{
    // Log when the destructor is called
    std::cout << "ManagePaygrade destructor called." << std::endl;
    delete ui;
}

void ManagePaygrade::on_pushButton_clicked()
{
    ui->lineEdit_name->setVisible(false);
    ui->lineEdit_basic->setVisible(false);
    ui->lineEdit_TA->setVisible(false);
    ui->lineEdit_DA->setVisible(false);
    ui->lineEdit_bonus->setVisible(false);

    ui->label_Department->setVisible(false);
    ui->label_grade->setVisible(false);
    ui->label_basic->setVisible(false);
    ui->label_TA->setVisible(false);
    ui->label_DA->setVisible(false);
    ui->label_bonus->setVisible(false);

    ui->comboBox_departmentlist->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_update_submit->setVisible(false);

    ui->tableWidget_paygrades->setVisible(false);
    ui->pushButton_update_fetch->setVisible(false);

    std::cout << "Home button clicked." << std::endl;
    emit backHome();
}

void ManagePaygrade::updateDepartmentComboBox() {
    std::cout << "Updating department combo box." << std::endl;
    ui->comboBox_departmentlist->clear(); // Clear existing items
    std::vector<std::string> departments = department->getAllDepartmentNames();

    if (departments.empty()) {
        std::cout << "No departments found in the database." << std::endl;
    } else {
        std::cout << "Departments found: " << departments.size() << std::endl;
        for (const std::string &name : departments) {
            std::cout << "Adding department to combo box: " << name << std::endl;
            ui->comboBox_departmentlist->addItem(QString::fromStdString(name));
        }
    }
}

void ManagePaygrade::addPaygrade() {
    std::cout << "Add pay grade button clicked." << std::endl;

    // Retrieve data from UI elements
    QString name = ui->lineEdit_name->text();
    QString basic = ui->lineEdit_basic->text();
    QString TA = ui->lineEdit_TA->text();
    QString DA = ui->lineEdit_DA->text();
    QString bonus = ui->lineEdit_bonus->text();
    QString department = ui->comboBox_departmentlist->currentText();

    // Log input values
    std::cout << "Input values: Name = " << name.toStdString()
              << ", Basic = " << basic.toStdString()
              << ", TA = " << TA.toStdString()
              << ", DA = " << DA.toStdString()
              << ", Bonus = " << bonus.toStdString()
              << ", Department = " << department.toStdString() << std::endl;

    // Call the backend function to add an employee
    if (!name.isEmpty() && !basic.isEmpty() && !TA.isEmpty()) {
        std::cout << "Adding pay grade to the database." << std::endl;
        payGrade->addPaygradeToDatabase(department.toStdString(), name.toStdString(), basic.toFloat(), DA.toFloat(),
                                        TA.toFloat(), bonus.toFloat());
    } else {
        std::cout << "Input validation failed, required fields are missing." << std::endl;
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
    }

    ui->lineEdit_name->clear();
    ui->lineEdit_basic->clear();
    ui->lineEdit_TA->clear();
    ui->lineEdit_DA->clear();
    ui->lineEdit_bonus->clear();


    ui->comboBox_departmentlist->clear();
}

void ManagePaygrade::on_pushButton_add_clicked() {
    std::cout << "Add button clicked!" << std::endl;


    // Toggle visibility
    ui->lineEdit_name->setVisible(true);
    ui->lineEdit_basic->setVisible(true);
    ui->lineEdit_TA->setVisible(true);
    ui->lineEdit_DA->setVisible(true);
    ui->lineEdit_bonus->setVisible(true);

    ui->label_Department->setVisible(true);
    ui->label_grade->setVisible(true);
    ui->label_basic->setVisible(true);
    ui->label_TA->setVisible(true);
    ui->label_DA->setVisible(true);
    ui->label_bonus->setVisible(true);

    ui->comboBox_departmentlist->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->tableWidget_paygrades->setVisible(false);
    ui->pushButton_update_submit->setVisible(false);
    ui->pushButton_update_fetch->setVisible(false);

    this->update();
    if(ui->comboBox_departmentlist->isVisible()){
        updateDepartmentComboBox();
    }
}

void ManagePaygrade::on_pushButton_view_clicked() {
    std::cout << "View button clicked!" << std::endl;

    // Hide input fields
    ui->lineEdit_name->setVisible(false);
    ui->lineEdit_basic->setVisible(false);
    ui->lineEdit_TA->setVisible(false);
    ui->lineEdit_DA->setVisible(false);
    ui->lineEdit_bonus->setVisible(false);

    ui->label_Department->setVisible(false);
    ui->label_grade->setVisible(false);
    ui->label_basic->setVisible(false);
    ui->label_TA->setVisible(false);
    ui->label_DA->setVisible(false);
    ui->label_bonus->setVisible(false);

    ui->comboBox_departmentlist->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_update_submit->setVisible(false);
    ui->pushButton_update_fetch->setVisible(false);

    // Show the table widget
    std::cout << "Making the table visible." << std::endl;
    ui->tableWidget_paygrades->setVisible(true);

    // Clear any existing rows
    ui->tableWidget_paygrades->setRowCount(0);

    std::cout << "Fetching pay grades from the backend." << std::endl;
    std::vector<PayGradeDetail> payGrades = payGrade->listPayGradesByDepartment("all");

    // Check if any pay grades were fetched
    if (payGrades.empty()) {
        std::cout << "No pay grades found in the database." << std::endl;
    } else {
        std::cout << "Number of pay grades found: " << payGrades.size() << std::endl;
    }

    // Add new rows with the fetched data
    for (size_t row = 0; row < payGrades.size(); ++row) {
        const auto &detail = payGrades[row];
        std::cout << "Adding row for department: " << detail.department_name
                  << ", grade name: " << detail.grade_name << std::endl;

        ui->tableWidget_paygrades->insertRow(static_cast<int>(row));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 0, new QTableWidgetItem(QString::fromStdString(detail.department_name)));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 1, new QTableWidgetItem(QString::fromStdString(detail.grade_name)));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 2, new QTableWidgetItem(QString::number(detail.basic_pay)));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 3, new QTableWidgetItem(QString::number(detail.dearness_allowance)));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 4, new QTableWidgetItem(QString::number(detail.travel_allowance)));
        ui->tableWidget_paygrades->setItem(static_cast<int>(row), 5, new QTableWidgetItem(QString::number(detail.bonus)));
    }

    // Adjust column sizes
    std::cout << "Adjusting column sizes to fit content." << std::endl;
    ui->tableWidget_paygrades->resizeColumnsToContents();
}

void ManagePaygrade::on_pushButton_update_clicked()
{
    std::cout << "Update button clicked!" << std::endl;
    ui->lineEdit_name->setVisible(true);
    ui->lineEdit_basic->setVisible(false);
    ui->lineEdit_TA->setVisible(false);
    ui->lineEdit_DA->setVisible(false);
    ui->lineEdit_bonus->setVisible(false);

    ui->label_Department->setVisible(false);
    ui->label_grade->setVisible(true);
    ui->label_basic->setVisible(false);
    ui->label_TA->setVisible(false);
    ui->label_DA->setVisible(false);
    ui->label_bonus->setVisible(false);

    ui->comboBox_departmentlist->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_update_fetch->setVisible(true);
    ui->pushButton_update_submit->setVisible(false);
    ui->tableWidget_paygrades->setVisible(false);
}

void ManagePaygrade::on_pushButton_update_fetch_clicked() {
    std::string gradeName = ui->lineEdit_name->text().toStdString();

    // Fetch the paygrade details
    PayGradeDetail detail = payGrade->getPayGradeDetailsByName(gradeName);

    // Check if the paygrade was found and update the UI accordingly
    if (!detail.grade_name.empty()) {
        ui->lineEdit_basic->setText(QString::number(detail.basic_pay));
        ui->lineEdit_DA->setText(QString::number(detail.dearness_allowance));
        ui->lineEdit_TA->setText(QString::number(detail.travel_allowance));
        ui->lineEdit_bonus->setText(QString::number(detail.bonus));

        // Make input fields and labels visible for editing
        ui->lineEdit_basic->setVisible(true);
        ui->lineEdit_DA->setVisible(true);
        ui->lineEdit_TA->setVisible(true);
        ui->lineEdit_bonus->setVisible(true);
        ui->label_basic->setVisible(true);
        ui->label_DA->setVisible(true);
        ui->label_TA->setVisible(true);
        ui->label_bonus->setVisible(true);
        ui->pushButton_update_fetch->setVisible(false);

        // Enable the submit button
        std::cout << "Fetch button clicked!" << std::endl;

        ui->pushButton_update_submit->setVisible(true);
    } else {
        QMessageBox::warning(this, "Not Found", "Paygrade not found. Please check the name and try again.");
    }
}

void ManagePaygrade::on_pushButton_update_submit_clicked() {
    std::string gradeName = ui->lineEdit_name->text().toStdString();
    float basic = ui->lineEdit_basic->text().toFloat();
    float da = ui->lineEdit_DA->text().toFloat();
    float ta = ui->lineEdit_TA->text().toFloat();
    float bonus = ui->lineEdit_bonus->text().toFloat();

    // Call the backend function to update the paygrade
    payGrade->updatePayGradeDetails(gradeName, basic, da, ta, bonus);

    QMessageBox::information(this, "Updated", "Paygrade details updated successfully.");

    ui->lineEdit_name->clear();
    ui->lineEdit_basic->clear();
    ui->lineEdit_DA->clear();
    ui->lineEdit_TA->clear();
    ui->lineEdit_bonus->clear();

    ui->lineEdit_name->setVisible(true);
    ui->lineEdit_basic->setVisible(false);
    ui->lineEdit_TA->setVisible(false);
    ui->lineEdit_DA->setVisible(false);
    ui->lineEdit_bonus->setVisible(false);

    ui->label_Department->setVisible(false);
    ui->label_grade->setVisible(true);
    ui->label_basic->setVisible(false);
    ui->label_TA->setVisible(false);
    ui->label_DA->setVisible(false);
    ui->label_bonus->setVisible(false);

    ui->comboBox_departmentlist->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_update_fetch->setVisible(true);
    ui->pushButton_update_submit->setVisible(false);
    ui->tableWidget_paygrades->setVisible(false);
}

