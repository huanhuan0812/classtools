#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInformation>
#include <QLabel>
#include<QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void updateNetworkStatus(QNetworkInformation::Reachability reachability);
private:
    Ui::MainWindow *ui;
    QNetworkInformation *m_netInfo = nullptr;
    void initNetworkMonitoring();
    QString reachabilityToString(QNetworkInformation::Reachability reachability);
    void keyPressEvent(QKeyEvent *event);
    void on_closeBtn_clicked();
    int cnt=0;
    void on_cnt_add();
    void writehost();
    void requestAdminPrivileges();
    bool isRunAsAdmin();
};
#endif // MAINWINDOW_H