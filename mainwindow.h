#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
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
    void on_btnFiles_clicked();

    void on_btnProgram_clicked();
    void on_btnContent_clicked();
    void on_btnScripts_clicked();

    void showFilesFromESP();
    void showScriptsOnPC();


    void on_btnRemove_clicked();

    void on_btnUploadESP_clicked();

    void on_btnSetMain_clicked();

    void on_btnUploadModule_clicked();

    void on_btnModules_clicked();

private:
    Ui::MainWindow *ui;

    QString comPort;

    quint16  vendorID = 4292;
    quint16  productID = 60000;
    QStringList list;
    QProcess *proc;

};

#endif // MAINWINDOW_H
