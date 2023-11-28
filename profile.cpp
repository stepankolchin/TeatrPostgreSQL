#include "profile.h"
#include "ui_profile.h"

profile::profile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::profile)
{
    ui->setupUi(this);
    ui->lineEdit_name->setReadOnly(true);
    ui->lineEdit_surname->setReadOnly(true);
    ui->lineEdit_sex->setReadOnly(true);
    ui->lineEdit_nick->setReadOnly(true);
    ui->lineEdit_pwd->setReadOnly(true);
    ui->pushButton_saveinf->hide();
    ui->pushButton_saveinf_savepwd->hide();
    this->setWindowTitle("Ваши личные данные");
}

profile::~profile(){
    delete ui;
}

void profile::setName(QString name){
    ui->lineEdit_name->setText(name);
}
void profile::setSurname(QString surname){
    ui->lineEdit_surname->setText(surname);
}
void profile::setSex(QString sex){
    ui->lineEdit_sex->setText(sex);
}

void profile::setNick(QString nick){
    ui->lineEdit_nick->setText(nick);
}
void profile::setPassword(QString pwd){
    ui->lineEdit_pwd->setText(pwd);
}

QString profile::getName(){
    return ui->lineEdit_name->text();
}

QString profile::getSurname(){
    return ui->lineEdit_surname->text();
}

QString profile::getSex(){
    return ui->lineEdit_sex->text();
}
QString profile::getNick(){
    return ui->lineEdit_nick->text();
}
QString profile::getPassword(){
    return ui->lineEdit_pwd->text();
}

void profile::on_pushButton_chinf_clicked(){
    ui->pushButton_chinf->hide();
    ui->pushButton_saveinf->show();
    ui->lineEdit_name->setReadOnly(false);
    ui->lineEdit_surname->setReadOnly(false);
    ui->lineEdit_sex->setReadOnly(false);
    ui->lineEdit_nick->setReadOnly(false);
}


void profile::on_pushButton_saveinf_clicked(){
    QString name=getName();
    QString surname=getSurname();
    QString sex=getSex();
    QString nick=getNick();
    ui->pushButton_chinf->show();
    ui->pushButton_saveinf->hide();
    ui->lineEdit_name->setReadOnly(true);
    ui->lineEdit_surname->setReadOnly(true);
    ui->lineEdit_sex->setReadOnly(true);
    ui->lineEdit_nick->setReadOnly(true);
    emit changeData(name,surname,sex,nick);
}

void profile::on_pushButton_chpwd_clicked(){
    ui->pushButton_chpwd->hide();
    ui->pushButton_saveinf_savepwd->show();
    ui->lineEdit_pwd->setReadOnly(false);
}

void profile::on_pushButton_saveinf_savepwd_clicked(){//
    ui->pushButton_chpwd->show();
    ui->pushButton_saveinf_savepwd->hide();
    QString pwd=getPassword();
    emit changePwd(pwd);
}

void profile::on_pushButton_shpwd_pressed(){//нажимаем кнопку показать пароль
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
}

void profile::on_pushButton_shpwd_released(){//отпускаем кнопку показать пароль
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
}
