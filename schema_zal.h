#ifndef SCHEMA_ZAL_H
#define SCHEMA_ZAL_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <labelclicker.h>

namespace Ui {
class schema_zal;
}

class schema_zal : public QDialog
{
    Q_OBJECT

public:
    explicit schema_zal(QWidget *parent = nullptr);
    ~schema_zal();

    void setMassiv(bool mas[]);
    void setZal(int);
public slots:
    void reservePlace(int);
signals:
    void placeReserved(int,int);
private:
    Ui::schema_zal *ui;

//    bool eventFilter(QObject*,QEvent*);

    QObject *mas[30];
    bool mas_bool[30];
    labelClicker *mas_ev_filters[30];


    void makeRed(QWidget *lbl);
    void makeGreen(QWidget *lbl);
};

#endif // SCHEMA_ZAL_H
