#include "mybutton.h"

mybutton::mybutton(QWidget *parent):QPushButton(parent)//конструктор
{

}

void mybutton::mousePressEvent(QMouseEvent *event){//функция для того чтобы при нажатии на кнопку срабатывал переопределнный сигнал
    if (event->button()==Qt::LeftButton)
        emit clicked(this->windowTitle());
}
