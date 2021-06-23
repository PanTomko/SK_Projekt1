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
    DWORD timeout = 5 * 1000;
    ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);


    if(::connect(sock, (const sockaddr *)&addr, sizeof(addr)) != 0)
        std::cout << "Error connect !" << std::endl;

    th = new std::thread( &MainWindow::handle_server_msg, this );
}

void MainWindow::upload_file()
{
    active_transmition.lock();

    char buffer[16] = "UP";
    ::send(sock, buffer, sizeof(buffer), 0);

    active_transmition.unlock();
}

void MainWindow::delete_file()
{

}

void MainWindow::download_file()
{

}

void MainWindow::handle_server_msg()
{
    char buffer[271];
    while(is_running())
    {
        recv( sock, buffer, 271, 0 );
        active_transmition.lock();
        active_transmition.unlock();
    }
}
