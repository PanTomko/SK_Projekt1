#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <winsock2.h>
#include <winsock.h>

#include <vector>
#include <string>

#include <thread>
#include <mutex>

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

    SOCKET read_socket;             // socket for idle connection
    std::thread *th;                // for idle server connection

    bool is_running(){ return _running; }

    void connect_to_server();
    std::vector<std::string> get_current_files();
    void set_list_of_files( const std::vector<std::string>& file_list );

    void disconnect_form_server();
    void set_current_file_list();

public slots:
    void upload_file();
    void delete_file();
    void download_file();

    void handle_server_msg();

private:
    bool _running = true;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
