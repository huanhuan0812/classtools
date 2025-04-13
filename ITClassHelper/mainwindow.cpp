#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->m_refreshButton->setText("刷新状态");
    // 连接按钮信号
    connect(ui->m_refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    // 尝试加载网络信息后端
    if (!QNetworkInformation::loadDefaultBackend()) {
        QMessageBox::warning(this, "警告", "无法加载网络信息后端");
        return;
    }
    // 初始状态更新
    updateNetworkStatus(m_netInfo->reachability());
    // 连接状态变化信号
    connect(m_netInfo, &QNetworkInformation::reachabilityChanged,this, &MainWindow::updateNetworkStatus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateNetworkStatus(QNetworkInformation::Reachability reachability)
{
    if (reachability == QNetworkInformation::Reachability::Online) {
        ui->m_statusLabel->setText("网络连接正常");
    } else {
        ui->m_statusLabel->setText("网络连接异常");
    }
}

void MainWindow::onRefreshClicked()
{
    if (m_netInfo) {
        updateNetworkStatus(m_netInfo->reachability());
    } else {
        QMessageBox::warning(this, "警告", "无法加载网络信息后端");
    }
}