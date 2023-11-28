#include "mybutton.h"

mybutton::mybutton(QWidget *parent):QPushButton(parent)
{

}

void mybutton::mousePressEvent(QMouseEvent *event){
    if (event->button()==Qt::LeftButton)
        emit clicked(this->windowTitle());
}
