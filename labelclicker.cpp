#include "labelclicker.h"

labelClicker::labelClicker(QWidget *parent)//конструктор
{
    parent_=parent;//для того чтобы в родительский виджет пихать сообщение
}

void labelClicker::setReservance(bool masiv[]){
    for (int i=0;i<30;i++)
        mas_bool[i]=masiv[i];//массив занятости мест
}

bool labelClicker::eventFilter(QObject *lbl,QEvent *event){//добавляем фильтр событий
    QStringList strl=lbl->objectName().split("_");//дробим имя лейбла
    int kal=strl[1].toInt()-1;//-1, т.к. место 1 а не 0, не .last(), тк может быть 1значное или 2значное
    if (event->type()==QEvent::MouseButtonPress)//есил событие - нажатие ЛКМ то что то делаем
        if (!mas_bool[kal]){//если место занято
            QMessageBox::warning(parent_,"Внимание","Данное место уже занято");
            return true;
        }
        else{//если место не занято
            int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Вы точно хотите положить в корзину данное место?",
                                QMessageBox::Ok|QMessageBox::Cancel,parent_).exec();//спрашиваем хотим ли положить в корзину
            if (res==QMessageBox::Ok){//если да
                mas_bool[kal]=false;//изменяем массив
                emit placeReserved(kal);//сигнал об изменении этого места
                return false;
            }
            else
                return false;
        }
    else return false;
}
