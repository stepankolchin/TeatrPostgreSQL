#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <vhod_register.h>
#include <profile.h>
#include <schema_zal.h>
#include <mybutton.h>
#include <labelclicker.h>
#include <basket_view.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    vhod_register *registration;
    profile *my_profile;
    schema_zal *zal_form;
    basket_view *basket_form;

private slots:
    void on_pushButton_login_clicked();

    void checkUser(QString login,QString password);

    void addUser(QString name,QString surname,bool sex,QString login,QString pwd);

    void on_pushButton_register_clicked();

    void showPlaces(QString btn_name);

    void on_pushButton_profile_clicked();

    void on_pushButton_basket_clicked();

    void updateData(QString name,QString surname,QString sex,QString nick);

    void updatePwd(QString pwd);

    void reservePlace(int,int,int);

    void ticketDelete(int ticket_id);


private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlQuery *qw;
    int cur_user=0;
    bool mas_bool[20];
    QString kostil;

};
#endif // MAINWINDOW_H
