#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    stop_server();
    delete ui;
}

void MainWindow::start_server()
{
    server.listen(QHostAddress::Any, PORT);
    std::cout << server.errorString().toStdString() << std::endl;
    std::cout << "test" << std::endl;
}

void MainWindow::stop_server()
{
    server.close();

    for(auto & x : connections){
        x->close();
    }
}


void MainWindow::accept_new_connection()
{
    if(server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection();
        std::cout << "peer : " << socket->socketDescriptor() << " connected !" << std::endl;

        connected++;
        connections.push_back( socket );
    }
}

bool MainWindow::delete_file(char filename[])
{
    char path[65] = "E:\\Rest\\Drive\\";
    strcat(path, filename);

    if (remove(path) != 0){
        perror("File deletion failed");
        return false;
    }
    else {
        file_list.erase(std::remove(file_list.begin(), file_list.end(), filename), file_list.end());
        std::cout << "File deleted successfully";
        return true;
    }
}
