#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    connect(ui->main_settings_btn, &QPushButton::clicked, this, &MainWindow::openDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}
