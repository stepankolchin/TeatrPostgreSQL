#include "vhod_register.h"
#include "ui_vhod_register.h"

vhod_register::vhod_register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vhod_register)
{
    ui->setupUi(this);
}

vhod_register::~vhod_register()
{
    delete ui;
}

void vhod_register::on_pushButton_login_clicked()
{
    if (ui->lineEdit_login->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Логин' не должно быть пустым!",QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_pwd->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Пароль' не должно быть пустым!",QMessageBox::Ok);
        return;
    }
    QString login=ui->lineEdit_login->text();
    QString pwd=ui->lineEdit_pwd->text();
    emit logining(login,pwd);
}


void vhod_register::on_pushButton_close_clicked()
{
    clear();
    this->close();
}


void vhod_register::on_pushButton_clicked()
{
    if (ui->lineEdit_Name->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Имя' не должно быть пустым!",QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_Surname->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Фамилия' не должно быть пустым!",QMessageBox::Ok);
        return;
    }


    if (ui->lineEdit_new_login->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Логин' не должно быть пустым!",QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_new_pwd->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Поле 'Пароль' не должно быть пустым!",QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_new_pwd_2->text().isEmpty()){
        QMessageBox::warning(this,"Внимание","Оба поля 'Пароль' не должны быть пустыми!",QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_new_pwd->text()!=ui->lineEdit_new_pwd_2->text()){
        QMessageBox::warning(this,"Внимание","Пароли не совпадают!",QMessageBox::Ok);
        return;
    }
    QString name=ui->lineEdit_Name->text();
    QString surname=ui->lineEdit_Surname->text();
    bool sex=ui->radioButton->isChecked();
    QString login=ui->lineEdit_new_login->text();
    QString pwd=ui->lineEdit_new_pwd->text();

    emit registration(name,surname,sex,login,pwd);
}

void vhod_register::clear(){
    ui->lineEdit_Name->clear();
    ui->lineEdit_Surname->clear();
    ui->lineEdit_login->clear();
    ui->lineEdit_new_login->clear();
    ui->lineEdit_new_pwd->clear();
    ui->lineEdit_new_pwd_2->clear();
    ui->lineEdit_pwd->clear();


}
void vhod_register::closeEvent(QCloseEvent *event){
    clear();
    event->accept();
}
void vhod_register::setModeRegistration(){
    ui->lineEdit_login->hide();
    ui->lineEdit_pwd->hide();
    ui->pushButton_login->hide();
    ui->lineEdit_Name->show();
    ui->lineEdit_Surname->show();
    ui->lineEdit_new_login->show();
    ui->lineEdit_new_pwd->show();
    ui->lineEdit_new_pwd_2->show();
    ui->radioButton->show();
    ui->radioButton_2->show();
    ui->pushButton->show();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
}

void vhod_register::setModeLogin(){
    ui->lineEdit_login->show();
    ui->lineEdit_pwd->show();
    ui->pushButton_login->show();
    ui->lineEdit_Name->hide();
    ui->lineEdit_Surname->hide();
    ui->lineEdit_new_login->hide();
    ui->lineEdit_new_pwd->hide();
    ui->lineEdit_new_pwd_2->hide();
    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->pushButton->hide();

    ui->label->show();
    ui->label_2->show();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
}
