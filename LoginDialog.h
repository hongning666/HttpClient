#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
public:
    //获取用户名和密码
    const QString& getUsername();
    const QString& getPassword();
private:
    Ui::LoginDialog *ui;
    //保存输入的用户名和密码
    QString m_username;
    QString m_password;
};

#endif // LOGINDIALOG_H






