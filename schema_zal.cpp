#include "schema_zal.h"
#include "ui_schema_zal.h"

schema_zal::schema_zal(QWidget *parent) ://конструктор
    QDialog(parent),
    ui(new Ui::schema_zal)
{
    ui->setupUi(this);
    int index=0;
    for (int i=0;i<ui->gridLayout->rowCount();i++)//проходимся по всем ячейкам грида
        for (int j=0;j<ui->gridLayout->columnCount();j++)
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)//если ячейка существует
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){//если в ячеййке есть виджет(лейбл)
                    mas[index]=ui->gridLayout->itemAtPosition(i,j)->widget();//(возможно без см)
                    labelClicker *lblclicker=new labelClicker(ui->gridLayout->itemAtPosition(i,j)->widget());//ставим фильт событий для лейбла
                    mas_ev_filters[index]=lblclicker;//заносим этот фильтр в массив
                    connect(lblclicker,SIGNAL(placeReserved(int)),this,SLOT(reservePlace(int)));//соединяем сигнал из фильтра со слотом
                    ui->gridLayout->itemAtPosition(i,j)->widget()->installEventFilter(lblclicker);//устанавливаем фильтр для лейбла
                    index++;
                }
    this->setWindowTitle("Схема театрального зала");
}

schema_zal::~schema_zal(){//деструктор
    delete ui;
}

void schema_zal::makeRed(QWidget *lbl){//функция окрашивания лейбла в красный
    QPalette pal;
    QColor col;
    col.setRgb(255,0,0);
    pal.setColor(QPalette::ColorRole::WindowText,col);
    lbl->setPalette(pal);

}
void schema_zal::makeGreen(QWidget *lbl){//функция окрашивания лейбла в зеленый
    QPalette pal;
    QColor col;
    col.setRgb(0,255,0);
    pal.setColor(QPalette::ColorRole::WindowText,col);
    lbl->setPalette(pal);

}
void schema_zal::setZalShow(int zal, int show_id){//заполняем данные о зале и шоу
    ui->label_31->setText(QString::number(zal));
    show=show_id;//понадобится чтобы не делать при бронировании лишний запрос
}

void schema_zal::reservePlace(int num_place){//отправляем сигнал, в котором передаем: место, ид зал и ид спектакля
    emit placeReserved(num_place,ui->label_31->text().toInt(),show);
}

void schema_zal::setMassiv(bool masiv[]){//основная функция
    for (int i=0;i<30;i++)//заполняем собственный массив (возможно без см)
        mas_bool[i]=masiv[i];
    int index=0;//
    for (int i=0;i<=ui->gridLayout->rowCount();i++)//проходимся по всем ячейкам грида
        for (int j=0;j<=ui->gridLayout->columnCount();j++)
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)//если ячейка существует
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){//если в ячеййке есть виджет(лейбл)
                    if (masiv[index])//если место не звнято
                        makeGreen(ui->gridLayout->itemAtPosition(i,j)->widget());//окрашиваем в зеленый
                    else{
                        makeRed(ui->gridLayout->itemAtPosition(i,j)->widget());//окрашиваем в красный
                    }
                    index++;
                }
    for(int i=0;i<30;i++)//передаем каждому фильтру событий массив заполненности (возможно без см)
        mas_ev_filters[i]->setReservance(mas_bool);
}
