#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QFileDialog>
#include <QProcess>

#include <QSignalMapper>
#include <QTimer>

#include <QDialog>
#include <QDateTime>
#include <QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

loadSettings();

    // make sure only comport of the atmega168 is getting obaint
    foreach(const QSerialPortInfo &portInfo,QSerialPortInfo::availablePorts())
    {
        if(portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier())
        {
            if(portInfo.vendorIdentifier() == vendorID)
            {
                if(portInfo.productIdentifier() == productID)
                {
                    comPort = portInfo.portName();
                    serialFlag = true;
                }
            }
        }
    }

    showFilesFromESP();
    showFilesFromESP();
    ui->txtConsole->append("files from ESP loaded");





    showScriptsOnPC();
    showModulesOnPC();



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
    int counter = 0;

    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("run");
    arguments.append("-n");
    arguments.append(script);


    QProcess* process = new QProcess(this);
    if(process->state() == QProcess::NotRunning)
    {
        ui->lblProgram->setText("Programming");
        for(int i = 0; i < 99;i++)
        {
            counter ++;
            ui->progressProgramming->setValue(counter);
        }

    }
    process->start(prog,arguments);
    qDebug() << process->state();
    if(process->state() == QProcess::Running)
    {
        counter ++;
        ui->progressProgramming->setValue(counter);
        ui->lblProgram->setText("Donne");
    }

    process->waitForFinished();

    counter = 0;
    ui->progressProgramming->setValue(counter);
    ui->lblProgram->setText("Waiting");
    ui->txtConsole->append(script + " " + "programmed");

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

        str = tmp.replace("\r"," ");
        str = tmp.replace("\n"," ");

        ui->txtbox->append(str);
    }

    ui->txtConsole->append("Got content from " + file);
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
    QDir::setCurrent(location_scripts);
    QStringList lst;
    lst = direct.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);//(QDir::Filter::Files,QDir::SortFlag::NoSort);
    ui->listScripts->addItems(lst);
    ui->txtConsole->append("Scripts loaded");
}

void MainWindow::showModulesOnPC()
{
    ui->listModule->clear();
    QDir direct;
    QDir::setCurrent(location_modules);
    QStringList lst;
    lst = direct.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);//(QDir::Filter::Files,QDir::SortFlag::NoSort);
    ui->listModule->addItems(lst);
    ui->txtConsole->append("Modules loaded");
}


void MainWindow::on_btnRemove_clicked()
{
    QString item = ui->listFilesESP->currentItem()->text();
    int x = ui->listFilesESP->currentRow();
    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("rm");
    arguments.append(item);



    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

    delete ui->listFilesESP->item(x);

    ui->txtConsole->append(item + " " + "deleted from filesystem");


}

void MainWindow::on_btnUploadESP_clicked()
{
    QDir::setCurrent(location_scripts);

    QString script = ui->listScripts->currentItem()->text();

    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("put");
    arguments.append(script);

    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

    showFilesFromESP();

    ui->txtConsole->append(script + " " + "added to filesystem");

}

void MainWindow::on_btnSetMain_clicked()
{
    QString script = ui->listScripts->currentItem()->text();
    QDir::setCurrent(location_scripts);

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

    ui->txtConsole->append(script + " " + "set as main.py");

}

void MainWindow::on_btnUploadModule_clicked()
{
    QDir::setCurrent(location_modules);

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

    showFilesFromESP();
    ui->txtConsole->append(module + " " + "added to filesystem");

}

void MainWindow::on_btnReset_clicked()
{

    QDir::setCurrent("D:/Tomek/ESP8266/Hardware configs");
    QString prog = "ampy";
    QStringList arguments;
    arguments.append("-p");
    arguments.append(comPort);
    arguments.append("run");
    arguments.append("-n");
    arguments.append("reset.py");


    QProcess* process = new QProcess(this);
    process->start(prog,arguments);
    process->waitForFinished();

    ui->txtConsole->append("ESP8266 resseted");

}

void MainWindow::on_actionScript_folder_triggered()
{
    QString dir =  QFileDialog::getExistingDirectory(this, tr("Open Directory"),location_scripts,QFileDialog::ShowDirsOnly |  QFileDialog::DontResolveSymlinks);
    location_scripts = dir;
    saveSettings();
    showScriptsOnPC();
}


void MainWindow::saveSettings()
{
    QSettings settings("JT","ESP Tool"); //No fucking idea why you need those parameters
    settings.beginGroup("settings");
    settings.setValue("Scripts",location_scripts); //set setings
    settings.setValue("Modules",location_modules); //set setings
    settings.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings settings("JT","ESP Tool");
    settings.beginGroup("settings");
    location_scripts = settings.value("Scripts").toString(); //get settings
    location_modules = settings.value("Modules").toString(); //get settings
    settings.endGroup();


}

void MainWindow::on_actionLibrary_folder_triggered()
{
    QString dir =  QFileDialog::getExistingDirectory(this, tr("Open Directory"),location_modules,QFileDialog::ShowDirsOnly |  QFileDialog::DontResolveSymlinks);
    location_modules = dir;
    saveSettings();
    showModulesOnPC();
}



