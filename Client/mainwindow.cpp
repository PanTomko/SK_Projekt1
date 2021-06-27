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
    addr.sin_port = htons(2466);
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if(::connect(sock, (const sockaddr *)&addr, sizeof(addr)) != 0)
        std::cout << "Error connect !" << std::endl;

    read_socket = true;

    set_current_file_list();

    th = new std::thread( &MainWindow::handle_server_msg, this );   
}

void MainWindow::set_current_file_list()
{
}

void MainWindow::sendToken(TOKEN token)
{
    ::send(sock, (char*)&token, sizeof(TOKEN), 0);
}

TOKEN MainWindow::recvToken()
{
    TOKEN token;
    recv( sock, (char*)&token, sizeof(TOKEN), 0 );
    return token;
}

void MainWindow::upload_file()
{
    std::cout << "upload start." << std::endl;

    sendToken(TOKEN::TOKEN_UPLOAD);

    // wait
    if( recvToken() != TOKEN::TOKEN_OK){
        std::cout << "upload abort." << std::endl;
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select file.", NULL, NULL);
    QFileInfo info(path);
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    // send file_name
    char file_name[255];
    strcpy(file_name, info.fileName().toStdString().c_str());
    ::send(sock, file_name, 255, 0);

    // wait
    if( recvToken() != TOKEN::TOKEN_OK){
        std::cout << "upload abort." << std::endl;
        return;
    }

    // send file size
    qint64 file_size = file.size();
    ::send(sock, (char*)&file_size, sizeof(qint64), 0);

    // wait
    if( recvToken() != TOKEN::TOKEN_OK){
        std::cout << "upload abort." << std::endl;
        return;
    }

    sendFile(&file);

    file.close();
    read_socket = true;
}

void MainWindow::delete_file()
{
    read_socket = true;
}

void MainWindow::download_file()
{
    std::cout << "download start." << std::endl;

    QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();

    if(selected.empty()){
        QMessageBox msg_box;
        msg_box.setText("No file was selected");
        msg_box.exec();
        return;
    }

    QString save_path = QFileDialog::getExistingDirectory(this, tr("Select save directory"),
                                                          NULL,
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);

    sendToken(TOKEN::TOKEN_DOWNLOAD);

    // wait
    if( recvToken() != TOKEN::TOKEN_OK){
        std::cout << "upload abort." << std::endl;
        return;
    }

    // send file_name
    char file_name[255];
    strcpy(file_name, selected[0]->text().toStdString().c_str());
    ::send(sock, file_name, 255, 0);

    // wait
    if( recvToken() != TOKEN::TOKEN_OK){
        std::cout << "upload abort." << std::endl;
        return;
    }

    // receive file
    qint64 file_size;
    recv( sock, (char*)&file_size, sizeof(qint64), 0 );
    std::cout << "file_size : " << file_size << '.' << std::endl;

    std::cout << "saved at : " << (save_path + file_name).toStdString() << std::endl;
    QFile file(save_path + "/" + file_name);
    file.open(QIODevice::WriteOnly);

    while(file_size != 0)
    {
        char packet[1024*4];
        int received = recv( sock, packet, sizeof(1024*4), 0 );
        file.write(packet, received);
        file_size -= received;
    }

    file.close();

    std::cout << "end." << std::endl;
    read_socket = true;
}

void MainWindow::handleToken_UPLOADED()
{
    std::cout << "token : TOKEN_UPLOADED" << std::endl;
    char file_name[255];
    recv( sock, file_name, sizeof(file_name), 0 );
    ui->listWidget->addItem(file_name);
}

void MainWindow::handleToken_DELETED()
{
    std::cout << "token : TOKEN_DELETED" << std::endl;
}

void MainWindow::sendFile(QFile *file)
{
    qintptr total_sended = file->size();
    while (total_sended != 0)
    {
        QByteArray data = file->read(1024*4);
        int sended = 0;
        while(sended != data.size()){
            sended += ::send(sock, data.data() + sended, data.size() - sended, 0);
        }
        total_sended -= sended;
    }
}

QFile MainWindow::recvFile()
{

}

void MainWindow::handle_server_msg()
{
    while(is_running())
    {
        if(!read_socket) continue; // skip locking if socket is not active

        TOKEN token = recvToken();

        if(token != TOKEN::TOKEN_UPLOADED ) // non brodcast token
        {
            read_socket = false;
        }
        else
        {
            if(token == TOKEN::TOKEN_UPLOADED) handleToken_UPLOADED();
            else if(token == TOKEN::TOKEN_DELETED) handleToken_DELETED();
            else std::cout << "wrong  token." << std::endl;
        }
    }
}
