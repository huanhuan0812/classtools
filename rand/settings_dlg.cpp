//
// Created by qdhua on 25-3-9.
//
#include"settings_dlg.h"
SettingDialog::SettingDialog(QWidget *parent)
        : QDialog(parent)
{
    // 创建控件


    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Enter something:"));
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    // 设置布局
    setLayout(layout);

    // 连接信号和槽
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}