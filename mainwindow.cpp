#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QFileDialog>
#include <QProcess>
#include <QtSerialPort/QSerialPort>
#include <QSignalMapper>
#include <QTimer>
#include <QSerialPortInfo>

#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    // make sure only comport of the atmega168 is getting obaint
    foreach(const QSerialPortInfo &portInfo,QSerialPortInfo::availablePorts())
    {
        if(portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier())
        {
            qDebug () << "vendorID: " << portInfo.vendorIdentifier();
            qDebug () << "productID: " << portInfo.productIdentifier();
            if(portInfo.vendorIdentifier() == vendorID)
            {
                if(portInfo.productIdentifier() == productID)
                {
                    qDebug() << "COM: " << portInfo.portName();
                    comPort = portInfo.portName();
                }
            }
        }
    }




}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnProgram_clicked()
{
    QDir::setCurrent("D:/Tomek/ESP8266/Scripts");
    QString script = ui->listScripts->currentItem()->text();
    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("run");
    arguments.append("-n");
    arguments.append(script);


    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();
}

void MainWindow::on_btnContent_clicked()
{
    ui->txtbox->clear();
    QString file;
    file =  ui->listFilesESP->currentItem()->text();
    QString tmp;
    QString str;
    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("get");
    arguments.append(file);

    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

    while(!process->atEnd())
    {
        tmp = process->readLine();
        qDebug() << tmp;

        str = tmp.replace("\r"," ");
        str = tmp.replace("\n"," ");
        qDebug() << str;

        ui->txtbox->append(str);

    }
}

void MainWindow::on_btnFiles_clicked()
{
    showFilesFromESP();
}

void MainWindow::on_btnScripts_clicked()
{
showScriptsOnPC();
}

void MainWindow::showFilesFromESP()
{
    ui->listFilesESP->clear();
    QString prog = "ampy";
    int len = 0;
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("ls");



    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

    QString tmp = process->readAll();
    len = tmp.length();

    QCharRef c = tmp[len-1];
    tmp.replace(c," "); // \r weg
    tmp =  tmp.remove(len-1,1);
    len = tmp.length();
    tmp.remove("\r");
    len = tmp.length();
    list = tmp.split(" ");
    ui->listFilesESP->addItems(list);
}

void MainWindow::showScriptsOnPC()
{
    ui->listScripts->clear();
    QDir direct;
    QDir::setCurrent("D:/Tomek/ESP8266/Scripts/");
    QStringList lst;
    lst = direct.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);//(QDir::Filter::Files,QDir::SortFlag::NoSort);
    ui->listScripts->addItems(lst);
}


void MainWindow::on_btnRemove_clicked()
{

    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("rm");
    arguments.append(ui->listFilesESP->currentItem()->text());



    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();


}

void MainWindow::on_btnUploadESP_clicked()
{
    QDir::setCurrent("D:/Tomek/ESP8266/Scripts/");

    QString script = ui->listScripts->currentItem()->text();
    qDebug() << script;

    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("put");
    arguments.append(script);

    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

}

void MainWindow::on_btnSetMain_clicked()
{
    QString script = ui->listScripts->currentItem()->text();
    QDir::setCurrent("D:/Tomek/ESP8266/Scripts/");

    qDebug() << script;
    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("put");
    arguments.append(script);
    arguments.append("/main.py");

    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();
}

void MainWindow::on_btnUploadModule_clicked()
{
    QDir::setCurrent("D:/Tomek/ESP8266/Modules/");

    QString module = ui->listModule->currentItem()->text();

    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("put");
    arguments.append(module);

    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();
}

void MainWindow::on_btnModules_clicked()
{
    ui->listModule->clear();
    QDir direct;
    QDir::setCurrent("D:/Tomek/ESP8266/Modules/");
    QStringList lst;
    lst = direct.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);//(QDir::Filter::Files,QDir::SortFlag::NoSort);
    ui->listModule->addItems(lst);
}
