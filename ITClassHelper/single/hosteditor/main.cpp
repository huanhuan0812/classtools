#include <iostream>
#include <fstream>
#include <vector>
#include<windows.h>
#include<sstream>
using namespace std;
ofstream logout("log.txt");

struct Host {
    string ip;
    string site;
};

vector<Host> sites;
Host host;

bool skipBOM(std::ifstream &file) {
    char bom[3];
    if (file.read(bom, 3)) {
        if (bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF) {
            return true;  // BOM 存在，已跳过
        } else {
            file.seekg(0);  // 不是 BOM，重置读取位置
        }
    }
    return false;
}

int countstates(string str) {
    int count = 0;
    if (str.empty()) {
        return 0;
    }
    if (str[0] != ' ') {
        count++;
    }
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            for (;i<str.length();i++) {
                if (str[i] != ' ') {
                    break;
                }
            }
            count++;
        }
    }
    return count;
}

bool isRunAsAdmin(){

    BOOL isAdmin = FALSE;
    PSID adminGroup;

    // 创建管理员组的SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0,
                                 &adminGroup)) {
        return false;
                                 }

    // 检查当前令牌是否属于管理员组
    if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
        isAdmin = FALSE;
    }

    FreeSid(adminGroup);
    return isAdmin;
}

void requestAdminPrivileges(){
    wchar_t path[MAX_PATH];
    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = path;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (!ShellExecuteExW(&sei)) {
            DWORD err = GetLastError();
            if (err == ERROR_CANCELLED) {
                logout << "User refuses to elevate permissions\n";
            }
        }
        else {
            logout << "User accepts to elevate permissions\n";
        }
    }
}

bool isrun() {
    HANDLE hMutex = CreateMutex(NULL, TRUE, reinterpret_cast<LPCSTR>(L"hosteditor"));

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        return true;
    }
    return false;
}

int main() {
    if (isrun()) {
        return 0;
    }
    string version="0.1";
    string filename;
    std::ifstream fin("C:\\Windows\\System32\\drivers\\etc\\hosts");
    logout<<"open hosts\n";
    if (!fin.is_open()) {
        fin.open("C:\\Windows\\System32\\drivers\\etc\\HOSTS");
        logout <<"open HOSTS\n";
        if (!fin.is_open()) {
            logout<< "无法打开文件\n";
            return 0;
        }
        else {
            filename="C:\\Windows\\System32\\drivers\\etc\\HOSTS";
        }
    }
    else filename="C:\\Windows\\System32\\drivers\\etc\\hosts";
    skipBOM(fin);
    string site;
    while (std::getline(fin, site)) {
        //logout<<site<<"\n";
        if (site.empty() || site[0] == '#') {
            continue; // 跳过空行和注释行
        }
        std::istringstream iss(site);
        if (iss >> host.ip >> host.site) {
            sites.push_back(host);
        }
    }
    logout<<"read finished\n";
    fin.close();
    //
    // //requestAdminPrivileges();
    string ip;
    string mod;
    string line;
    int states=0;
    cout<<">>";
    ofstream fout;
    while (true) {
        getline(cin, line);
        if (line == "exit") {
            break;
        }
        if (line == "list") {
            if (sites.size()==0) {
                cout<<"hosts is empty\n";
                cout<<">>";
                continue;
            }
            for (int i = 0; i < sites.size(); i++) {
                cout << sites.at(i).site << " " << sites.at(i).ip << endl;
            }
            cout<<">>";
            continue;
        }
        if (line=="version"){cout<<"hosteditor command tool v"<<version<<"\n>>";continue;}
        if (line=="help") {
            cout<<"add <ip> <site>\tadd a new site\n";
            cout<<"list\tlist all sites\n";
            cout<<"exit\texit the program\n";
            cout<<"help\tshow this help\n";
            cout<<"version\tshow version\n";
            cout<<">>";
            continue;
        }
        states=countstates(line);
        std::istringstream iss(line);
        iss >> mod;
        if (mod == "add") {
            if (states<3) {
                if (states==2) cerr << "error:IP address or site name was minssing\n";
                else if (states==1) cerr << "error:parameters were missing\n";
                cout<<">>";
            }
            else {
                iss >> site>> ip ;
                host.ip = ip;
                host.site = site;
                bool has=false;
                for (int i = 0; i < sites.size(); i++) {
                    if (sites.at(i).site == host.site) {
                        has=true;
                        cout<<host.site<<"has already presented\ndo you want to modify it?(y/n):";
                        string ans;
                        cin>>ans;
                        if (ans == "y"|| ans == "Y") {
                            sites.at(i).ip = host.ip;//------------------替换程序
                            cout << "modify successfully\n>>";
                            break;
                        }
                        if (ans == "n"|| ans == "N") {
                            cout << "modify cancelled\n>>";
                            break;
                        }
                        cout << "invalid answer\n>>";
                        break;
                    }
                }
                if (!has) {
                    sites.push_back(host);
                    cout << "add successfully\n>>";
                    // fout.open(filename, std::ios::app);
                    // if (!fout.is_open()) {
                    //     cerr << "无法打开文件\n";
                    //     return 0;
                    // }
                    // fout << host.ip << " " << host.site << "\n";
                    // fout.close();
                }
            }
        }
        else {
            cerr << "unknown command"<<endl;
            cout<<"type \"help\" for help"<<endl;
            cout<<">>";
        }
    }
    return 0;
}
/*

bool MainWindow::isRunAsAdmin(){

    BOOL isAdmin = FALSE;
    PSID adminGroup;

    // 创建管理员组的SID
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0,
                                 &adminGroup)) {
        return false;
                                 }

    // 检查当前令牌是否属于管理员组
    if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
        isAdmin = FALSE;
    }

    FreeSid(adminGroup);
    return isAdmin;
}

void MainWindow::requestAdminPrivileges(){
    wchar_t path[MAX_PATH];
    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = path;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (!ShellExecuteExW(&sei)) {
            DWORD err = GetLastError();
            if (err == ERROR_CANCELLED) {
                qDebug() << "用户拒绝提升权限";
            }
        }
    }
}
*/