#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"

#include <QMainWindow>
#include <QtNetwork/QSctpServer>
#include <QtNetwork/QTcpSocket>

#include <vector>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const int PORT = 2644;

    QTcpServer server;

    void start_server();
    void stop_server();

    void accept_new_connection();
    void upload_current_file_list(Client *client);
    void on_peer_disconnect();
    void run_client(Client *client);        // handle client in thread

    void tokenH_upload_file(const Client *client);
    void tokenH_delete_file(const Client *client);
    void tokenH_download_file(const Client *client);

    void broad_cast(char msg[271]);

    std::vector<std::thread> peer_thread;
    std::vector<Client*> connections;       // list of connected peers
    int connected;                          // count of connected peers

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
