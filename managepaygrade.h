#ifndef MANAGEPAYGRADE_H
#define MANAGEPAYGRADE_H

#include <QWidget>

namespace Ui {
class ManagePaygrade;
}

class ManagePaygrade : public QWidget
{
    Q_OBJECT

public:
    explicit ManagePaygrade(QWidget *parent = nullptr);
    ~ManagePaygrade();

private slots:
    void on_pushButton_clicked();

signals:
    void backHome();

private:
    Ui::ManagePaygrade *ui;
};

#endif // MANAGEPAYGRADE_H
