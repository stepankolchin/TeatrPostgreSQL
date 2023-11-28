#ifndef LABELCLICKER_H
#define LABELCLICKER_H

#include <QObject>
#include <QMessageBox>
#include <QEvent>

class labelClicker:public QObject
{
    Q_OBJECT
public:
    labelClicker(QWidget *parent);

    void setReservance(bool masiv[]);

signals:
    void placeReserved(int);

protected:
    bool eventFilter(QObject *lbl,QEvent *event);

private:
    QObject *mas[30];
    bool mas_bool[30];
    QWidget *parent_;
};

#endif // LABELCLICKER_H
