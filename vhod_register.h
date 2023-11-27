#ifndef VHOD_REGISTER_H
#define VHOD_REGISTER_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class vhod_register;
}

class vhod_register : public QDialog
{
    Q_OBJECT

public:
    explicit vhod_register(QWidget *parent = nullptr);
    ~vhod_register();

    void clear();

    void setModeRegistration();

    void setModeLogin();

signals:
   void logining(QString l,QString p);

   void registration(QString,QString,bool,QString,QString);

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_clicked();

    void closeEvent(QCloseEvent*);

private:
    Ui::vhod_register *ui;
};

#endif // VHOD_REGISTER_H
