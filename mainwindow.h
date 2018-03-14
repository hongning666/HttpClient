#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QAuthenticator>//处理登陆认证
#include <QDebug>
#include <QThread>
#include "workthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    //发送请求
    void sendRequst();
    void run();
public slots:
    //处理登陆认证槽函数
    void onAuthenticationRequest(
            QNetworkReply*,QAuthenticator*);
    //接收响应数据
    void onReadyRead();
    //响应数据接收完成
    void onFinished();
public slots:
    //处理链接的槽函数
    void onAnchorClicked(const QUrl&);
public slots:
    void receiveFile();
    void receiveFileFinish();
    void onDownloadProgress(qint64,qint64);
public:
    //下载文件操作，参数表示文件的URL
    void downloadFile(const QUrl& fileUrl);
private:
    Ui::MainWindow *ui;

    QNetworkAccessManager* manager;//管理通信过程
    QNetworkRequest* request;//请求
    QNetworkReply* reply;//响应
    QUrl current;//保存当前网页的网址
    QFile *file;
    //定义容器，保存所有要支持下载的文件类型
    QList <QString> fileType;
};

#endif // MAINWINDOW_H




