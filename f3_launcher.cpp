#include "f3_launcher.h"
#include <QMessageBox>

f3_launcher::f3_launcher()
{

}

void f3_launcher::startCheck(QMainWindow* window,QString& devPath)
{
    QString cmdline("f3write ");
    cmdline.append(devPath);
    f3_cui.start(cmdline);
    f3_cui.waitForStarted();
    f3_cui.waitForFinished();
    QString str = f3_cui.readAllStandardError();

    cmdline = "f3read ";
    cmdline.append(devPath);
    f3_cui.start(cmdline);
    f3_cui.waitForStarted();
    f3_cui.waitForFinished();
    str.append(f3_cui.readAllStandardError());

    QMessageBox::information(window,"Output of f3",str,QMessageBox::Ok);
}

void f3_launcher::stopCheck()
{

}
