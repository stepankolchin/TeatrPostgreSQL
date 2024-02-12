#ifndef ADDSHOWDIALOGUE_H
#define ADDSHOWDIALOGUE_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
namespace Ui {
class addShowDialogue;
}

class addShowDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit addShowDialogue(QWidget *parent = nullptr);
    ~addShowDialogue();
    void setName(QString);
    void setDate(QString);
    void setDescription(QString);
    void setTime(QString);
    void setZal(QString);
    void setKnopka();


signals:
    void addShow(QString,QString,QString,QString,int);

    void changeShow(QString,QString,QString,QString,int);

private slots:
    void on_pushButton_Add_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::addShowDialogue *ui;
    bool fl;

};

#endif // ADDSHOWDIALOGUE_H
