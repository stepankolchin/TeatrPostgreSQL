#ifndef BASKET_VIEW_H
#define BASKET_VIEW_H

#include <QDialog>
#include <mybutton.h>
#include <QtSql/QSqlQuery>

namespace Ui {
class basket_view;
}

class basket_view : public QDialog
{
    Q_OBJECT

public:
    explicit basket_view(QWidget *parent = nullptr);
    ~basket_view();

    void setBasket(QSqlQuery * qw);

public slots:
    void delTicket(QString btn_name);

signals:
    void delTicket(int ticket_id);


private slots:
    void on_pushButton_close_clicked();

private:
    Ui::basket_view *ui;
};

#endif // BASKET_VIEW_H
