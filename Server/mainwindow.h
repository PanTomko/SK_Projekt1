#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QSctpServer>
#include <QtNetwork/QTcpSocket>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
