#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include<QProcess>
#include<windows.h>
#include <QFile>
#include<fstream>
#include<Qvector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化封禁网址
    writehost();

    ui->setupUi(this);
    // 初始化网络监控
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

void MainWindow::writehost() {

    std::ifstream initin;
    initin.open("weblist.txt");
    if (!initin.is_open()) {
        qDebug() << "无法打开文件";
        return;
    }
    QVector<QString> sites;
    std::string line;
    while (initin>>line) {
        sites.push_back(QString::fromStdString(line));
    }



    std::ifstream fin("C:\\Windows\\System32\\drivers\\etc\\hosts");
    if (!fin.is_open()) {
        qDebug() << "无法打开文件";
    }
    else {
        fin.open("C:\\Windows\\System32\\drivers\\etc\\HOSTS");
        if (!fin.is_open()) {
            qDebug() << "无法打开文件";
            return;
        }
    }
    QString site;
    std::string tmpsite;
    while (!fin.eof()) {
        fin>>tmpsite;
        if (tmpsite == "//") {
            std::getline(fin, line);
            continue;
        }
        site= QString::fromStdString(tmpsite);
        for (int i = 0; i < sites.size(); i++) {
            if (sites.at(i) == site) {
                sites.removeAt(i);
                i--;
            }
        }
    }

    if (!isRunAsAdmin()) {
        QMessageBox::warning(this, "警告", "需要管理员权限运行程序");
        requestAdminPrivileges();
        if (!isRunAsAdmin()) {
            QMessageBox::warning(this, "警告", "无法获取管理员权限");
            return;
        }
        return;
    }
    // std::ofstream fout(hostsPath.toStdString(), std::ios::app);
    // if (!fout.is_open()) {
    //     qDebug() << "无法打开文件";
    //     return;
    // }
    // for (int i = 0; i < sites.size(); i++) {
    //     fout<<sites.at(i).toStdString()<<" "<<"192.108.1.1\n";
    // }
    // fout.close();
    // initin.close();
    // fin.close();
}

bool MainWindow::isRunAsAdmin(){

    BOOL isAdmin = FALSE;
    PSID adminGroup;

    // 创建管理员组的SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0,
                                 &adminGroup)) {
        return false;
                                 }

    // 检查当前令牌是否属于管理员组
    if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
        isAdmin = FALSE;
    }

    FreeSid(adminGroup);
    return isAdmin;
}

void MainWindow::requestAdminPrivileges(){
    wchar_t path[MAX_PATH];
    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = path;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (!ShellExecuteExW(&sei)) {
            DWORD err = GetLastError();
            if (err == ERROR_CANCELLED) {
                qDebug() << "用户拒绝提升权限";
            }
        }
    }
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