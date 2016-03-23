#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checking = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatus(const QString &string)
{
    ui->labelStatus->setText(string);
}

void MainWindow::on_buttonCheck_clicked()
{
    QString inputPath = ui->textDevPath->text();
    if (inputPath.isEmpty())
    {
        QMessageBox::warning(this,"Warning","Please input the device path!");
        return;
    }
    if (checking)
    {
        showStatus("Stopped.");
        cui.stopCheck();

    }
    else
    {
        showStatus("Checking... Please wait...");
        checking = true;
        //TODO: Make checking asynchronous.
        cui.startCheck(this,inputPath);
        checking = false;
        showStatus("Finished.");
    }
}

void MainWindow::on_buttonExit_clicked()
{
    if (checking)
        cui.stopCheck();
    this->close();
}
