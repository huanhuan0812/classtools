#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->main_range_lab->setText("Range: "+QString::number(be)+" - "+QString::number(ed));
    connect(ui->main_settings_btn, &QPushButton::clicked, this, &MainWindow::openDialog);
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
        ui->main_range_lab->setText("Range: "+QString::number(be)+" - "+QString::number(ed));
    }
}
