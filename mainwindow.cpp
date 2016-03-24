#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&cui,
            SIGNAL(f3_launcher_status_changed(f3_launcher_status)),
            this,
            SLOT(on_cui_status_changed(f3_launcher_status)));
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

void MainWindow::on_cui_status_changed(f3_launcher_status status)
{
    switch(status)
    {
        case f3_launcher_ready:
            showStatus("Ready.");
            break;
        case f3_launcher_running:
            showStatus("Checking... Please wait...");
            break;
        case f3_launcher_finished:
            showStatus("Finished.");
            QMessageBox::information(this,"Output of f3",cui.f3_cui_output);
            break;
        case f3_launcher_stopped:
            showStatus("Stopped.");
            break;
        case f3_launcher_path_incorrect:
            QMessageBox::critical(this,"Path error",cui.f3_cui_output);
        default:
            showStatus("An error occurred.");
    }
    if (status == f3_launcher_running)
    {
        checking = true;
        ui->buttonCheck->setText("Stop");
    }
    else
    {
        checking = false;
        ui->buttonCheck->setText("Check!");
    }
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
        cui.startCheck(inputPath);
    }
}

void MainWindow::on_buttonExit_clicked()
{
    if (checking)
        cui.stopCheck();
    this->close();
}
