#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDir>

void f3_qt_fillReport(f3_launcher_report &report)
{
    const QString defaultValue = "(N/A)";
    if (report.ReportedFree.isEmpty())
        report.ReportedFree = defaultValue;
    if (report.ActualFree.isEmpty())
        report.ActualFree = defaultValue;
    if (report.availability < 0)
        report.availability = -1;
    if (report.ReadingSpeed.isEmpty())
        report.ReadingSpeed = defaultValue;
    if (report.WritingSpeed.isEmpty())
        report.WritingSpeed = defaultValue;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    f3_launcher_error_code cuiError = cui.getErrCode();
    if (cuiError != 0)
        on_cui_error(cuiError);

    ui->setupUi(this);
    connect(&cui,
            SIGNAL(f3_launcher_status_changed(f3_launcher_status)),
            this,
            SLOT(on_cui_status_changed(f3_launcher_status)));
    connect(&cui,
            SIGNAL(f3_launcher_error(f3_launcher_error_code)),
            this,
            SLOT(on_cui_error(f3_launcher_error_code)));
    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(on_timer_timeout()));
    checking = false;
    this->userMode = 0;
    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->width() - width()) / 2);
    setFixedSize(width(), height());
    clearStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatus(const QString &string)
{
    ui->statusBar->showMessage(string);
}

void MainWindow::clearStatus()
{
    ui->statusBar->showMessage("Ready");
    ui->groupResult->hide();
    ui->labelSpace->clear();
    ui->labelSpeed->clear();
    ui->progressBar->setValue(0);
    ui->labelProgress->setText("Progress:");
}

void MainWindow::showCapacity(int value)
{
    if (value >= 0)
    {
        ui->capacityBar->setFormat("%p% Available");
        timerTarget = value;
    }
    else
        ui->capacityBar->setFormat("Capacity not available");

    ui->capacityBar->setValue(0);
    if (value <= 0)
        return;
    timer.setInterval(1000 / value);
    timer.start();
}

QString MainWindow::mountDisk(const QString& device)
{
    QDir dir;
    QString mountDir = device;
    mountDir.replace('/','_').replace('\\','_').append("_f3-qt");
    dir.mkdir(mountDir);
    QProcess cui;
    QStringList args(device);
    args << mountDir;
    cui.start("mount", args);
    cui.waitForStarted();
    cui.waitForFinished();
    if (cui.exitCode() == 0)
        return mountDir;
    else
    {
        dir.rmdir(mountDir);
        return "";
    }
}

bool MainWindow::unmountDisk(const QString& mountPoint)
{
    QProcess cui;
    QStringList args(mountPoint);
    cui.start("umount", args);
    cui.waitForStarted();
    cui.waitForFinished();
    QDir dir;
    dir.rmdir(mountPoint);
    if (cui.exitCode() == 0)
        return true;
    else
        return false;
}

bool MainWindow::sureToExit(bool manualClose)
{
    if (!manualClose)
        if (QMessageBox::question(this,"Quit F3",
                              "The program is still running a check.\n"
                              "Quit anyway?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) != QMessageBox::Yes)
            return false;
    if (userMode == 1 && ui->optionQuickTest->isChecked()
                      && ui->optionDestructive->isChecked() == false)
    {
        if (QMessageBox::warning(this,"Quit F3",
                              "You are going to abort this test.\n"
                              "Quit now will cause PERMANENT DATA LOSS!\n"
                              "Do you really want to quit?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) != QMessageBox::Yes)
            return false;
    }
    return true;
}

void MainWindow::promptFix()
{
    if (QMessageBox::question(this, "Wrong Capacity Detected",
                              "This device does not have the capacity it claims.\n"
                              "Would you like to fix it?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes) != QMessageBox::Yes)
        return;
    if (QMessageBox::warning(this,"Fix Capacity",
                          "You are going to fix the capacity of this disk.\n"
                          "All data on this disk will be LOST!\n"
                          "Are you sure to continue?",
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::No) != QMessageBox::Yes)
        return;
    cui.startFix();
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
            ui->buttonMode->setEnabled(false);
            ui->textDev->setReadOnly(true);
            ui->textDevPath->setReadOnly(true);
            ui->buttonSelectDev->setEnabled(false);
            ui->buttonSelectPath->setEnabled(false);
            ui->groupProgress->show();
            break;
        case f3_launcher_finished:
        {
            f3_launcher_report report = cui.getReport();
            if (report.success)
                showStatus("Finished (without error).");
            else
                showStatus("Finished.");
            if (report.ReportedFree == "(Fixed)")
            {
                QMessageBox::information(this,"Fixed successfully",
                                         "The capacity of the partition of the disk\n"
                                         "has been adjusted to what it should be.");
                break;
            }
            f3_qt_fillReport(report);
            ui->labelSpace->setText(QString("Free Space: ")
                                    .append(report.ReportedFree)
                                    .append("\nActual: ")
                                    .append(report.ActualFree)
                                    );
            ui->labelSpeed->setText(QString("Read speed: ")
                                    .append(report.ReadingSpeed)
                                    .append("\nWrite speed: ")
                                    .append(report.WritingSpeed)
                                    );
            ui->groupResult->show();
            ui->progressBar->setMaximum(100);
            showCapacity(report.availability * 100);
            break;
        }
        case f3_launcher_stopped:
            showStatus("Stopped.");
            ui->progressBar->setMaximum(100);
            ui->progressBar->setValue(0);
            break;
        case f3_launcher_staged:
        {
            QString progressText;
            progressText.sprintf("Progress:(Stage %d)",cui.getStage());
            ui->labelProgress->setText(progressText);
            ui->progressBar->setMaximum(0);
            ui->progressBar->setValue(0);
            break;
        }
        case f3_launcher_progressed:
            ui->progressBar->setMaximum(100);
            ui->progressBar->setValue(cui.progress);
            break;
    }
    if (status == f3_launcher_running ||
        status == f3_launcher_staged ||
        status == f3_launcher_progressed)
    {
        checking = true;
        ui->buttonCheck->setText("Stop");
    }
    else
    {
        if (userMode == 1)
        {
            if (!ui->optionQuickTest->isChecked())
                unmountDisk(mountPoint);
            ui->groupProgress->hide();
        }
        checking = false;
        ui->buttonCheck->setText("Check!");
        ui->buttonMode->setEnabled(true);
        ui->textDev->setReadOnly(false);
        ui->textDevPath->setReadOnly(false);
        ui->buttonSelectDev->setEnabled(true);
        ui->buttonSelectPath->setEnabled(true);
    }
}

void MainWindow::on_cui_error(f3_launcher_error_code errCode)
{
    switch(errCode)
    {
        case f3_launcher_no_cui:
            QMessageBox::critical(this,"No f3 program",
                                  "Cannot find f3read/f3write.\n"
                                  "Please install f3 first.");
            exit(0);
        case f3_launcher_no_progress:
            QMessageBox::warning(this,"No progress showing",
                                 "You are using an old version of f3read/f3write.\n"
                                 "The progress will not be shown during checking.");
            break;
        case f3_launcher_path_incorrect:
            QMessageBox::critical(this,"Path error",
                              "Device path not found.\n"
                              "Please try mounting it correctly.");
            break;
        case f3_launcher_no_permission:
            QMessageBox::warning(this,"Permission denied",
                          "Cannot write to device.\n"
                          "Try to re-run with sudo.");
            showStatus("No enough space for test.");
            break;
        case f3_launcher_no_space:
            QMessageBox::information(this,"No space",
                          "No enough space for checking.\n"
                          "Please delete some file, or format the device and try again.");
            showStatus("No enough space for test.");
            break;
        case f3_launcher_no_quick:
            QMessageBox::warning(this,"Legacy Mode Only",
                                 "f3probe was not found.\n"
                                 "We are not able to test under quick mode.");
            break;
        case f3_launcher_cache_nofound:
            showStatus("No cached data found. Test from writing...");
            break;
        case f3_launcher_no_memory:
            QMessageBox::warning(this,"Out of memory",
                                 "No enough memory for checking.\n"
                                 "You may try again with following options:\n"
                                 "  -Use less memory\n"
                                 "  -Destructive Test");
            break;
        case f3_launcher_not_directory:
            QMessageBox::critical(this,"Path error",
                              "The path specified is not a directory.\n");
            break;
        case f3_launcher_not_disk:
            QMessageBox::critical(this,"Device type error",
                                  "The device specified is not a disk.\n"
                                  "Please make sure what you choose is a disk,"
                                  " not a partition.");
            break;
        case f3_launcher_not_USB:
            QMessageBox::critical(this,"Device type error",
                                  "The device specified is not a USB device.\n"
                                  "Currently f3 does not support quick test on "
                                  "device that is not backed by USB (e.g. mmc, scsi).");
            break;
        case f3_launcher_no_fix:
            QMessageBox::warning(this,"Probing Only",
                             "f3fix was not found.\n"
                             "We are not able to fix the disk if its capacity is wrong.");
            break;
        case f3_launcher_no_report:
            QMessageBox::warning(this,"No test result",
                                 "No test has been completed. Please run a test first.");
            break;
        case f3_launcher_oversize:
            QMessageBox::critical(this,"Fix failed",
                                  "Cannot use detected capacity for fixing.\n"
                                  "You may need to report this as a bug.");
            break;
        case f3_launcher_damaged:
            QMessageBox::critical(this,"Device inaccessible",
                                  "Cannot access the specified device.\n"
                                  "You may not have the right permission to"
                                  "read and write to it, or it has been damaged.");
            break;
        default:
            QMessageBox::warning(this,"Unknown error",
                                 "Unknown error occurred.\n"
                                 "You may need to report this as a bug.");
            showStatus("An error occurred.");
    }
}

void MainWindow::on_buttonCheck_clicked()
{
    if (checking)
    {
        cui.stopCheck();
        return;
    }

    QString inputPath;
    if (userMode == 0)
        inputPath  = ui->textDevPath->text().trimmed();
    else
        inputPath = ui->textDev->text().trimmed();
    if (inputPath.isEmpty())
    {
        QMessageBox::warning(this,"Warning","Please input the device path!");
        return;
    }

    if (userMode == 0)
    {
        cui.setOption("mode", "legacy");
        cui.setOption("cache", "none");
    }
    else
    {
        if (ui->optionQuickTest->isChecked())
            cui.setOption("mode", "quick");
        else
        {
            mountPoint = mountDisk(inputPath);
            if (mountPoint.isEmpty())
            {
                QMessageBox::critical(this,"Error","Cannot mount selected device!\n"
                                      "You may need to run this program as root.");
                return;
            }
            inputPath = mountPoint;
            cui.setOption("mode", "legacy");
        }

        if (ui->optionUseCache->isChecked())
            cui.setOption("cache", "write");
        else
            cui.setOption("cache", "none");

        if (ui->optionLessMem->isChecked())
            cui.setOption("memory", "minimum");
        else
            cui.setOption("memory", "full");

        if (ui->optionDestructive->isChecked())
        {
            if (QMessageBox::question(this, "Run destructive test",
                                      "You choose to run destructive test.\n"
                                      "The data on the disk will be destroyed!\n"
                                      "Continue?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No) != QMessageBox::Yes)
                return;
            cui.setOption("destructive", "yes");
        }
        else
            cui.setOption("destructive", "no");
    }

    clearStatus();
    cui.startCheck(inputPath);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!checking)
        return;
    if (sureToExit(false))
    {
        cui.stopCheck();
        help.close();
    }
    else
        event->ignore();
}

void MainWindow::on_buttonExit_clicked()
{
    if (checking)
    {
        if (!sureToExit(true))
            return;
        cui.stopCheck();
        checking = false;
    }
    this->close();
}

void MainWindow::on_buttonSelectPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Choose Device Path");
    if (!path.isEmpty())
        ui->textDevPath->setText(path);

}

void MainWindow::on_timer_timeout()
{
    int value = ui->capacityBar->value();
    if (value < timerTarget)
    {
        ui->capacityBar->setValue(++value);
    }
    else
    {
        timer.stop();
        if (timerTarget < 100 && userMode == 1 && ui->optionQuickTest->isChecked())
            promptFix();
    }
}

void MainWindow::on_buttonHelp_clicked()
{
    help.show();
}

void MainWindow::on_buttonMode_clicked()
{
    if (this->userMode == 0)
    {
        this->userMode = 1;
        showStatus("Switched to advacned mode.");
        ui->stackedWidget->setCurrentIndex(1);
        ui->buttonMode->setIcon(QIcon(":/icon/back.png"));
        ui->buttonMode->setToolTip("Basic Mode");
        ui->groupProgress->hide();
    }
    else
    {
        this->userMode = 0;
        showStatus("Switched to basic mode.");
        ui->stackedWidget->setCurrentIndex(0);
        ui->buttonMode->setIcon(QIcon(":/icon/advanced.png"));
        ui->buttonMode->setToolTip("Advanced Mode");
        ui->groupProgress->show();
    }
    if (cui.getStatus() != f3_launcher_finished)
        ui->groupResult->hide();
}

void MainWindow::on_buttonSelectDev_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Choose Device Path","/dev");
    if (!path.isEmpty())
        ui->textDev->setText(path);
}

void MainWindow::on_optionQuickTest_clicked()
{
    if (ui->optionQuickTest->isChecked())
    {
        ui->optionDestructive->setEnabled(true);
        ui->optionLessMem->setEnabled(true);
        ui->optionUseCache->setEnabled(false);
    }
    else
    {
        ui->optionDestructive->setChecked(false);
        ui->optionDestructive->setEnabled(false);
        ui->optionLessMem->setChecked(false);
        ui->optionLessMem->setEnabled(false);
        ui->optionUseCache->setEnabled(true);
    }
}

void MainWindow::on_optionLessMem_clicked()
{
    ui->optionDestructive->setChecked(false);
}

void MainWindow::on_optionDestructive_clicked()
{
    ui->optionLessMem->setChecked(false);
}

void MainWindow::on_buttonMode_2_clicked()
{
    ui->groupResult->hide();
}
