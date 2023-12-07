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
#include <addshowdialogue.h>
#include <deleteshowdialogue.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    vhod_register *registration;//диалоговое окно для входа и регистрации

    profile *my_profile;//диалоговое окно, в котором можно просмотреть и изменить свои данные

    schema_zal *zal_form;//схема залов

    basket_view *basket_form;//диалоговое окно, отображающее корзину

    addShowDialogue *addShow_form;//диалоговое окно для добавления спектакля

    deleteShowDialogue *delShow_form;//диалоговое окно для удаления спектаклей


private slots:
    void on_pushButton_login_clicked();//кнопка залогиниться

    void checkUser(QString login,QString password);//функция проверяющая введенные данные при нажатии на кнопку войти

    void addUser(QString name,QString surname,bool sex,QString login,QString pwd);//функция, добавляющая нового пользователя в БД

    void on_pushButton_register_clicked();//кнопка зарегаться

    void showPlaces(QString btn_name);//функция, которая открывает схему зала

    void on_pushButton_profile_clicked();//кнопка профиль

    void on_pushButton_basket_clicked();//кнопка корзина

    void updateData(QString name,QString surname,QString sex,QString nick);//изменение личных данных

    void updatePwd(QString pwd);//изменение пароля

    void reservePlace(int,int,int);//резервация места

    void ticketDelete(int ticket_id);//удаление билета

    void setAdminInterface();//настройка интерфейса админа

    void on_pushButton_addShow_clicked();

    void on_pushButton_deleteShow_clicked();

    void addSpectacle(QString name,QString description,QString date,QString time,int zal);//добавление спектакля админом

    void delSpectacle(int);//удаление спектаклей админом

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;//указатель на объект базы данных
    QSqlQuery *qw;//указатель на объект запроса
    int cur_user=0;//идентификатор пользователя, вошедшего в системе
    bool mas_bool[20];//вспомогательный массив
    QString kostil;//используется для того чтобы на ходу изменять схему зала

};
#endif // MAINWINDOW_H
