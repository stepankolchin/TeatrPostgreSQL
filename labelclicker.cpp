#include "labelclicker.h"

labelClicker::labelClicker(QWidget *parent)
{
    parent_=parent;
}

void labelClicker::setReservance(bool masiv[]){
    for (int i=0;i<30;i++)
        mas_bool[i]=masiv[i];
}

bool labelClicker::eventFilter(QObject *lbl,QEvent *event){
    QStringList strl=lbl->objectName().split("_");
    int kal=strl[1].toInt()-1;
    if (event->type()==QEvent::MouseButtonPress)
        if (!mas_bool[kal]){
            QMessageBox::warning(parent_,"Внимание","Данное место уже занято");
            return true;
        }
        else{
            int res=QMessageBox(QMessageBox::Icon::Information,"Внимание", "Вы точно хотите положить в корзину данное место?",
                                QMessageBox::Ok|QMessageBox::Cancel,parent_).exec();
            if (res==QMessageBox::Ok){
                mas_bool[kal]=false;
                emit placeReserved(kal);
                return false;
            }
            else
                return false;
        }
    else return false;
}
