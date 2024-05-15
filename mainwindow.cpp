#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "src/database/Database.h"
#include <string>
#include "src/user_authentication/Login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(&dashboardwidget);

    ui->stackedWidget->setCurrentIndex(0);

    db = Database::getInstance().getConn();

    connect(&dashboardwidget,SIGNAL(logout()),this,SLOT(logout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    Login login(db);
    std::string username = ui->usernameText->text().toStdString();
    std::string password = ui->passwordText->text().toStdString();

    ui->usernameText->clear();
    ui->passwordText->clear();



    if(login.authenticate(username, password)){
        //ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget->setCurrentWidget(&dashboardwidget);
    }else{
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
    }

}


void MainWindow::logout()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                                  QMessageBox::Yes|QMessageBox::No);

    // Check the user's response to the QMessageBox
    if (reply == QMessageBox::Yes) {
        // User clicked "Yes"
        ui->stackedWidget->setCurrentIndex(0); // Assuming index 0 is the login page
    } else {
        return;
    }
}

