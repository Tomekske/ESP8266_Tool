#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnProgram_clicked();
    void on_btnContent_clicked();

    void showFilesFromESP();
    void showScriptsOnPC();
    void showModulesOnPC();


    void on_btnRemove_clicked();
    void on_btnUploadESP_clicked();
    void on_btnSetMain_clicked();
    void on_btnUploadModule_clicked();

    void on_btnReset_clicked();
    void on_actionScript_folder_triggered();
    void saveSettings();
    void loadSettings();

    void on_actionLibrary_folder_triggered();


private:
    Ui::MainWindow *ui;

    QString comPort;
    QSerialPort *serial;
    quint16  vendorID = 4292;
    quint16  productID = 60000;
    QStringList list;
    QProcess *proc;
    QByteArray m_buffer;

    QString location_scripts = "c:/";
    QString location_modules = "c:/";
    QTimer *timerSerial; // timer for serial

    bool serialFlag = false;
    bool serialConnection = false;


};

#endif // MAINWINDOW_H
