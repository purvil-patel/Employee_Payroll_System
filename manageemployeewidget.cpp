#include "manageemployeewidget.h"
#include "ui_manageemployeewidget.h"
#include "src/database/Database.h"
#include <QMessageBox>

ManageEmployeeWidget::ManageEmployeeWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ManageEmployeeWidget)
    ,employee(std::make_unique<Employee>(Database::getInstance().getConn()))
    ,department(std::make_unique<Department>(Database::getInstance().getConn()))
, payGrade(std::make_unique<PayGrade>(Database::getInstance().getConn()))
{
    ui->setupUi(this);

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

    // Connect the Add button's clicked signal to the slot
    connect(ui->pushButton_2, &QPushButton::clicked, this, &ManageEmployeeWidget::on_addButton_clicked);

    //connect(ui->comboBox, &QComboBox::highlighted, this, &ManageEmployeeWidget::updateDepartmentComboBox);
    //connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updatePayGradeComboBox(QString)));
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &ManageEmployeeWidget::updatePayGradeComboBox);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &ManageEmployeeWidget::addEmployee);

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

// Dummy Implementation for testing

// void ManageEmployeeWidget::updatePayGradeComboBox(const QString &departmentName) {
//     ui->comboBox_2->clear(); // Clear existing items

//     // Hardcoded grade names for testing the dropdown functionality
//     std::vector<std::string> gradeNames = {
//         "Junior Developer",
//         "Senior Developer",
//         "Project Manager",
//         "Quality Assurance",
//         "User Experience Designer"
//     };

//     // Populate the ComboBox with hardcoded grade names
//     for (const std::string &name : gradeNames) {
//         ui->comboBox_2->addItem(QString::fromStdString(name));
//     }
// }

// Actual Implementation

void ManageEmployeeWidget::updatePayGradeComboBox(const QString &departmentName) {
    ui->comboBox_2->clear(); // Clear existing items
    qDebug() << "Department selected:" << departmentName;
    // Fetch the details of pay grades based on the selected department
    std::vector<PayGradeDetail> payGrades = payGrade->listPayGradesByDepartment(departmentName.toStdString());
    std::vector<std::string> gradeNames;

    // gradeNames = {
    //     "Junior Developer",
    //     "Senior Developer",
    //     "Project Manager",
    //     "Quality Assurance",
    //     "User Experience Designer"
    // };

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
    emit backHome();
}

void ManageEmployeeWidget::on_addButton_clicked()
{

    // Toggle the visibility of the input fields, labels, and Add Employee button
    bool isVisible = ui->lineEdit->isVisible();
    ui->lineEdit->setVisible(!isVisible);
    ui->lineEdit_2->setVisible(!isVisible);
    ui->lineEdit_5->setVisible(!isVisible);
    ui->lineEdit_6->setVisible(!isVisible);
    ui->lineEdit_7->setVisible(!isVisible);
    ui->lineEdit_8->setVisible(!isVisible);
    ui->comboBox->setVisible(!isVisible);
    ui->comboBox_2->setVisible(!isVisible);
    ui->pushButton_3->setVisible(!isVisible);
    // Toggling the labels' visibility
    ui->label_2->setVisible(!isVisible);
    ui->label_3->setVisible(!isVisible);
    ui->label_5->setVisible(!isVisible);
    ui->label_6->setVisible(!isVisible);
    ui->label_7->setVisible(!isVisible);
    ui->label_8->setVisible(!isVisible);
    ui->label_9->setVisible(!isVisible);
    ui->label_10->setVisible(!isVisible);
    if(ui->comboBox->isVisible()){
        updateDepartmentComboBox();

        // For Testing
        updatePayGradeComboBox("Dummy Department");
    }
}
