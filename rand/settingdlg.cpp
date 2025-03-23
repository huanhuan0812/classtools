//
// Created by qdhua on 25-3-23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_settingdlg.h" resolved

#include "settingdlg.h"
#include "ui_settingdlg.h"
#include<QString>

namespace SUi {
    settingdlg::settingdlg(QWidget *parent) :
        QDialog(parent), ui(new Ui::settingdlg) {
        ui->setupUi(this);
        ui->lang_lab->setText(QString::fromStdString(lang));
        connect(ui->ok_btn, &QPushButton::clicked, this, &QDialog::accept);
        connect(ui->cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
    }
    settingdlg::~settingdlg() {
        delete ui;
    }

    void settingdlg::accept() {
        be = ui->begin_sbox->value();
        ed = ui->end_sbox->value();
        QDialog::accept();
    }
    void settingdlg::closeEvent(QCloseEvent *event) {
        QDialog::reject();
    }

} // SUi



