#include "basket_view.h"
#include "ui_basket_view.h"

basket_view::basket_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::basket_view)
{
    ui->setupUi(this);
}

basket_view::~basket_view()
{
    delete ui;
}


void basket_view::setBasket(QSqlQuery *qw){
    ui->tableWidget->setRowCount(qw->size());
    ui->tableWidget->setColumnCount(7);
    QStringList list={"Порядковый номер билета","Номер ряда","Номер места","Название спектакля","Номер зала","Дата спектакля","Время спектакля"};
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
            if (i==6){
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
    ui->tableWidget->insertColumn(7);
    for (int i=0;i<ui->tableWidget->rowCount();i++){
////                for (int k=0;k<20;k++){
////                  if (mas_bool[k]){
////                        continue;
////                    }
////                    else{
//                    if (ui->tableWidget->item(i,5)==nullptr)
//                    {
//                            QTableWidgetItem * ti;
//                            ti = new QTableWidgetItem;
//                            ui->tableWidget->setItem(i, 5, ti);
//                    }
        mybutton *buy=new mybutton(this);
        buy->setText("удалить");
        QString str;
        str.setNum(i);
        buy->setWindowTitle("btn_buy_"+str);
        //                    qDebug()<<"btn_buy"+str;
        connect(buy,SIGNAL(clicked(QString)),this,SLOT(delTicket(QString)));

        ui->tableWidget->setCellWidget(i,7,buy);
        ////                    mas_bool[k]=true;
        ////                    break;
    }

}

void basket_view::on_pushButton_close_clicked()
{
    this->close();
}

void basket_view::delTicket(QString btn_name){
    int row_of_btn=0;
        for (int i=0;i<ui->tableWidget->rowCount();i++)
            if (ui->tableWidget->cellWidget(i,7)->windowTitle()==btn_name){
                row_of_btn=i;
                break;
            }
    int ticket_id=ui->tableWidget->item(row_of_btn,0)->text().toInt();
    emit delTicket(ticket_id);

}

