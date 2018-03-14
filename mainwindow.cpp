#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoginDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //隐藏下载进度条
    ui->progressBar->hide();

    manager = new QNetworkAccessManager(this);
    //初始化请求
    request = new QNetworkRequest(
                QUrl("http://code.tarena.com.cn/"));
    //发送请求
    sendRequst();

    //点击界面上面链接时，发送信号anchorClicked，
    //参数是当前点击链接的URL地址
    connect(ui->textBrowser,SIGNAL(anchorClicked(QUrl)),
            this,SLOT(onAnchorClicked(QUrl)));

    //向容器中添加要支持的文件类型
    fileType << ".tar" << ".bz2";
    fileType << ".7z" << ".zip" << ".rar";
    fileType << ".txt" << ".pdf" << ".doc";
    fileType << ".c" << ".h" << ".ui" << ".pro";
    fileType << ".png" << ".jpeg" << ".bmp"<<".JPG";
}
MainWindow::~MainWindow()
{
    delete ui;
}
//发送请求
void MainWindow::sendRequst()
{
    //发送请求，返回响应对象
    reply = manager->get(*request);
    //如果请求的服务器需要登陆认证，将会发送信号
    //authenticationRequired()
    connect(manager,SIGNAL(authenticationRequired(
                   QNetworkReply*,QAuthenticator*)),
            this,SLOT(onAuthenticationRequest(
                   QNetworkReply*,QAuthenticator*)));
    //响应数据到来时，发送信号readyRead
    connect(reply,SIGNAL(readyRead()),
            this,SLOT(onReadyRead()));
    //响应数据接收结束
    connect(reply,SIGNAL(finished()),
            this,SLOT(onFinished()));
}
//处理登陆认证槽函数
void MainWindow::onAuthenticationRequest(
        QNetworkReply*,QAuthenticator* authenticator)
{
    qDebug("%s",__func__);
    //练习：增加登陆认证对话框
    LoginDialog login(this);
    //exec()：显示登陆对话框并进入事件循环
    //点击ok按钮时获取用户名和密码，执行accept，退出
    //exec，返回Accepted，然后下面登陆验证,如果认证
    //成功，则接收响应数据，如果认证失败，会再次发送
    //认证信号，当前槽函数重新执行，重新认证
    //点击cancle，执行reject，退出事件循环返回Rejected,
    //不做登陆认证，显示认证失败的界面
    if(login.exec() == QDialog::Accepted){
        authenticator->setUser(login.getUsername());
        authenticator->setPassword(login.getPassword());
    }
    //authenticator->setUser("tarenacode");
    //authenticator->setPassword("code_2013");
}
//接收响应数据
void MainWindow::onReadyRead()
{
    //读取响应数据
    QString replyBuf = reply->readAll();
    //显示数据界面
    ui->textBrowser->setText(replyBuf);
    //保存当前网址url
    current = reply->url();
}
//响应数据接收完成
void MainWindow::onFinished()
{
    qDebug("%s",__func__);
    reply->deleteLater();
    reply = NULL;
}

//处理链接的槽函数
void MainWindow::onAnchorClicked(const QUrl& url)
{
    //qDebug() << "当前url:" << current.toString();
    //qDebug() << "点击url:" << url.toString();
    /*eg：
     * 当前url: "http://code.tarena.com.cn/"
       点击url: "CSDCode/" */
    //如果要进入CSDCode目录链接中，发送新的请求，
    //对应url：http://code.tarena.com.cn/CSDCode/

    QUrl newUrl;//保存新的请求url
    if(url.toString() != "../"){
        newUrl = current.toString() + url.toString();

    }
    //处理”../“目录链接
    else{
        //如果当前在顶层目录链接中，点击”../“不处理
        if(current.toString() ==
                "http://code.tarena.com.cn/" ){
            return;
        }
        //如果不再顶层目录链接中，去掉最后一级路径
        //eg:
        //当前：http://xxx/xxx/xxx/CSDCode/
        //新的网址：http://xxx/xxx/xxx/
        else{
            //查找倒数第二次出现"/"位置
            int pos =
                current.toString().lastIndexOf("/",-2);
            //截取新的链接，从头到第二次出现"/"位置
            newUrl = current.toString().mid(0,pos+1);
        }
    }
    //判断新的链接网址对应的是不是要下载的文件，如果是
    //执行文件下载操作
    foreach(QString type,fileType){
        //qDebug() << type;
        int res = url.toString().lastIndexOf(type);
        if(res >= 0){
            //qDebug() << "下载文件：" << newUrl.toString();
            downloadFile(newUrl);
            return;
        }
    }
    //设置新的请求网址
    request->setUrl(newUrl);
    //发送新的请求
    sendRequst();
}

//下载文件操作，参数表示文件的URL
void MainWindow::downloadFile(const QUrl& fileUrl)
{
    //根据文件url，获取要下载文件名
    QFileInfo fileInfo(fileUrl.path());
    //弹出模态对话框，返回要下载的文件路径
    QString downloadPath = QFileDialog::getSaveFileName(this,"保存",fileInfo.fileName());
    QString filename(downloadPath);
    //在本地创建同名的文件,并打开
    file = new QFile(filename,this);
    //以写打开文件，如果文件不存在会自动创建
    file->open(QIODevice::WriteOnly);
    //发送获取文件请求
    request->setUrl(fileUrl);
    reply = manager->get(*request);
    //发送请求后，再收到的响应数据内容就是文件里面的内容
    connect(reply,SIGNAL(readyRead()),
            this,SLOT(receiveFile()));
    //当文件下载文件完成发送信号finished
    connect(reply,SIGNAL(finished()),
            this,SLOT(receiveFileFinish()));
    //伴随文件下载会发送信号downloadProgres
    //参数：
    //1）表示已经收到的字节数
    //2) 总字节数
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),
            this,SLOT(onDownloadProgress(qint64,qint64)));
    //显示下载进度条
    ui->progressBar->setValue(0);
    ui->progressBar->show();
}

//接收文件内容槽函数
void MainWindow::receiveFile(void){
    file->write(reply->readAll());
}
//文件下载完成
void MainWindow::receiveFileFinish(void)
{
    //qDebug("下载文件完成");
    file->flush();
    file->close();
    reply->deleteLater();
    //隐藏下载进度条
    ui->progressBar->hide();
}
//更新显示文件下载进度的槽函数
void MainWindow::onDownloadProgress(
        qint64 bytesRead,qint64 bytesTotal)
{
    //显示百分比=bytesRead/bytesTotal
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesRead);
}












