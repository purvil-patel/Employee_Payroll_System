#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dashboardwidget.h"
#include <sqlite3.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void logout();

private:
    Ui::MainWindow *ui;
    DashboardWidget dashboardwidget;
    sqlite3* db;
};
#endif // MAINWINDOW_H
