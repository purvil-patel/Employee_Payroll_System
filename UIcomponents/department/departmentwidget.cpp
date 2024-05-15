#include "departmentwidget.h"
#include "ui_departmentwidget.h"
#include "../../src/database/Database.h"
#include <QMessageBox>

Departmentwidget::Departmentwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Departmentwidget)
    , model(new QStandardItemModel(this))
    , department(std::make_unique<Department>(Database::getInstance().getConn()))
{
    ui->setupUi(this);

    ui->listView->setModel(model);

    std::vector<std::string> departments = department->getAllDepartmentNames();

    for (const auto& deptName : departments) {
        QStandardItem *item = new QStandardItem(QString::fromStdString(deptName));
        model->appendRow(item);

    }

    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->submitNewDepartmentButton->setEnabled(false);

    connect(ui->newDepartmentNameText, &QLineEdit::textChanged,this, &Departmentwidget::checkInput);


}

Departmentwidget::~Departmentwidget()
{
    delete ui;
}

void Departmentwidget::on_pushButton_clicked()
{
    emit backHome();
}

void Departmentwidget::checkInput(const QString &text)
{
    // Enable the button only if the text is not empty
    ui->submitNewDepartmentButton->setEnabled(!text.isEmpty());
}

void Departmentwidget::on_submitNewDepartmentButton_clicked()
{

    std::string newDepartmentName =ui->newDepartmentNameText->text().toStdString();

    auto result = department->addDepartment(newDepartmentName);

    if (result.first) {
        QMessageBox::information(this, "Success", QString::fromStdString(result.second));
        ui->newDepartmentNameText->clear();

        QStandardItem *newItem = new QStandardItem(QString::fromStdString(newDepartmentName));
        model->appendRow(newItem);

        model->sort(0);
    } else {
        QMessageBox::warning(this, "Failure", QString::fromStdString(result.second));
    }


}

