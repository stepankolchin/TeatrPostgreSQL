#include "schema_zal.h"
#include "ui_schema_zal.h"

schema_zal::schema_zal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::schema_zal)
{
    ui->setupUi(this);
    int index=0;
    for (int i=0;i<ui->gridLayout->rowCount();i++)
        for (int j=0;j<ui->gridLayout->columnCount();j++)
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){//для того чтобы не создавалось больше 1 фильтра событий
                    mas[index]=ui->gridLayout->itemAtPosition(i,j)->widget();
                    labelClicker *lblclicker=new labelClicker(ui->gridLayout->itemAtPosition(i,j)->widget());
                    mas_ev_filters[index]=lblclicker;
                    connect(lblclicker,SIGNAL(placeReserved(int)),this,SLOT(reservePlace(int)));
                    ui->gridLayout->itemAtPosition(i,j)->widget()->installEventFilter(lblclicker);
                    index++;
                }
}

schema_zal::~schema_zal(){
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

void schema_zal::reservePlace(int num_place){
    emit placeReserved(num_place,ui->label_31->text().toInt(),show);
}

void schema_zal::setMassiv(bool masiv[]){
    for (int i=0;i<30;i++)
        mas_bool[i]=masiv[i];
    int index=0;
    for (int i=0;i<=ui->gridLayout->rowCount();i++)
        for (int j=0;j<=ui->gridLayout->columnCount();j++)
            if (ui->gridLayout->itemAtPosition(i,j)!=nullptr)
                if (ui->gridLayout->itemAtPosition(i,j)->widget()!=nullptr){
                    if (masiv[index])
                        makeGreen(ui->gridLayout->itemAtPosition(i,j)->widget());
                    else
                        makeRed(ui->gridLayout->itemAtPosition(i,j)->widget());
                    index++;
                }
    for(int i=0;i<30;i++)
        mas_ev_filters[i]->setReservance(mas_bool);
}
