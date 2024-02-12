#include "addshowdialogue.h"
#include "ui_addshowdialogue.h"

addShowDialogue::addShowDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addShowDialogue)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление и изменение спектакля");
    fl=false;


}

addShowDialogue::~addShowDialogue()
{
    delete ui;
}

void addShowDialogue::on_pushButton_Add_clicked()//нажали на добавить
{
    QString name=ui->lineEdit_Name->text();
    QString date=ui->lineEdit_Date->text();
    if (date.left(4)!="2023" and date.left(4)!="2024"){
        QMessageBox::warning(this,"Внимание","Неправильная дата.");
        return;
    }
    QString tmp=date;
    tmp.remove(0,5);
    if (tmp.left(2).toInt()>12){

        QMessageBox::warning(this,"Внимание","Неправильная дата.");
        return;
    }
    if (tmp.left(2).toInt()==2 and tmp.right(2).toInt()>29)
    {
        QMessageBox::warning(this,"Внимание","Неправильная дата.");
        return;
    }
    if (tmp.left(2).toInt()<=7 and  tmp.left(2).toInt()%2==1){
        if (tmp.right(2).toInt()>31){
            QMessageBox::warning(this,"Внимание","Неправильная дата.");
            return;
        }


    }
    else if(tmp.left(2).toInt()>=8 and tmp.left(2).toInt()%2==0){
        if (tmp.right(2).toInt()>31){
//            qDebug()<<tmp.left(2).toInt();
            QMessageBox::warning(this,"Внимание","Неправильная дата.");
            return;
        }
    }
    else if(tmp.left(2).toInt()>=8 and tmp.left(2).toInt()%2==1){
        if (tmp.right(2).toInt()>30){
            QMessageBox::warning(this,"Внимание","Неправильная дата.");
            return;
        }
    }
    else if (tmp.left(2).toInt()<=7 and  tmp.left(2).toInt()%2==0) {
        if (tmp.right(2).toInt()>30){
            QMessageBox::warning(this,"Внимание","Неправильная дата.");
            return;
        }
    }
    QString time=ui->comboBox_Time->currentText()+":00";
    int zal=ui->comboBox->currentIndex()+1;
    QString description=ui->textEdit->toPlainText();
    if (date.size()==10)
        if (!name.isEmpty())
            if (!description.isEmpty()){
                if (ui->pushButton_Add->text()=="Изменить")
                    emit changeShow(name,description,date,time,zal);
                else
                    emit addShow(name,description,date,time,zal);
                fl=true;
//                this->close();
            }
            else
                QMessageBox::warning(this,"Внимание","Описание не должно быть пустым!");
        else
            QMessageBox::warning(this,"Внимание","Название спектакля не должно быть пустым!");
    else
        QMessageBox::warning(this,"Внимание","Неправильная дата");
}
void addShowDialogue::closeEvent(QCloseEvent *event){//не закрываем окно сразу
    if ((ui->lineEdit_Name->text().isEmpty() and ui->textEdit->toPlainText().isEmpty()) or fl){
        fl=false;
        ui->lineEdit_Date->clear();
        ui->lineEdit_Name->clear();
        ui->comboBox_Time->setCurrentIndex(0);
        ui->textEdit->clear();
        event->accept();
        return;
    }
    int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Вы точно хотите закрыть это окно? Все несохраненные изменения пропадут.",
                        QMessageBox::Ok|QMessageBox::Cancel,this).exec();//спрашиваем хотим ли положить в корзину
    if (res!=QMessageBox::Ok){//если да
        event->ignore();
        return;
    }

    ui->lineEdit_Date->clear();
    ui->lineEdit_Name->clear();
    ui->comboBox_Time->setCurrentIndex(0);
    ui->textEdit->clear();
    event->accept();
}
//сеттеры
void addShowDialogue::setName(QString str){
    ui->lineEdit_Name->setText(str);

}

void addShowDialogue::setDescription(QString str){
    ui->textEdit->setText(str);

}

void addShowDialogue::setDate(QString str){
    ui->lineEdit_Date->setText(str);

}

void addShowDialogue::setZal(QString str){
    ui->comboBox->setCurrentIndex(str.toInt()-1);
    ui->pushButton_Add->setText("Изменить");
}

void addShowDialogue::setTime(QString str){
    if (str.left(2).toInt()==15)
        ui->comboBox_Time->setCurrentIndex(0);
    if (str.left(2).toInt()==18)
        ui->comboBox_Time->setCurrentIndex(1);
    if (str.left(2).toInt()==21)
        ui->comboBox_Time->setCurrentIndex(2);

}

void addShowDialogue::setKnopka(){
    ui->pushButton_Add->setText("Добавить");
}

