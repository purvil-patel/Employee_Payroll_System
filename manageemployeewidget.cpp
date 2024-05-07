#include "manageemployeewidget.h"
#include "ui_manageemployeewidget.h"

ManageEmployeeWidget::ManageEmployeeWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ManageEmployeeWidget)
{
    ui->setupUi(this);

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
}

ManageEmployeeWidget::~ManageEmployeeWidget()
{
    delete ui;
}

void ManageEmployeeWidget::on_pushButton_clicked()
{
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
}
