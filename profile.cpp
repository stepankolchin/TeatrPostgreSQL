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
    ui->lineEdit_name->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_surname->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_sex->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_nick->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_pwd->setFocusPolicy(Qt::NoFocus);
    ui->label_6->hide();
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
    ui->lineEdit_name->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_surname->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_sex->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_nick->setFocusPolicy(Qt::StrongFocus);
    ui->label_6->show();
    ui->label_6->setText(ui->label_6->text()+" логина");
    ui->pushButton_chpwd->hide();
//    ui->lineEdit_pwd->setFocusPolicy(Qt::StrongFocus);
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
    ui->lineEdit_name->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_surname->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_sex->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_nick->setFocusPolicy(Qt::NoFocus);
    ui->label_6->hide();
    ui->label_6->setText(ui->label_6->text().left(37));
    ui->pushButton_chpwd->show();
    //    ui->lineEdit_pwd->setFocusPolicy(Qt::NoFocus);
    emit changeData(name,surname,sex,nick);
}

void profile::on_pushButton_chpwd_clicked(){
    ui->pushButton_chpwd->hide();
    ui->pushButton_saveinf_savepwd->show();
    ui->lineEdit_pwd->setReadOnly(false);
    ui->label_6->show();
    ui->label_6->setText(ui->label_6->text()+" пароля");
    ui->pushButton_chinf->hide();
//    ui->lineEdit_name->setFocusPolicy(Qt::StrongFocus);
//    ui->lineEdit_surname->setFocusPolicy(Qt::StrongFocus);
//    ui->lineEdit_sex->setFocusPolicy(Qt::StrongFocus);
//    ui->lineEdit_nick->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_pwd->setFocusPolicy(Qt::StrongFocus);
}

void profile::on_pushButton_saveinf_savepwd_clicked(){//
    ui->pushButton_chpwd->show();
    ui->pushButton_saveinf_savepwd->hide();
    QString pwd=getPassword();
    ui->lineEdit_pwd->setFocusPolicy(Qt::NoFocus);
    ui->label_6->hide();
    ui->label_6->setText(ui->label_6->text().left(37));
    ui->pushButton_chinf->show();
    emit changePwd(pwd);
}

void profile::on_pushButton_shpwd_pressed(){//нажимаем кнопку показать пароль
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
}

void profile::on_pushButton_shpwd_released(){//отпускаем кнопку показать пароль
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
}
