#include "addshowdialogue.h"
#include "ui_addshowdialogue.h"

addShowDialogue::addShowDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addShowDialogue)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление спектакля");
}

addShowDialogue::~addShowDialogue()
{
    delete ui;
}

void addShowDialogue::on_pushButton_Add_clicked()
{
    QString name=ui->lineEdit_Name->text();
    QString date=ui->lineEdit_Date->text();
    QString time=ui->lineEdit_Time->text()+":00";
    int zal=ui->comboBox->currentIndex()+1;
    QString description=ui->textEdit->toPlainText();
    if (date.size()==10 and time.length()==8)
        if (!name.isEmpty())
            if (!description.isEmpty())
                emit addShow(name,description,date,time,zal);
            else
                QMessageBox::warning(this,"Внимание","Описание не должно быть пустым!");
        else
            QMessageBox::warning(this,"Внимание","Название спектакля не должно быть пустым!");
    else
        QMessageBox::warning(this,"Внимание","Неправильная дата или время");
}
void addShowDialogue::closeEvent(QCloseEvent *event){
    if (ui->lineEdit_Name->text().isEmpty() and ui->textEdit->toPlainText().isEmpty())
        event->accept();
    int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Вы точно хотите закрыть это окно? Все несохраненные изменения пропадут.",
                        QMessageBox::Ok|QMessageBox::Cancel,this).exec();//спрашиваем хотим ли положить в корзину
    if (res!=QMessageBox::Ok){//если да
        event->ignore();
        return;
    }

    ui->lineEdit_Date->clear();
    ui->lineEdit_Name->clear();
    ui->lineEdit_Time->clear();
    ui->textEdit->clear();
    event->accept();
}

