#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include <QtGui/QScreen>
#include <QGuiApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtCore/QDir>

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
        on_cuiError(cuiError);

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(&cui,
            SIGNAL(f3_launcher_status_changed(f3_launcher_status)),
            this,
            SLOT(on_cuiStatusChanged(f3_launcher_status)));
    connect(&cui,
            SIGNAL(f3_launcher_error(f3_launcher_error_code)),
            this,
            SLOT(on_cuiError(f3_launcher_error_code)));
    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(on_timerTimeout()));
    checking = false;
    this->userMode = 0;
    // Center window on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        move((screenGeometry.width() - width()) / 2 + screenGeometry.left(),
             (screenGeometry.height() - height()) / 2 + screenGeometry.top());
    }
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
    ui->statusBar->showMessage(tr("Ready"));
    ui->labelSpace->clear();
    ui->labelSpeed->clear();
    ui->labelProgress->setText(tr("Progress:"));
    showProgress(0);
    showProgressPage(false);
}

void MainWindow::showProgress(int progress10K)
{
    if (progress10K < 0)
    {
        ui->progressBar->setMaximum(0);
        ui->progressBar->setValue(0);
        ui->labelProgressValue->setText("");
        return;
    }

    if (ui->progressBar->maximum() <= 0)
        ui->progressBar->setMaximum(10000);
    ui->progressBar->setValue(progress10K);
    ui->labelProgressValue->setText(QString("%1%").
                                    arg(progress10K / 100.0f));
    ui->progressBar->repaint();
}

void MainWindow::showCapacity(int value)
{
    if (value >= 0)
    {
        ui->capacityBar->setFormat(tr("%p% Available"));
        timerTarget = value;
    }
    else
        ui->capacityBar->setFormat(tr("Capacity not available"));

    ui->capacityBar->setValue(0);
    if (value <= 0)
        return;
    timer.setInterval(1000 / value);
    timer.start();
}

void MainWindow::showProgressPage(bool visible)
{
    if (visible)
        ui->stackedWidget->setCurrentIndex(2);
    else if (ui->stackedWidget->currentIndex() == 2)
        ui->stackedWidget->setCurrentIndex(userMode == 1 ? 1 : 0);
}

void MainWindow::showResultPage(bool visible)
{
    if (visible)
        ui->stackedWidget->setCurrentIndex(3);
    else
        ui->stackedWidget->setCurrentIndex(userMode == 1 ? 1 : 0);
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
        if (QMessageBox::question(this, tr("Quit F3"),
                              tr("The program is still running a check.\n"
                                "Quit anyway?"),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) != QMessageBox::Yes)
            return false;
    if (userMode == 1 && ui->optionQuickTest->isChecked()
                      && ui->optionDestructive->isChecked() == false)
    {
        if (QMessageBox::warning(this, tr("Quit F3"),
                              tr("You are going to abort this test.\n"
                                "Quit now will cause PERMANENT DATA LOSS!\n"
                                "Do you really want to quit?"),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) != QMessageBox::Yes)
            return false;
    }
    return true;
}

void MainWindow::promptFix()
{
    if (QMessageBox::question(this, tr("Wrong Capacity Detected"),
                              tr("This device does not have the capacity "
                                 "it claims.\n"
                                 "Would you like to fix it?"),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes) != QMessageBox::Yes)
        return;
    if (QMessageBox::warning(this, tr("Fix Capacity"),
                             tr("You are going to fix the capacity of this "
                                "disk.\n"
                                "All data on this disk will be LOST!\n"
                                "Are you sure to continue?"),
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::No) != QMessageBox::Yes)
        return;
    cui.startFix();
}

void MainWindow::on_cuiStatusChanged(f3_launcher_status status)
{
    QString qsSpinNext;
    switch(status)
    {
        case f3_launcher_ready:
            showStatus(tr("Ready."));
            break;
        case f3_launcher_running:
            showStatus(tr("Checking... Please wait..."));
            ui->buttonMode->setEnabled(false);
            ui->textDev->setReadOnly(true);
            ui->textDevPath->setReadOnly(true);
            ui->buttonSelectDev->setEnabled(false);
            ui->buttonSelectPath->setEnabled(false);
            showProgressPage(true);
            break;
        case f3_launcher_finished:
        {
            f3_launcher_report report = cui.getReport();
            if (report.success)
                showStatus(tr("Finished (without error)."));
            else
                showStatus(tr("Finished."));
            if (report.ReportedFree == "(Fixed)")
            {
                QMessageBox::information(this, tr("Fixed successfully"),
                                         tr("The capacity of the disk has been "
                                            "adjusted to what it should be."));
                break;
            }
            f3_qt_fillReport(report);
            ui->labelSpace->setText(QString(tr("Free Space: %1\n"
                                               "Actual: %2"))
                                    .arg(report.ReportedFree)
                                    .arg(report.ActualFree));
            ui->labelSpeed->setText(QString(tr("Read speed: %1\n"
                                               "Write speed: %2"))
                                    .arg(report.ReadingSpeed)
                                    .arg(report.WritingSpeed));
            showProgress(0);
            showCapacity(report.availability * 100);
            showResultPage(true);
            break;
        }
        case f3_launcher_stopped:
            showStatus(tr("Stopped."));
            showProgress(0);
            ui->labelProgressSpin->setText("!");
            break;
        case f3_launcher_staged:
        {
            QString progressText = QString(tr("Progress: (Stage %1)")).
                                   arg(cui.getStage());
            ui->labelProgress->setText(progressText);
            showProgress(-1);
            ui->labelProgressSpin->setText("?");
            break;
        }
        case f3_launcher_progressed:
            showProgress(cui.progress10K);
            switch(ui->labelProgressSpin->text()[0].toLatin1()){
                case '|': qsSpinNext = "/"; break;
                case '/': qsSpinNext = "---"; break;
                case '-': qsSpinNext = "\\"; break;
                case '\\': qsSpinNext = "|"; break;
                default: qsSpinNext = "|"; break;
            }
            ui->labelProgressSpin->setText(qsSpinNext);
            break;
    }
    if (status == f3_launcher_running ||
        status == f3_launcher_staged ||
        status == f3_launcher_progressed)
    {
        checking = true;
        ui->buttonCheck->setText(tr("Stop"));
    }
    else
    {
        if (userMode == 1)
        {
            if (!ui->optionQuickTest->isChecked())
                unmountDisk(mountPoint);
        }
        checking = false;
        showProgressPage(false);
        ui->buttonCheck->setText(tr("Check!"));
        ui->buttonMode->setEnabled(true);
        ui->textDev->setReadOnly(false);
        ui->textDevPath->setReadOnly(false);
        ui->buttonSelectDev->setEnabled(true);
        ui->buttonSelectPath->setEnabled(true);
    }
}

void MainWindow::on_cuiError(f3_launcher_error_code errCode)
{
    switch(errCode)
    {
        case f3_launcher_no_cui:
            QMessageBox::critical(this, tr("No f3 program"),
                                  tr("Cannot find f3read/f3write.\n"
                                    "Please install f3 first."));
            exit(0);
        case f3_launcher_no_progress:
            QMessageBox::warning(this, tr("No progress showing"),
                                 tr("You are using an old version of "
                                    "f3read/f3write.\n"
                                    "The progress will not be shown during "
                                    "checking."));
            break;
        case f3_launcher_path_incorrect:
            QMessageBox::critical(this, tr("Path error"),
                                  tr("Device path not found.\n"
                                     "Please try mounting it correctly."));
            break;
        case f3_launcher_no_permission:
            QMessageBox::warning(this, tr("Permission denied"),
                                 tr("Cannot write to device.\n"
                                    "Try to re-run with sudo."));
            showStatus(tr("No enough space for test."));
            break;
        case f3_launcher_no_space:
            QMessageBox::information(this, tr("No space"),
                                     tr("No enough space for test.\n"
                                        "Please delete some files, or format "
                                        "the device and try again."));
            showStatus(tr("No enough space for test."));
            break;
        case f3_launcher_no_quick:
            QMessageBox::warning(this, tr("Legacy Mode Only"),
                                 tr("f3probe is not found.\n"
                                    "We are not able to run quick tests."));
            break;
        case f3_launcher_cache_nofound:
            showStatus(tr("No cached data found. "
                          "We will try to write data first..."));
            break;
        case f3_launcher_no_memory:
            QMessageBox::warning(this, tr("Out of memory"),
                                 tr("No enough memory for checking.\n"
                                 "You may try again with following options:\n"
                                 "  - Use less memory\n"
                                 "  - Destructive Test"));
            break;
        case f3_launcher_not_directory:
            QMessageBox::critical(this, tr("Path error"),
                                  tr("The path that you specified is not a "
                                     "directory."));
            break;
        case f3_launcher_not_disk:
            QMessageBox::critical(this, tr("Device type error"),
                                  tr("The device specified is not a disk.\n"
                                     "Please make sure what you choose is "
                                     "a disk, not a partition."));
            break;
        case f3_launcher_not_USB:
            QMessageBox::critical(this, tr("Device type error"),
                                  tr("The device specified is not a USB "
                                     "device.\n"
                                     "Currently f3 does not support quick test "
                                     "on device that is not backed by USB "
                                     "(e.g. mmc, scsi)."));
            break;
        case f3_launcher_no_fix:
            QMessageBox::warning(this, tr("Probing Only"),
                                 tr("f3fix is not found.\n"
                                    "We are not able to fix the disk if "
                                    "its reported capacity is wrong."));
            break;
        case f3_launcher_no_report:
            QMessageBox::warning(this, tr("No test result"),
                                tr("No test has been completed. "
                                    "Please run a test first."));
            break;
        case f3_launcher_oversize:
            QMessageBox::critical(this, tr("Fix failed"),
                                  tr("Cannot use the detected capacity for "
                                     "fixing.\n"
                                    "You may need to report this as a bug."));
            break;
        case f3_launcher_damaged:
            QMessageBox::critical(this, tr("Device inaccessible"),
                                  tr("Cannot access the specified device.\n"
                                     "You may not have the right permission to "
                                     "read and write to it, or it has been "
                                     "damaged."));
            break;
        default:
            QMessageBox::warning(this, tr("Unknown error"),
                                 tr("Unknown error occurred.\n"
                                    "You may need to report this as a bug."));
            showStatus( tr("An error occurred."));
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
        QMessageBox::warning(this, tr("Warning"),
                             tr("Please input the device path!"));
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
                QMessageBox::critical(this, tr("Error"),
                                      tr("Cannot mount selected device!\n"
                                         "You may need to run this program "
                                         "as root."));
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
            if (QMessageBox::question(this,  tr("Run destructive test"),
                                      tr("You choose to run destructive test.\n"
                                         "The data on the disk will be "
                                         "PERMANENTLY LOST!\n"
                                         "Continue?"),
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
    QString path = QFileDialog::getExistingDirectory(this,
                                                     tr("Choose Device Path"));
    if (!path.isEmpty())
        ui->textDevPath->setText(path);

}

void MainWindow::on_timerTimeout()
{
    int value = ui->capacityBar->value();
    if (value < timerTarget)
    {
        ui->capacityBar->setValue(++value);
    }
    else
    {
        timer.stop();
        if (timerTarget < 100 && userMode == 1 &&
            ui->optionQuickTest->isChecked())
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
        showStatus(tr("Switched to advanced mode."));
        ui->stackedWidget->setCurrentIndex(1);
        ui->buttonMode->setIcon(QIcon(":/icon/back.png"));
        ui->buttonMode->setToolTip(tr("Basic Mode"));
    }
    else
    {
        this->userMode = 0;
        showStatus(tr("Switched to basic mode."));
        ui->stackedWidget->setCurrentIndex(0);
        ui->buttonMode->setIcon(QIcon(":/icon/advanced.png"));
        ui->buttonMode->setToolTip(tr("Advanced Mode"));
    }
    showProgressPage(false);
}

void MainWindow::on_buttonSelectDev_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose Device Path"),
                                                "/dev");
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

void MainWindow::on_buttonHideResult_clicked()
{
    showResultPage(false);
}
