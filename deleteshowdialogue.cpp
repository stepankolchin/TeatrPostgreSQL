#include "deleteshowdialogue.h"
#include "ui_deleteshowdialogue.h"

deleteShowDialogue::deleteShowDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteShowDialogue)
{
    ui->setupUi(this);
    this->setWindowTitle("Удаление спектаклей");
}

deleteShowDialogue::~deleteShowDialogue()
{
    delete ui;
}

void deleteShowDialogue::on_pushButton_Close_clicked()
{
    this->close();
}

void deleteShowDialogue::setList(QSqlQuery *qw){
    ui->tableWidget->setRowCount(qw->size());
    ui->tableWidget->setColumnCount(6);

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
        while(i!=6){
            if (i==4){
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
    ui->tableWidget->insertColumn(6);
    for (int i=0;i<ui->tableWidget->rowCount();i++){
        mybutton *buy=new mybutton(this);
        buy->setText("удалить");
        QString str;
        str.setNum(i);
        buy->setWindowTitle("btn_del_"+str);
        connect(buy,SIGNAL(clicked(QString)),this,SLOT(delShow(QString)));
        ui->tableWidget->setCellWidget(i,6,buy);
    }
    QStringList list={"Номер спектакля","Название","Описание","Дата","Время","Номер зала","Удаление спектакля"};
    ui->tableWidget->setHorizontalHeaderLabels(list);
}


void deleteShowDialogue::delShow(QString btn_name){
    int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Вы точно хотите удалить данный спектакль? Это приведет к удалению всех существующих билетов.",
                        QMessageBox::Ok|QMessageBox::Cancel,this).exec();//спрашиваем хотим ли положить в корзину
    if (res!=QMessageBox::Ok){//если да
        return;
    }
    int row_of_btn=0;
    for (int i=0;i<ui->tableWidget->rowCount();i++)
        if (ui->tableWidget->cellWidget(i,6)->windowTitle()==btn_name){
            row_of_btn=i;
            break;
        }
    int show_id=ui->tableWidget->item(row_of_btn,0)->text().toInt();
    emit deleteShow(show_id);

}
