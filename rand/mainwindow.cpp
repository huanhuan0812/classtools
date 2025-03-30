#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->count_warn_lab->setStyleSheet("color: red;");;
    ui->main_range_lab->setText("Range: "+QString::number(be)+" - "+QString::number(ed));
    connect(ui->main_settings_btn, &QPushButton::clicked, this, &MainWindow::openDialog);
    connect(ui->start_btn, &QPushButton::clicked, this, &MainWindow::startrand);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDialog(){
    SUi::settingdlg dialog(this);
    if (dialog.exec()==QDialog::Accepted) {
        be=dialog.be;
        ed=dialog.ed;
        lang=dialog.lang;
        bannednum=dialog.bannednum;
        len=ed-be+1;
        ui->main_range_lab->setText("Range: "+QString::number(be)+" - "+QString::number(ed));
    }
}

void MainWindow::startrand() {
    cnt=ui->count_ledit->text().toInt();
    if (cnt<=0) {
        ui->count_warn_lab->setText("Please input a positive number");
        return;
    }
    else if (cnt>len) {
        ui->count_warn_lab->setText("Please input a number less than "+QString::number(len));
    }
    else {
        ui->count_warn_lab->clear();
    }
}