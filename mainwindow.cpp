#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db=nullptr;

    if (db!=nullptr){
        db->close();
        QMessageBox::information(this,"Внимание","База данных не открыта",QMessageBox::Ok);

    }
    else{
        db= new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
        QString dbname="bd_for_i";
        QString host="127.0.0.1";
        QString user="postgres";
        QString pwd="igra777";

        db->setDatabaseName(dbname);
        db->setHostName(host);
        db->setUserName(user);
        db->setPassword(pwd);
        db->setPort(5432);
        qw=new QSqlQuery(*db);
        for (int i=0;i<20;i++){
            mas_bool[i]=false;
        }


    }

    if (!db->open()){
        QMessageBox::information(this,"Ошибка",db->lastError().text(),QMessageBox::Ok);
        db=nullptr;

    }
    if (db->isOpen()){
        *qw=db->exec("SELECT show_name,show_description,show_date,show_time,zal_id FROM public.show s\n");

        ui->tableWidget->setRowCount(qw->size());
        ui->tableWidget->setColumnCount(5);
        QStringList list={"Название спектакля","Описание","Дата","Время","Номер зала"};
        ui->tableWidget->setHorizontalHeaderLabels(list);
        for(int i=0;i<ui->tableWidget->rowCount();i++)
            for (int j=0;j<ui->tableWidget->columnCount();j++)
                if (ui->tableWidget->item(i,j)==nullptr)
                {
                        QTableWidgetItem * ti;
                        ti = new QTableWidgetItem;
                        ui->tableWidget->setItem(i, j, ti);
                }

        int j=0;
        while(qw->next()){
            int i=0;
            while(!qw->value(i).isNull()){
                if (i==3){
                    QString time=qw->value(i).toString();
                    time.remove(4,7);
                    ui->tableWidget->item(j,i)->setText(time);
                    i++;
                    continue;
                }
                ui->tableWidget->item(j,i)->setText(qw->value(i).toString());
                i++;
            }

            j++;
        }

    }
    registration=new vhod_register(this);
    my_profile=new profile(this);
    zal_form=new schema_zal(this);
    basket_form=new basket_view(this);
    connect(zal_form,SIGNAL(placeReserved(int,int)),this,SLOT(reservePlace(int,int)));
    connect(basket_form,SIGNAL(delTicket(int)),this,SLOT(ticketDelete(int)));
    connect(registration,SIGNAL(logining(QString,QString)),this,SLOT(checkUser(QString,QString)));
    connect(registration,SIGNAL(registration(QString,QString,bool,QString,QString)),this,SLOT(addUser(QString,QString,bool,QString,QString)));
    connect(my_profile,SIGNAL(changeData(QString,QString,QString,QString)),this,SLOT(updateData(QString,QString,QString,QString)));
    connect(my_profile,SIGNAL(changePwd(QString)),this,SLOT(updatePwd(QString)));

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->pushButton_basket->hide();
    ui->pushButton_profile->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete registration;
    delete my_profile;
    delete zal_form;
}


void MainWindow::on_pushButton_login_clicked()
{
    if (ui->pushButton_login->text()=="Войти"){
        registration->setModeLogin();
        registration->exec();
    }
    else{
        cur_user=0;
        ui->pushButton_login->setText("Войти");
        ui->label_2->show();
        ui->pushButton_register->show();
        ui->pushButton_basket->hide();
        ui->pushButton_profile->hide();

    }
}

void MainWindow::checkUser(QString login,QString password){
    if (db->isOpen()){
        *qw=db->exec("SELECT customer_name,customer_surname,customer_login,customer_password,customer_id FROM customer WHERE customer_login='"+login+"'");
        qw->next();
        if (!qw->value(0).isNull()){
            if (qw->value(3).toString()==password){
                QMessageBox::information(registration,"Ура","Вход выполнен!");
                registration->close();
                ui->tableWidget->show();
                ui->label->show();
//                ui->label_Cur_user->show();
                cur_user=qw->value(4).toInt();
//                ui->label_Cur_user->setText("Вы зашли в систему под именем " + qw->value(0).toString()+ " "+qw->value(1).toString());
                ui->pushButton_login->setText("Выйти");
                ui->label_2->hide();
                ui->pushButton_register->hide();
                ui->pushButton_profile->show();
                ui->pushButton_basket->show();
                ui->pushButton_basket->setText("Корзина (0)");
                ui->tableWidget->insertColumn(5);
                for(int i=0;i<ui->tableWidget->rowCount();i++){
                    for (int k=0;k<20;k++){
                        if (mas_bool[k]){
                            continue;
                        }
                        else{
                        mybutton *buy=new mybutton(this);
                        buy->setText("Места");
                        QString str;
                        str.setNum(k);
                        buy->setWindowTitle("btn_buy"+str);
                        qDebug()<<"btn_buy"+str;
                        connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));

                        ui->tableWidget->setCellWidget(i,5,buy);
                        mas_bool[k]=true;
                        break;
                        }
                    }
                }
            }
            else{
                QMessageBox::warning(registration,"Внимание","Неправильный пароль");

            }
        }
        else{
            QMessageBox::warning(registration,"Внимание","Неправильная почта");

        }
    }
}

void MainWindow::addUser(QString name,QString surname,bool sex,QString login,QString pwd){
    if (db->isOpen()){
        qw->prepare("INSERT INTO customer (customer_name,customer_surname,customer_male,customer_login,customer_password)\n"
                    "VALUES(:name,:surname,:sex,:login,:pwd)");
        qw->bindValue(":name",name);
        qw->bindValue(":surname",surname);
        qw->bindValue(":sex",sex);
        qw->bindValue(":login",login);
        qw->bindValue(":pwd",pwd);
        if (qw->exec()){
            QMessageBox::information(registration,"Поздравляем","Вы успешно зарегистрировались и вошли!");
            *qw=db->exec("SELECT customer_name,customer_surname,customer_login,customer_password,customer_id FROM customer WHERE customer_login='"+login+"'");
            qw->next();
            cur_user=qw->value(4).toInt();
            ui->pushButton_register->hide();
            ui->pushButton_login->setText("Выйти");
            ui->label->hide();
            ui->pushButton_basket->show();
            ui->pushButton_profile->show();
            ui->tableWidget->insertColumn(5);
            for(int i=0;i<ui->tableWidget->rowCount();i++){
                for (int k=0;k<20;k++){
                    if (mas_bool[k]){
                        continue;
                    }
                    else{
                    mybutton *buy=new mybutton(this);
                    buy->setText("Места");
                    QString str;
                    str.setNum(k);
                    buy->setWindowTitle("btn_buy"+str);
                    qDebug()<<"btn_buy"+str;
                    connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));

                    ui->tableWidget->setCellWidget(i,5,buy);
                    mas_bool[k]=true;
                    break;
                    }
                }
            }
        }
        else{
            QMessageBox::warning(registration,"О нет","Что-то пошло не так");

        }
    }
}

void MainWindow::on_pushButton_register_clicked()
{
    registration->setModeRegistration();
    registration->exec();

}

void MainWindow::showPlaces(QString num_of_btn){
    if (db->isOpen()){
        bool mas[30];
        int num=num_of_btn.last(1).toInt();
        int zal=ui->tableWidget->item(num,4)->text().toInt();
        qw->prepare("SELECT place_free FROM place\n"
                    "WHERE zal_id=" + QString::number(zal));
        if (qw->exec()){
            qw->next();
            for(int i=0;i<30;i++){
                mas[i]=qw->value(0).toBool();
                qw->next();
            }
        }
        zal_form->setMassiv(mas);
        zal_form->setZal(zal);
        zal_form->open();

    }
}

void MainWindow::on_pushButton_profile_clicked(){
    if (db->isOpen()){
        if (qw->exec("SELECT customer_name,customer_surname,customer_male,customer_login,customer_password FROM customer\n"
                     "WHERE customer_id="+QString::number(cur_user))){
            qw->next();
            my_profile->setName(qw->value(0).toString());
            my_profile->setSurname(qw->value(1).toString());
            if (qw->value(2).toBool())
                my_profile->setSex("Мужчина");
            else
                my_profile->setSex("Женщина");
            my_profile->setNick(qw->value(3).toString());
            my_profile->setPassword(qw->value(4).toString());
            my_profile->exec();
        }
        else
            QMessageBox::warning(my_profile,"Упс","Что-то пошло не по плану");
    }
}
void MainWindow::updateData(QString name,QString surname,QString sex,QString nick){
    if (db->isOpen()){
        qw->prepare("UPDATE customer\n"
                    "SET customer_name=:name,\n"
                    "customer_surname=:surname,\n"
                    "customer_male=:sex,\n"
                    "customer_login=:nick\n"
                    "WHERE customer_id="+QString::number(cur_user));
        qw->bindValue(":name",name);
        qw->bindValue(":surname",surname);
        if (sex=="Мужчина")
            sex="true";
        else
            sex="false";
        qw->bindValue(":sex",sex);
        qw->bindValue(":nick",nick);
        if (qw->exec()){
            QMessageBox::information(my_profile,"Отлично","Персональные данные были обновлены");
        }
        else
            QMessageBox::warning(my_profile,"о нет","Что-то пошло не так");
    }
}

void MainWindow::updatePwd(QString pwd){
    if (db->isOpen()){
        qw->prepare("UPDATE customer\n"
                    "SET customer_password=:pwd\n"
                    "WHERE customer_id="+QString::number(cur_user));
        qw->bindValue(":pwd",pwd);
        if (qw->exec()){
            QMessageBox::information(my_profile,"Отлично","Персональные данные были обновлены");
        }
        else
            QMessageBox::warning(my_profile,"о нет","Что-то пошло не так");
    }
}




void MainWindow::on_pushButton_basket_clicked()
{
    if (db->isOpen()){
        qw->prepare("SELECT t.ticket_id,p.place_row,p.place_num,s.show_name,p.zal_id,s.show_date,s.show_time FROM public.basket_tickets b\n"
                    "JOIN public.ticket t ON t.ticket_id=b.ticket_id\n"
                    "JOIN public.place p ON p.place_id=t.place_id\n"
                    "JOIN public.show s ON s.show_id=t.show_id\n"
                    "WHERE t.customer_id="+QString::number(cur_user)+"\n"
                    "ORDER BY t.ticket_id ASC");
        if (qw->exec()){
            basket_form->setBasket(qw);
            basket_form->exec();
        }
        else
            QMessageBox::warning(this,"о нет","Чтото пошло не так и блаблабла");

    }
}

void MainWindow::ticketDelete(int ticket_id){
    if (db->isOpen()){
        qw->prepare("UPDATE place\n"
                    "SET place_free=true\n"
                    "WHERE place_id=(SELECT place_id FROM ticket WHERE ticket_id=:t_id)");
        qw->bindValue(":t_id",ticket_id);
        if (qw->exec()){
            QMessageBox::information(basket_form,"Сработало","СРАБОТАЛО");
        }
        else return;
        qw->prepare("DELETE FROM public.basket_tickets t\n"
                    "USING public.basket b\n"
                    "WHERE b.basket_id=t.basket_id AND t.ticket_id=:t_id AND b.customer_id="+QString::number(cur_user));
        qw->bindValue(":t_id",ticket_id);
        if (qw->exec()){
            QMessageBox::information(basket_form,"Хорошо","Билет был удален");
            qw->prepare("SELECT t.ticket_id,p.place_row,p.place_num,s.show_name,p.zal_id,s.show_date,s.show_time FROM public.basket_tickets b\n"
                        "JOIN public.ticket t ON t.ticket_id=b.ticket_id\n"
                        "JOIN public.place p ON p.place_id=t.place_id\n"
                        "JOIN public.show s ON s.show_id=t.show_id\n"
                        "WHERE t.customer_id="+QString::number(cur_user)+"\n"
                        "ORDER BY t.ticket_id ASC");
            if (qw->exec()){
                basket_form->setBasket(qw);
//                QMessageBox::information(basket_form,"Ура","Билет был уда");
            }
        }else
            QMessageBox::warning(basket_form,"О нет...","Что-то пошло не так и билет не был удален");
    }

}
void MainWindow::reservePlace(int num_place,int num_zal){

}
