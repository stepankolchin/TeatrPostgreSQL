#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class mybutton:public QPushButton
{
    Q_OBJECT
public:
    explicit mybutton(QWidget *parent=nullptr);

private slots:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(QString btn_name);
};

#endif // MYBUTTON_H
