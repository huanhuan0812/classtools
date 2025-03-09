//
// Created by qdhua on 25-3-9.
//

#ifndef RAND_SETTINGS_DLG_H
#define RAND_SETTINGS_DLG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SettingDialog : public QDialog
{
Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);

private:
    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton= new QPushButton("Cancel", this);
};



#endif //RAND_SETTINGS_DLG_H
