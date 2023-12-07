#ifndef DELETESHOWDIALOGUE_H
#define DELETESHOWDIALOGUE_H

#include <QDialog>
#include <QMessageBox>
#include <mybutton.h>
#include <QtSql/QSqlQuery>

namespace Ui {
class deleteShowDialogue;
}

class deleteShowDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit deleteShowDialogue(QWidget *parent = nullptr);
    ~deleteShowDialogue();

    void setList(QSqlQuery *qw);
signals:
    void deleteShow(int);

private slots:
    void on_pushButton_Close_clicked();

    void delShow(QString btn_name);

private:
    Ui::deleteShowDialogue *ui;
};

#endif // DELETESHOWDIALOGUE_H
