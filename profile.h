#ifndef PROFILE_H
#define PROFILE_H

#include <QDialog>

namespace Ui {
class profile;
}

class profile : public QDialog
{
    Q_OBJECT

public:
    explicit profile(QWidget *parent = nullptr);
    ~profile();


    void setName(QString name);

    void setSurname(QString surname);

    void setSex(QString sex);

    void setNick(QString nick);
    void setPassword(QString pwd);


    QString getName();

    QString getSurname();

    QString getSex();

    QString getNick();

    QString getPassword();

signals:
    void changeData(QString,QString,QString,QString);

    void changePwd(QString);

private slots:
    void on_pushButton_chinf_clicked();

    void on_pushButton_saveinf_clicked();

    void on_pushButton_chpwd_clicked();

    void on_pushButton_saveinf_savepwd_clicked();


    void on_pushButton_shpwd_pressed();

    void on_pushButton_shpwd_released();

private:
    Ui::profile *ui;
};

#endif // PROFILE_H
