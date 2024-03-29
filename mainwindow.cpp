#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)//конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db=nullptr;
    if (db!=nullptr){//
        db->close();
        QMessageBox::information(this,"Внимание","База данных не открыта",QMessageBox::Ok);
    }
    else{//создаем и подключаем базу данных
        db= new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
        QString dbname="last_db";
        QString host="127.0.0.1";
        QString user="postgres";
        QString pwd="igra777";

        db->setDatabaseName(dbname);
        db->setHostName(host);
        db->setUserName(user);
        db->setPassword(pwd);
        db->setPort(5432);
        qw=new QSqlQuery(*db);//выделяем память для объекта запроса
        for (int i=0;i<20;i++)//инициализация вспомогательного массива нулями
            mas_bool[i]=false;
    }
    if (!db->open()){//если БД не открылась
        QMessageBox::information(this,"Ошибка",db->lastError().text(),QMessageBox::Ok);
        db=nullptr;
    }
    if (db->isOpen()){//если открылась - выводим начальные спектакли
        *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                     "ORDER BY show_date,show_time");//выбираем нужные поля из таблицы спектаклей
        ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
        ui->tableWidget->setColumnCount(7);
        QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
        ui->tableWidget->setHorizontalHeaderLabels(list);
        for(int i=0;i<ui->tableWidget->rowCount();i++)
            for (int j=0;j<ui->tableWidget->columnCount();j++)
                if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
                {
                    QTableWidgetItem * ti;
                    ti = new QTableWidgetItem;
                    ui->tableWidget->setItem(i, j, ti);
                }
        int j=0;//индекс для первого цикла
        while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
            int i=0;//индекс для второго цикла
            while(i<6){//всего 6 полей в запросе

                if (i==4){//если это время - убираем секунды и доли секунд
                    QString time=qw->value(i).toString();
                    time.remove(4,7);
                    ui->tableWidget->item(j,i)->setText(time);
                    i++;
                    continue;
                }

                ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
                if (i!=2){
                    ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                    ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
                }
                i++;//шаг
            }
            if (i==6)//для поля с кнопкой
                for (int k=0;k<20;k++){//максимум можно 20 шоу
                    if (mas_bool[k])//для нумерации кнопок
                        continue;
                    else{
                        mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                        buy->setText("Места");//надпись на кнопке
                        QString str;
                        str.setNum(k);
                        buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                        connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                        ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                        mas_bool[k]=true;
                        break;
                    }
                }
            j++;//шаг
        }
    }
    for (int i=0;i<20;i++)
        mas_bool[i]=false;
    ui->tableWidget->hideColumn(6);//прячем столбец с кнопками
    //память для форм
    registration=new vhod_register(this);
    my_profile=new profile(this);
    zal_form=new schema_zal(this);
    basket_form=new basket_view(this);
    addShow_form=new addShowDialogue(this);
    delShow_form=new deleteShowDialogue(this);
    //связываем сигналы форм со слотами
    //необходимо потому, что все запросы выполняются здесь
    connect(delShow_form,SIGNAL(deleteShow(int)),this,SLOT(delSpectacle(int)));
    connect(addShow_form,SIGNAL(changeShow(QString,QString,QString,QString,int)),this,SLOT(showChange(QString,QString,QString,QString,int)));
    connect(addShow_form,SIGNAL(addShow(QString,QString,QString,QString,int)),this,SLOT(addSpectacle(QString,QString,QString,QString,int)));
    connect(zal_form,SIGNAL(placeReserved(int,int,int)),this,SLOT(reservePlace(int,int,int)));
    connect(basket_form,SIGNAL(delTicket(int)),this,SLOT(ticketDelete(int)));
    connect(registration,SIGNAL(logining(QString,QString)),this,SLOT(checkUser(QString,QString)));
    connect(registration,SIGNAL(registration(QString,QString,bool,QString,QString)),this,SLOT(addUser(QString,QString,bool,QString,QString)));
    connect(my_profile,SIGNAL(changeData(QString,QString,QString,QString)),this,SLOT(updateData(QString,QString,QString,QString)));
    connect(my_profile,SIGNAL(changePwd(QString)),this,SLOT(updatePwd(QString)));

    //настройка интерфейса
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->pushButton_basket->hide();
    ui->pushButton_profile->hide();
    ui->label_admin->hide();
    ui->pushButton_addShow->hide();
    ui->pushButton_deleteShow->hide();
    ui->pushButton_redact->hide();
    ui->spinBox->hide();
    ui->label_3->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->setWindowTitle("Театр для не черни толпы");
//    this->setWindowIcon(QIcon("E:\\baza_d\\iri\\teatrr\\teatr\\mainkartink.jpg"));
}

MainWindow::~MainWindow()//деструктор
{
    delete ui;
    delete registration;
    delete my_profile;
    delete zal_form;
    delete basket_form;
}


void MainWindow::on_pushButton_login_clicked()//нажали на кнопку войти(выйти)
{
    if (ui->pushButton_login->text()=="Войти"){//если входим
        registration->setModeLogin();//режим окна - вход
        registration->exec();//вызов диалогового окна
    }
    else{//если выходим
        cur_user=0;//затираев значение вошкдшего пользователя
        ui->tableWidget->hideColumn(6);//прячем колонку кнопок
        ui->pushButton_login->setText("Войти");//меняем текст на кнопке
        ui->label_2->show();//настройка интерфейса
        ui->pushButton_register->show();
        ui->pushButton_basket->hide();
        ui->pushButton_profile->hide();
        if (!ui->label_admin->isHidden()){
            ui->label_admin->hide();
            ui->pushButton_redact->hide();
            ui->label_3->hide();
            ui->spinBox->hide();
            ui->pushButton_addShow->hide();
            ui->pushButton_deleteShow->hide();
            ui->tableWidget->show();

        }
    }
}

void MainWindow::checkUser(QString login,QString password){//функция провереки пользователя, который входит
    if (db->isOpen()){
        *qw=db->exec("SELECT customer_name,customer_surname,customer_login,customer_password,customer_id FROM customer\n"
                     "WHERE customer_login='"+login+"'");//запрос на получение всех данных о пользователе
        if (qw->next()){//если не пустой рез запроса
            if (qw->value(3).toString()==password){//проверка пароля
                cur_user=qw->value(4).toInt();
                QMessageBox::information(registration,"Ура","Вход выполнен!");//сообщение
                registration->close();//закрытие диал окна
                ui->tableWidget->show();//настойка интерфейса
                *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                             "ORDER BY show_date,show_time");//выбираем нужные поля из таблицы спектаклей
                ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
                ui->tableWidget->setColumnCount(7);
                QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
                ui->tableWidget->setHorizontalHeaderLabels(list);
                for(int i=0;i<ui->tableWidget->rowCount();i++)
                    for (int j=0;j<ui->tableWidget->columnCount();j++)
                        if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
                        {
                            QTableWidgetItem * ti;
                            ti = new QTableWidgetItem;
                            ui->tableWidget->setItem(i, j, ti);
                        }
                int j=0;//индекс для первого цикла
                while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
                    int i=0;//индекс для второго цикла
                    while(i<6){//всего 6 полей в запросе

                        if (i==4){//если это время - убираем секунды и доли секунд
                            QString time=qw->value(i).toString();
                            time.remove(4,7);
                            ui->tableWidget->item(j,i)->setText(time);
                            i++;
                            continue;
                        }

                        ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
                        if (i!=2){
                            ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                            ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
                        }
                        i++;//шаг
                    }
                    if (i==6)//для поля с кнопкой
                        for (int k=0;k<20;k++){//максимум можно 20 шоу
                            if (mas_bool[k])//для нумерации кнопок
                                continue;
                            else{
                                mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                                buy->setText("Места");//надпись на кнопке
                                QString str;
                                str.setNum(k);
                                buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                                connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                                ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                                mas_bool[k]=true;
                                break;
                            }
                        }
                    j++;//шаг
                }
                for (int i=0;i<20;i++)
                    mas_bool[i]=false;
//            ui->tableWidget->hideColumn(6);//прячем столбец с кнопками
                ui->label->show();

                ui->pushButton_login->setText("Выйти");
                ui->label_2->hide();
                ui->pushButton_register->hide();
//                qw->bindValue(":c_id",cur_user);
                if (cur_user==10){
                    setAdminInterface();
                    return;
                }
                ui->pushButton_profile->show();
                ui->pushButton_basket->show();
                qw->prepare("SELECT COUNT(*) FROM public.basket_tickets\n"
                            "WHERE basket_id=(SELECT basket_id FROM public.basket WHERE customer_id=:c_id)");//для красивой кнопки корзина, узнаем сколько сейчас в  корзине билетов
                qw->bindValue(":c_id",cur_user);
                qw->exec();
                qw->next();
                ui->pushButton_basket->setText("Билеты ("+QString::number(qw->value(0).toInt())+")");
                ui->tableWidget->showColumn(6);
            }
            else//если непр пароль
                QMessageBox::warning(registration,"Внимание","Неправильный пароль");
        }
        else//если непр логин
            QMessageBox::warning(registration,"Внимание","Неправильный логин");
    }
}

void MainWindow::addUser(QString name,QString surname,bool sex,QString login,QString pwd){
    if (db->isOpen()){
        qw->prepare("SELECT customer_login FROM customer\n"
                    "WHERE customer_login=:login");//запрос для проверки уникальности
        qw->bindValue(":login",login);
        if (qw->exec()){
            if (qw->next()){
                QMessageBox::warning(registration,"Внимание","Пользователь с таким логином уже существует! Придумайте другой логин");
                return;
            }
        }
        else return;
        qw->prepare("INSERT INTO customer (customer_name,customer_surname,customer_male,customer_login,customer_password)\n"
                    "VALUES(:name,:surname,:sex,:login,:pwd)");
        qw->bindValue(":name",name);
        qw->bindValue(":surname",surname);
        qw->bindValue(":sex",sex);
        qw->bindValue(":login",login);
        qw->bindValue(":pwd",pwd);
        if (!qw->exec()){
            QMessageBox::information(registration,"","Проблема с запросом на создание нового пользователя");
            return;
        }
        qw->prepare("SELECT customer_id FROM customer\n"
                    "WHERE customer_login=:login");
        qw->bindValue(":login",login);
        if (!qw->exec()){
            QMessageBox::information(registration,"","Не удалось получить ID пользователя");
            return;
        }
        qw->next();
        cur_user=qw->value(0).toInt();
        qw->prepare("INSERT INTO basket(customer_id)\n"
                    "VALUES(:c_id)");
        qw->bindValue(":c_id",cur_user);
        if (qw->exec()){
            QMessageBox::information(registration,"Поздравляем","Вы успешно зарегистрировались и вошли!");
            ui->pushButton_register->hide();
            ui->pushButton_login->setText("Выйти");
            ui->label->hide();
            ui->pushButton_basket->show();
            ui->pushButton_basket->setText("Корзина (0)");
            ui->pushButton_profile->show();
            ui->tableWidget->showColumn(6);
            registration->clear();
            registration->close();
        }
        else
            QMessageBox::warning(registration,"О нет","Что-то пошло не так");
    }
}

void MainWindow::on_pushButton_register_clicked()
{
    registration->setModeRegistration();
    registration->exec();
}

void MainWindow::showPlaces(QString num_of_btn){//функция которая выводит схему зала
    if (db->isOpen()){
        bool mas[30];
        for (int i=0;i<30;i++)
            mas[i]=true;
        int num=num_of_btn.right(1).toInt();//номер строки
        kostil=num_of_btn;//
        int zal=ui->tableWidget->item(num,5)->text().toInt();
        int show=ui->tableWidget->item(num,0)->text().toInt();
        qw->prepare("SELECT place_id FROM public.ticket\n"
                    "WHERE show_id=:show\n"
                    "ORDER BY place_id");
        qw->bindValue(":show",show);
        if (qw->exec())
            while(qw->next())
                mas[qw->value(0).toInt()-((zal-1)*30)-1]=false;
        zal_form->setMassiv(mas);//раскраска лейблов
        zal_form->setZalShow(zal,show);
        if (!zal_form->isActiveWindow())//для того чтобы перекрасить место которое мы заняли, тк в функции reservePlace вызывается данная функция
            zal_form->exec();
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
        if (qw->exec())
            QMessageBox::information(my_profile,"Отлично","Персональные данные были обновлены");
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
                    "WHERE t.customer_id=:user\n"
                    "ORDER BY t.ticket_id ASC");
        qw->bindValue(":user",cur_user);
        if (qw->exec()){
            basket_form->setBasket(qw);
            basket_form->exec();
        }
        else
            QMessageBox::warning(this,"о нет","Что-то пошло не так и блаблабла");
    }
}

void MainWindow::ticketDelete(int ticket_id){
    if (db->isOpen()){
        qw->prepare("DELETE FROM public.ticket\n"
                    "WHERE ticket_id=:t_id");
        qw->bindValue(":t_id",ticket_id);
        if (qw->exec()){
            QMessageBox::information(basket_form,"Хорошо","Билет был удален");
            qw->prepare("SELECT t.ticket_id,p.place_row,p.place_num,s.show_name,p.zal_id,s.show_date,s.show_time FROM public.basket_tickets b\n"
                        "JOIN public.ticket t ON t.ticket_id=b.ticket_id\n"
                        "JOIN public.place p ON p.place_id=t.place_id\n"
                        "JOIN public.show s ON s.show_id=t.show_id\n"
                        "WHERE t.customer_id=:user\n"
                        "ORDER BY t.ticket_id ASC");
            qw->bindValue(":user",cur_user);
            if (qw->exec()){
                basket_form->setBasket(qw);
                QStringList str=ui->pushButton_basket->text().split("(");
                str[1].remove(")");
                int num=str[1].toInt();
                num--;
                ui->pushButton_basket->setText(str[0]+"("+QString::number(num)+")");
            }

        }else
            QMessageBox::warning(basket_form,"О нет...","Что-то пошло не так и билет не был удален");
    }
}
void MainWindow::reservePlace(int num_place,int num_zal, int show_id){//функция резервации места
    int place_id=(num_zal-1)*30+num_place+1;
    if (db->isOpen()){
        qw->prepare("SELECT basket_id FROM public.basket\n"
                    "WHERE customer_id=:user_id");
        qw->bindValue(":user_id",cur_user);
        if (!qw->exec()){
            QMessageBox::information(zal_form,"","Запрос на получение баскет_id не сработал");
            return;
        }
        qw->next();
        int basket_id=qw->value(0).toInt();
        qw->prepare("INSERT INTO public.ticket(customer_id,place_id,show_id)\n"
                    "VALUES(:c_id,:p_id,:s_id)");
        qw->bindValue(":c_id",cur_user);
        qw->bindValue(":p_id",place_id);
        qw->bindValue(":s_id",show_id);
        if (!qw->exec()){
            QMessageBox::information(zal_form,"","Запрос на добавление билета в базу сломан");
            return;
        }
        qw->prepare("SELECT ticket_id FROM public.ticket t\n"
                    "WHERE customer_id=:c_id AND place_id=:p_id AND show_id=:s_id\n"
                    "ORDER BY ticket_id DESC\n"
                    "LIMIT 1");
        qw->bindValue(":c_id",cur_user);
        qw->bindValue(":p_id",place_id);
        qw->bindValue(":s_id",show_id);
        if (!qw->exec()){
            QMessageBox::information(zal_form,"","Запрос на получение айди билета сломан");
            return;
        }
        qw->next();
        int ticket_id=qw->value(0).toInt();
        qw->prepare("INSERT INTO public.basket_tickets(basket_id,ticket_id)\n"
                    "VALUES(:b_id,:t_id)");
        qw->bindValue(":t_id",ticket_id);
        qw->bindValue(":b_id",basket_id);
        if (qw->exec()){
            QMessageBox::information(zal_form,"Хорошо","Билет был добавлен");
            showPlaces(kostil);
            QStringList str=ui->pushButton_basket->text().split("(");
            str[1].remove(")");
            int num=str[1].toInt();
            num++;
            ui->pushButton_basket->setText(str[0]+"("+QString::number(num)+")");

            /*^^^*/

        }else
            QMessageBox::warning(basket_form,"О нет...","Запрос на добавление билета в корзину сломан");
    }
}
void MainWindow::setAdminInterface(){//настройка интерфейса для админа
    //    ui->tableWidget->hide();
    ui->label->hide();
    ui->label_admin->show();
    ui->pushButton_addShow->show();
    ui->pushButton_deleteShow->show();
    ui->pushButton_redact->show();
    ui->spinBox->show();
    ui->label_3->show();
}

void MainWindow::on_pushButton_addShow_clicked()//нажата кнопка добавления спектакля
{
    addShow_form->exec();//открывается соответсвующее диалоговое окно
}


void MainWindow::on_pushButton_deleteShow_clicked()//функция отправления запроса
{
    if (db->isOpen()){
        qw->prepare("SELECT * FROM public.show\n"
                    "ORDER BY show_id");
        if (qw->exec()){
            delShow_form->setList(qw);//отправляем список спектаклей в диалоговое окно
            delShow_form->exec();

        }
        else{
            QMessageBox::warning(this,"Внимание","Не удалось получить список спектаклей");
        }
    }
}

void MainWindow::addSpectacle(QString name,QString description,QString date,QString time,int zal){//функция добавления спектакля (срабатывает с помощью сигнала из диалога)
    if (db->isOpen()){
        qw->prepare("SELECT COUNT(*) FROM public.show\n"
                    "WHERE show_date=:date");
        qw->bindValue(":date",date);
        qw->exec();
        qw->next();
        if (qw->value(0).toInt()>4){
            QMessageBox::information(addShow_form,"Внимание","На данный день в программе уже 4 спектакля.");
            return;
        }
        qw->prepare("SELECT COUNT(*) FROM public.show\n"
                    "WHERE show_name=:name AND show_date=:date");
        qw->bindValue(":name",name);
        qw->bindValue(":date",date);
        qw->exec();
        qw->next();
        if (qw->value(0).toInt()>0){
            QMessageBox::information(addShow_form,"Внимание","На данный день уже есть в программме данный спектакль.");
            return;
        }
        qw->prepare("SELECT show_time FROM public.show\n"
                    "WHERE show_date=:date AND zal_id=:zal");
//        qw->bindValue(":time",time);
        qw->bindValue(":date",date);
        qw->bindValue(":zal",zal);
        if (qw->exec()){
            while (qw->next()){
            QString tmp=qw->value(0).toString();
            tmp.remove(4,7);
            QMessageBox::information(addShow_form,"Внимание","В данном зале на выбранную дату и время уже есть спектакль. Если вы хотите добавить спектакль на"
                                                             " данную дату, то выберите другое время.");
            return;
            }
        }

        qw->prepare("INSERT INTO public.show(show_name,show_description,show_date,show_time,zal_id)\n"
                    "VALUES(:name,:desc,:date,:time,:zal)");
        qw->bindValue(":name",name);
        qw->bindValue(":desc",description);
        qw->bindValue(":date",date);
        qw->bindValue(":time",time);
        qw->bindValue(":zal",zal);
        if (qw->exec()){
            QMessageBox::information(addShow_form,"Хорошо","Спектакль был добавлен");
            addShow_form->close();
            *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                         "ORDER BY show_date, show_time\n");//выбираем нужные поля из таблицы спектаклей
            ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
            ui->tableWidget->setColumnCount(7);
            QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
            ui->tableWidget->setHorizontalHeaderLabels(list);
            for(int i=0;i<ui->tableWidget->rowCount();i++)
                for (int j=0;j<ui->tableWidget->columnCount();j++)
                    if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
                    {
                        QTableWidgetItem * ti;
                        ti = new QTableWidgetItem;
                        ui->tableWidget->setItem(i, j, ti);
                    }
            int j=0;//индекс для первого цикла
            while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
                int i=0;//индекс для второго цикла
                while(i<6){//всего 6 полей в запросе

                    if (i==4){//если это время - убираем секунды и доли секунд
                        QString time=qw->value(i).toString();
                        time.remove(4,7);
                        ui->tableWidget->item(j,i)->setText(time);
                        i++;
                        continue;
                    }

                    ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
                    if (i!=2){
                        ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                        ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
                    }
                    i++;//шаг
                }
                if (i==6)//для поля с кнопкой
                    for (int k=0;k<20;k++){//максимум можно 20 шоу
                        if (mas_bool[k])//для нумерации кнопок
                            continue;
                        else{
                            mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                            buy->setText("Места");//надпись на кнопке
                            QString str;
                            str.setNum(k);
                            buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                            connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                            ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                            mas_bool[k]=true;
                            break;
                        }
                    }
                j++;//шаг
            }
            for (int i=0;i<20;i++)
                mas_bool[i]=false;
            ui->tableWidget->hideColumn(6);

        }
        else{
            QMessageBox::information(addShow_form,"Плохо","Спектакль не был добавлен");

        }

    }

}

void MainWindow::delSpectacle(int show_id){//функция удаления спектакля (срабатывает с помощью сигнала из диалога)
    if (db->isOpen()){
//        qw->clear();
        qw->prepare("DELETE FROM public.show\n"
                    "WHERE show_id=:show");
        qw->bindValue(":show",show_id);
        if (qw->exec()){
            QMessageBox::information(delShow_form,"Хорошо","Спектакль был удален");
            qw->prepare("SELECT * FROM public.show\n"
                        "ORDER BY show_id");
            if (qw->exec()){
                delShow_form->setList(qw);
                *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                             "ORDER BY show_date,show_time\n");//выбираем нужные поля из таблицы спектаклей
                ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
                ui->tableWidget->setColumnCount(7);
                QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
                ui->tableWidget->setHorizontalHeaderLabels(list);
                for(int i=0;i<ui->tableWidget->rowCount();i++)
                    for (int j=0;j<ui->tableWidget->columnCount();j++)
                        if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
                        {
                            QTableWidgetItem * ti;
                            ti = new QTableWidgetItem;
                            ui->tableWidget->setItem(i, j, ti);
                        }
                int j=0;//индекс для первого цикла
                while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
                    int i=0;//индекс для второго цикла
                    while(i<6){//всего 6 полей в запросе

                        if (i==4){//если это время - убираем секунды и доли секунд
                            QString time=qw->value(i).toString();
                            time.remove(4,7);
                            ui->tableWidget->item(j,i)->setText(time);
                            i++;
                            continue;
                        }

                        ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
                        if (i!=2){
                            ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                            ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
                        }
                        i++;//шаг
                    }
                    if (i==6)//для поля с кнопкой
                        for (int k=0;k<20;k++){//максимум можно 20 шоу
                            if (mas_bool[k])//для нумерации кнопок
                                continue;
                            else{
                                mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                                buy->setText("Места");//надпись на кнопке
                                QString str;
                                str.setNum(k);
                                buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                                connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                                ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                                mas_bool[k]=true;
                                break;
                            }
                        }
                    j++;//шаг
                }
                for (int i=0;i<20;i++)
                    mas_bool[i]=false;
                if (cur_user==0)
                    ui->tableWidget->hideColumn(6);
            }

        }
        else{
            QMessageBox::information(delShow_form,"Плохо","Спектакль не был удален");
        }

    }
}

void MainWindow::on_pushButton_reload_clicked()//кнопка отбновления страницы
{
    *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                 "ORDER BY show_date,show_time\n");//выбираем нужные поля из таблицы спектаклей
    ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
    ui->tableWidget->setColumnCount(7);
    QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
    ui->tableWidget->setHorizontalHeaderLabels(list);
    for(int i=0;i<ui->tableWidget->rowCount();i++)
        for (int j=0;j<ui->tableWidget->columnCount();j++)
            if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
            {
                QTableWidgetItem * ti;
                ti = new QTableWidgetItem;
                ui->tableWidget->setItem(i, j, ti);
            }
    int j=0;//индекс для первого цикла
    while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
        int i=0;//индекс для второго цикла
        while(i<6){//всего 6 полей в запросе

            if (i==4){//если это время - убираем секунды и доли секунд
                QString time=qw->value(i).toString();
                time.remove(4,7);
                ui->tableWidget->item(j,i)->setText(time);
                i++;
                continue;
            }

            ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
            if (i!=2){
                ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
            }
            i++;//шаг
        }
        if (i==6)//для поля с кнопкой
            for (int k=0;k<20;k++){//максимум можно 20 шоу
                if (mas_bool[k])//для нумерации кнопок
                    continue;
                else{
                    mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                    buy->setText("Места");//надпись на кнопке
                    QString str;
                    str.setNum(k);
                    buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                    connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                    ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                    mas_bool[k]=true;
                    break;
                }
            }
        j++;//шаг
    }
    for (int i=0;i<20;i++)
        mas_bool[i]=false;
    if (cur_user==0)
        ui->tableWidget->hideColumn(6);
    qw->prepare("SELECT COUNT(*) FROM public.basket_tickets\n"
                "WHERE basket_id=(SELECT basket_id FROM public.basket WHERE customer_id=:c_id)");//для красивой кнопки корзина, узнаем сколько сейчас в  корзине билетов
    qw->bindValue(":c_id",cur_user);
    qw->exec();
    qw->next();
    ui->pushButton_basket->setText("Билеты ("+QString::number(qw->value(0).toInt())+")");
}


void MainWindow::on_pushButton_redact_clicked()//нажали на кнопку редактирование спектакля
{
    if (db->isOpen()){
        qw->prepare("SELECT show_name,show_description,show_date,show_time,zal_id FROM public.show \n"
                    "WHERE show_id=:show");
        qw->bindValue(":show",ui->spinBox->value());
        if (qw->exec()){
            if (!qw->next()){
                QMessageBox::warning(this,"Внимание","Нет спектакля с таким ID");
                return;
            }
            addShow_form->setName(qw->value(0).toString());
            addShow_form->setDescription(qw->value(1).toString());
            addShow_form->setDate(qw->value(2).toString());
            addShow_form->setTime(qw->value(3).toString());
            addShow_form->setZal(qw->value(4).toString());
            addShow_form->exec();


        }else {
        QMessageBox::warning(this,"Внимание","Нет спектакля с таким ID");
}
    }
}
void MainWindow::showChange(QString name,QString description,QString date,QString time,int zal){//функция изменения инф о спектакле
    if (db->isOpen()){
        qw->prepare("SELECT COUNT(*) FROM public.show\n"
                    "WHERE show_date=:date");
        qw->bindValue(":date",date);
        qw->exec();
        qw->next();
        if (qw->value(0).toInt()>5){
            QMessageBox::information(addShow_form,"Внимание","На данный день в программе уже 4 спектакля.");
            return;
        }
        qw->prepare("SELECT COUNT(*) FROM public.show\n"
                    "WHERE show_name=:name AND show_date=:date");
        qw->bindValue(":name",name);
        qw->bindValue(":date",date);
        qw->exec();
        qw->next();
        if (qw->value(0).toInt()>1){
            QMessageBox::information(addShow_form,"Внимание","На данный день уже есть в программме данный спектакль.");
            return;
        }
        qw->prepare("SELECT show_time,show_id FROM public.show\n"
                    "WHERE show_date=:date AND zal_id=:zal");
//        qw->bindValue(":time",time);
        qw->bindValue(":date",date);
        qw->bindValue(":zal",zal);
        if (qw->exec()){
            while (qw->next()){
                if (qw->value(1).toInt()==ui->spinBox->value()){
                    continue;
                }
            QString tmp=qw->value(0).toString();
            tmp.remove(4,7);
            QMessageBox::information(addShow_form,"Внимание","В данном зале на выбранную дату и время уже есть спектакль. Если вы хотите добавить спектакль на"
                                                             " данную дату, то выберите другое время.");
            return;
            }
        }
        qw->prepare("UPDATE public.show\n"
                    "SET show_name=:name,\n"
                    "show_description=:desc,\n"
                    "show_date=:date,\n"
                    "show_time=:time,\n"
                    "zal_id=:zal\n"
                    "WHERE show_id=:show_id");
        qw->bindValue(":name",name);
        qw->bindValue(":desc",description);
        qw->bindValue(":date",date);
        qw->bindValue(":time",time);
        qw->bindValue(":zal",zal);
        qw->bindValue(":show_id",ui->spinBox->value());
        if (qw->exec()){
            addShow_form->setKnopka();
            addShow_form->close();
            QMessageBox::information(addShow_form,"Отлично","Спектакль был изменен.");
            *qw=db->exec("SELECT show_id,show_name,show_description,show_date,show_time,zal_id FROM public.show s\n"
                         "ORDER BY show_date,show_time\n");//выбираем нужные поля из таблицы спектаклей
            ui->tableWidget->setRowCount(qw->size());//устанавливаем количесвто строк, которые вернул запрос
            ui->tableWidget->setColumnCount(7);
            QStringList list={"ID","Название спектакля","Описание","Дата","Время","Номер зала","Просмотр мест"};//названия столбцов
            ui->tableWidget->setHorizontalHeaderLabels(list);
            for(int i=0;i<ui->tableWidget->rowCount();i++)
                for (int j=0;j<ui->tableWidget->columnCount();j++)
                    if (ui->tableWidget->item(i,j)==nullptr)//выделяем память для каждой ячейки если она не выделена
                    {
                        QTableWidgetItem * ti;
                        ti = new QTableWidgetItem;
                        ui->tableWidget->setItem(i, j, ti);
                    }
            int j=0;//индекс для первого цикла
            while(qw->next()){//в условии осуществляем переход на следующую строку полученного запроса
                int i=0;//индекс для второго цикла
                while(i<6){//всего 6 полей в запросе

                    if (i==4){//если это время - убираем секунды и доли секунд
                        QString time=qw->value(i).toString();
                        time.remove(4,7);
                        ui->tableWidget->item(j,i)->setText(time);
                        i++;
                        continue;
                    }

                    ui->tableWidget->item(j,i)->setText(qw->value(i).toString());//если это не время то как есть
                    if (i!=2){
                        ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignHCenter);
                        ui->tableWidget->item(j,i)->setTextAlignment(Qt::AlignVCenter);
                    }
                    i++;//шаг
                }
                if (i==6)//для поля с кнопкой
                    for (int k=0;k<20;k++){//максимум можно 20 шоу
                        if (mas_bool[k])//для нумерации кнопок
                            continue;
                        else{
                            mybutton *buy=new mybutton(this);//создаем собств класс кнопки
                            buy->setText("Места");//надпись на кнопке
                            QString str;
                            str.setNum(k);
                            buy->setObjectName("btn_buy"+str);//название виджета кнопки(используется для того чтобы понять, в какой строке была нажата кнопка
                            connect(buy,SIGNAL(clicked(QString)),this,SLOT(showPlaces(QString)));//связываем переопределенный сигнал нажатия на кнопку с функцией
                            ui->tableWidget->setCellWidget(j,i,buy);//вставляем кнопку в соответсвующую ячейку
                            mas_bool[k]=true;
                            break;
                        }
                    }
                j++;//шаг
            }
            for (int i=0;i<20;i++)
                mas_bool[i]=false;
            if (cur_user==0)
                ui->tableWidget->hideColumn(6);

        }
        else{
            QMessageBox::warning(addShow_form,"О нет","ничего не получилось");
        }
    }

}
