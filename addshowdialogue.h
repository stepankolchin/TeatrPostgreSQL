#ifndef ADDSHOWDIALOGUE_H
#define ADDSHOWDIALOGUE_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class addShowDialogue;
}

class addShowDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit addShowDialogue(QWidget *parent = nullptr);
    ~addShowDialogue();
signals:
    void addShow(QString,QString,QString,QString,int);

private slots:
    void on_pushButton_Add_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::addShowDialogue *ui;
};

#endif // ADDSHOWDIALOGUE_H
