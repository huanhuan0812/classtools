#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"settings_dlg.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void openDialog()
    {
        SettingDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            QMessageBox::information(this, "Info", "You clicked OK!");
        } else {
            QMessageBox::information(this, "Info", "You clicked Cancel!");
        }
    }
};

#endif // MAINWINDOW_H
