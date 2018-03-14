#include "LoginDialog.h"
#include "ui_LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
//ok按钮对应的槽函数
void LoginDialog::on_buttonBox_accepted()
{
    m_username = ui->usernameEdit->text();
    m_password = ui->passwordEdit->text();
    accept();//退出登陆窗口的事件循环，返回1
}
//cancel按钮对应的槽函数
void LoginDialog::on_buttonBox_rejected()
{
    reject();//退出登陆窗口的事件循环，返回0
}
//获取用户名和密码
const QString& LoginDialog::getUsername()
{
    return m_username;
}
const QString& LoginDialog::getPassword()
{
    return m_password;
}










