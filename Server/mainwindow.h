#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QSctpServer>
#include <QtNetwork/QTcpSocket>

#include <vector>

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

    std::vector<QTcpSocket*> connections;   // list of connected peers
    int connected;                          // count of connected peers
    std::vector<std::string> file_list;
private:
    Ui::MainWindow *ui;
    bool delete_file(char filename[]);
};
#endif // MAINWINDOW_H
