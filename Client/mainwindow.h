#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <winsock2.h>

#include <vector>
#include <string>

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

    void connect_to_server();
    std::vector<std::string> get_current_files();
    void set_list_of_files( const std::vector<std::string>& file_list );

    void disconnect_form_server();

public slots:
    void upload_file();
    void delete_file();
    void download_file();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
