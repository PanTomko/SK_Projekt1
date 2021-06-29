#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <winsock2.h>
#include <winsock.h>

#include <windows.h>

#include <vector>
#include <queue>
#include <string>

#include <thread>
#include <mutex>

#include "token.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SOCKET sock;

    std::mutex active_transmition;

    bool read_socket;
    std::thread *th;                // for idle server connection

    bool is_running(){ return _running; }

    void connect_to_server();
    std::vector<std::string> get_current_files();
    void set_list_of_files( const std::vector<std::string>& file_list );

    void disconnect_form_server();
    void set_current_file_list();

    void handleToken_UPLOADED();
    void handleToken_DELETED();

    std::queue<TOKEN>tokens;

    void sendFile(QFile *file);
    QFile recvFile();

    void sendToken( TOKEN token );
    TOKEN recvToken();

public slots:
    void upload_file();
    void delete_file();
    void download_file();

    void handle_server_msg();

private:
    bool _running = true;
    Ui::MainWindow *ui;
    std::vector<std::string> file_list;
};
#endif // MAINWINDOW_H
