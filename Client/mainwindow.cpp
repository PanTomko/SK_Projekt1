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

    if(::connect(sock, (const sockaddr *)&addr, sizeof(addr)) != 0)
        std::cout << "Error connect !" << std::endl;
}

void MainWindow::upload_file()
{
    char buffer[] = "UP";
    char file_buffer[] = "XXX";

    ::send(sock, buffer, sizeof(buffer), 0);
    ::send(sock, file_buffer, sizeof(file_buffer), 0);

    std::cout << sizeof(buffer) << std::endl;
}

void MainWindow::delete_file()
{

}

void MainWindow::download_file()
{

}

