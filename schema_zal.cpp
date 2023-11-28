#include "schema_zal.h"
#include "ui_schema_zal.h"

schema_zal::schema_zal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::schema_zal)
{
    ui->setupUi(this);
//    if ui->gridLayout->itemAtPosition()
//        ui->gridLayout->itemAtPosition()

//    for (int i=0;i<=ui->gridLayout->rowCount();i++){
//        for (int j=0;j<=ui->gridLayout->columnCount();j++){
//            QLabel *widg=ui->label_1;
//            ui->gridLayout->addWidget(widg,5,1);
//            qDebug()<<ui->gridLayout->rowCount();
//            qDebug()<<ui->gridLayout->columnCount();
//        }
//    }

//            QString lblname="label_"+QString::number(i);
//            mas[i]=ui->gridLayout->itemAtPosition();
//        }
//    }
    int index=0;
    for (int i=0;i<ui->gridLayout->rowCount();i++){
        for (int j=0;j<ui->gridLayout->columnCount();j++){
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){
                    //для того чтобы не создавалось больше 1 фильтра событий
                    mas[index]=ui->gridLayout->itemAtPosition(i,j)->widget();
                    labelClicker *lblclicker=new labelClicker(ui->gridLayout->itemAtPosition(i,j)->widget());
                    //                    lblclicker->setMask(mas);
                    mas_ev_filters[index]=lblclicker;
                    connect(lblclicker,SIGNAL(placeReserved(int)),this,SLOT(reservePlace(int)));
                    ui->gridLayout->itemAtPosition(i,j)->widget()->installEventFilter(lblclicker);
                    index++;
                }
        }
    }
}

schema_zal::~schema_zal()
{
    delete ui;
}

void schema_zal::makeRed(QWidget *lbl){
    QPalette pal;
    QColor col;
    col.setRgb(255,0,0);
    pal.setColor(QPalette::ColorRole::WindowText,col);
    lbl->setPalette(pal);

}
void schema_zal::makeGreen(QWidget *lbl){
    QPalette pal;
    QColor col;
    col.setRgb(0,255,0);
    pal.setColor(QPalette::ColorRole::WindowText,col);
    lbl->setPalette(pal);

}
void schema_zal::setZalShow(int zal, int show_id){
    ui->label_31->setText(QString::number(zal));
    show=show_id;
}
//bool schema_zal::eventFilter(QObject *lbl,QEvent *event){
//    for (int i = 0; i < 30; i++) {
//        if (lbl==mas[i]){
//            if (event->type()==QEvent::MouseButtonPress){
//                if (!mas_bool[i]){
//                    QMessageBox::warning(this,"Внимание","Данное место уже занято");
//                    return true;
//                }
//                else{
//                    int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Если вы продолжите, то не сможете изменять свои личные данные еще месяц!",QMessageBox::Ok|QMessageBox::Cancel,this).exec();
//                    if (res==QMessageBox::Ok){
//                        mas_bool[i]=false;
//                        QMessageBox::information(this,"Ура","Данное место было добавлено в корзину");
//                        emit placeReserved(i,ui->label_31->text().toInt());
//                        return false;
//                    }
//                    else{
//                        return false;
//                    }

//                }
//            }
//            else return false;

//        }
//        else return false;

//    }
//    return false;

//}
void schema_zal::reservePlace(int num_place){
    emit placeReserved(num_place,ui->label_31->text().toInt(),show);
}


void schema_zal::setMassiv(bool masiv[]){
//    for (int i=1;i<=6;i++){
//        for (int j=1;j<=5;j++){
//            if (i==1 and j==1){
//                if (mas[0]){
//                    qDebug()<<ui->gridLayout->itemAtPosition(1,1);
//                    makeGreen(ui->gridLayout->itemAtPosition(1,1)->widget());
//                }
//                else {
//                    makeRed(ui->gridLayout->itemAtPosition(1,1)->widget());
//                }
//            }
//        if (mas[(i-1)*5+j]){
//            makeGreen(ui->gridLayout->itemAtPosition(j-1,i-1)->widget());
//            //придумать способ пройтись по всем лейблам с помощью обращения к названиям, возможно переписать функции перекраски
//        }
//        else{
//            makeRed(ui->gridLayout->itemAtPosition(j-1,i-1)->widget());
//        }
//        }
//    }
    for (int i=0;i<30;i++)
        mas_bool[i]=masiv[i];
    int index=0;
    for (int i=0;i<=ui->gridLayout->rowCount();i++){
        for (int j=0;j<=ui->gridLayout->columnCount();j++){
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){

                    if (masiv[index])
                        makeGreen(ui->gridLayout->itemAtPosition(i,j)->widget());
                    else
                        makeRed(ui->gridLayout->itemAtPosition(i,j)->widget());

                    index++;
            }

        }
    }
//    for (int i=0;i<ui->gridLayout->rowCount();i++){
//        for (int j=0;j<ui->gridLayout->columnCount();j++){
//            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)
//                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){
//                    //для того чтобы не создавалось больше 1 фильтра событий
//                    mas[i]=ui->gridLayout->itemAtPosition(i,j)->widget();
//                    labelClicker *lblclicker=new labelClicker(ui->gridLayout->itemAtPosition(i,j)->widget());
//                    //                    lblclicker->setMask(mas);
//                    lblclicker->setReservance(mas_bool);
//                    connect(lblclicker,SIGNAL(placeReserved(int)),this,SLOT(reservePlace(int)));
//                    ui->gridLayout->itemAtPosition(i,j)->widget()->installEventFilter(lblclicker);

//                }
//        }
//    }
    for(int i=0;i<30;i++){
        mas_ev_filters[i]->setReservance(mas_bool);
    }

}

