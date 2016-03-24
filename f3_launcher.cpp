#include "f3_launcher.h"
#include <QMessageBox>

f3_launcher::f3_launcher()
{
    stage = 0;
    connect(&f3_cui,
            SIGNAL(finished(int,QProcess::ExitStatus)),
            this,
            SLOT(on_f3_cui_finished()));
}

f3_launcher::~f3_launcher()
{
    f3_cui.terminate();
}

void f3_launcher::startCheck(QString& devPath)
{
    if (stage != 0)
        stopCheck();

    f3_cui_output.clear();
    stage = 1;
    emit f3_launcher_status_changed(f3_launcher_running);

    this->devPath = devPath;
    f3_cui.start(QString("f3write ").append(devPath));
}

void f3_launcher::stopCheck()
{
    f3_cui.terminate();
    f3_cui.waitForFinished();
}

int f3_launcher::parseOutput()
{
    int exitCode = f3_cui.exitCode();
    switch(exitCode)
    {
        case 0:     //Exit normally
            f3_cui_output.append("\n").append(f3_cui.readAllStandardOutput());
            break;
        case 2:     //Device not exists
            f3_cui_output = "Device path not exists.";
            emit f3_launcher_status_changed(f3_launcher_path_incorrect);
            break;
        case 15:    //Terminated manually
            emit f3_launcher_status_changed(f3_launcher_stopped);
            break;
        default:
            f3_cui_output = QString("Error:\n").append(f3_cui.readAllStandardError());
            emit f3_launcher_status_changed(f3_launcher_unknownError);
    }
    return exitCode;
}

void f3_launcher::on_f3_cui_finished()
{
    if (stage == 0)
        return;
    else if (stage == 1)
    {
        if (parseOutput() != 0)
        {
            stage = 0;
            return;
        }
        stage = 2;
        f3_cui.start(QString("f3read ").append(devPath));
    }
    else
    {
        stage = 0;
        if (parseOutput() == 0)
            emit f3_launcher_status_changed(f3_launcher_finished);
    }
}
