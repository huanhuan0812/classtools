//
// Created by qdhua on 25-3-23.
//

#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>
#include<unordered_map>
#include<QCloseEvent>

namespace SUi {
    QT_BEGIN_NAMESPACE
    namespace Ui { class settingdlg; }
    QT_END_NAMESPACE
    class settingdlg : public QDialog {
        Q_OBJECT
    public:
        explicit settingdlg(QWidget *parent = nullptr);
        ~settingdlg() override;
        std::string lang="en_us";
        int be=0,ed=0;
        std::unordered_map<int,int>bannednum;

    private:
        Ui::settingdlg *ui;
    protected:
        void accept();
        void closeEvent(QCloseEvent *event);
    };
} // SUi

#endif //SETTINGDLG_H
