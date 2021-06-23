#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->B_Download, &QPushButton::clicked, this, &MainWindow::download_file);
    connect(ui->B_Upload, &QPushButton::clicked, this, &MainWindow::upload_file);
    connect(ui->B_Delete, &QPushButton::clicked, this, &MainWindow::delete_file);
}

MainWindow::~MainWindow()
{
    closesocket(sock);

    _running = false;
    th->join();

    delete th;
    delete ui;
}

void MainWindow::connect_to_server()
{
    // ini
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);

    if(WSAStartup(version, &wsData) != 0)
        std::cout << "error winsock version!" << std::endl;

    // create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
         std::cout << "error socket !" << std::endl;

    // server info
    SOCKADDR_IN addr;
    addr.sin_port = htons(2644);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    // set up
    DWORD timeout = 1 * 1000;
    ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);

    if(::connect(sock, (const sockaddr *)&addr, sizeof(addr)) != 0)
        std::cout << "Error connect !" << std::endl;

    read_socket = sock;

    th = new std::thread( &MainWindow::handle_server_msg, this );
}

void MainWindow::upload_file()
{
    read_socket = NULL;
    active_transmition.lock();

    char buffer[16] = "UP";
    ::send(sock, buffer, sizeof(buffer), 0);

    active_transmition.unlock();
    read_socket = sock;
}

void MainWindow::delete_file()
{
    read_socket = NULL;
    active_transmition.lock();

    // code

    active_transmition.unlock();
    read_socket = sock;
}

void MainWindow::download_file()
{
    read_socket = NULL;
    active_transmition.lock();

    // code

    active_transmition.unlock();
    read_socket = sock;
}

void MainWindow::handle_server_msg()
{
    char buffer[271];
    while(is_running())
    {
        recv( read_socket, buffer, 271, 0 );

        if(read_socket == NULL) continue; // skip locking if socket is not active

        active_transmition.lock();
        active_transmition.unlock();
    }
}
