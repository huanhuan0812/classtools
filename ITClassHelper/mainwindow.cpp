#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include<QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化网络监控
    //设置窗口透明
    //setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint /*| Qt::Tool*/);
    this->setAttribute(Qt::WA_TranslucentBackground); //背景透明
    //this->setWindowOpacity(0.1);
    this->setWindowFlag(Qt::WindowTransparentForInput,false);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    //this->ui->centralwidget->setWindowOpacity(0.1);
    //this->setFocusPolicy(Qt::StrongFocus);
    //connect(this->ui->close_btn, SIGNAL(clicked()), this, SLOT(on_closeBtn_clicked()));
    //connect(this->ui->ok_btn,&QPushButton::clicked,this,&MainWindow::on_cnt_add);
    this->activateWindow();
    this->setFocus();
    initNetworkMonitoring();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initNetworkMonitoring()
{
    // 尝试加载网络信息后端
    if (!QNetworkInformation::loadDefaultBackend()) {
        QMessageBox::warning(this, "警告", "无法加载网络信息后端");
        return;
    }
    m_netInfo = QNetworkInformation::instance();
    if (!m_netInfo) {
        return;
    }
    // 初始状态更新
    updateNetworkStatus(m_netInfo->reachability());
    // 连接状态变化信号
    connect(m_netInfo, &QNetworkInformation::reachabilityChanged,
            this, &MainWindow::updateNetworkStatus);
}

void MainWindow::updateNetworkStatus(QNetworkInformation::Reachability reachability)
{
    // 根据状态改变窗口的显示方式
    if (reachability == QNetworkInformation::Reachability::Online) {
        //this->showMinimized();
    } else {
        this->showFullScreen();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() & Qt::ShiftModifier) &&
        (event->modifiers() & Qt::AltModifier) &&
        (event->modifiers() & Qt::MetaModifier) &&
        (event->key() == Qt::Key_P)) {
        // 检测到 Shift + Alt + Meta + P 键组合
        this->close();
        } else {
            event->ignore();
        }
}

void MainWindow::on_closeBtn_clicked()
{
    QProcess process;
    process.startDetached("shutdown", QStringList() << "/s" << "/t" << "0");
}

void MainWindow::on_cnt_add()
{
    QMessageBox::critical(this, "错误", "密码错误");
    if (this->ui->lineEdit_2->text()==":|.]") {
        this->close();
    }
    this->ui->lineEdit_2->clear();
    cnt++;
    if (cnt >= 5) {
        this->on_closeBtn_clicked();
    }
}