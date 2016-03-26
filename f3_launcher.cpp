#include "f3_launcher.h"
#include <QtMath>

QString f3_get_line_result(const QString& str,const QString& testString)
{
    int p1 = str.indexOf(testString);
    if (p1 >= 0)
    {
        int p2 = str.indexOf('\n',p1 + 1);
        return str.mid(p1 + testString.length(), p2 - p1 - testString.length()).trimmed();
    }
    else
        return "";
}

int f3_capacity_grade(const QString& capacity)
{
    int grade = 0;
    QString unit = capacity.trimmed().toUpper();
    if (unit.endsWith("KB") || unit.endsWith("KIB"))
        grade = 1;
    else if (unit.endsWith("MB") || unit.endsWith("MIB"))
        grade = 2;
    else if (unit.endsWith("GB") || unit.endsWith("GIB"))
        grade = 3;
    else if (unit.endsWith("TB") || unit.endsWith("TIB"))
        grade = 4;
    else if (unit.endsWith("PB") || unit.endsWith("PIB"))
        grade = 5;
    return grade;
}

float f3_capacity_ratio(const QString& numerator, const QString& denominator)
{
    float number1 = numerator.mid(0,numerator.indexOf(' ')).toFloat();
    float number2 = denominator.mid(0,denominator.indexOf(' ')).toFloat();
    int grade1 = f3_capacity_grade(numerator);
    int grade2 = f3_capacity_grade(denominator);
    if (number2 == 0)
        return 0;
    else
        return number1 / number2 / qPow(1024,grade2 - grade1);
}


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

f3_launcher_report f3_launcher::getReport()
{
    f3_launcher_report report;
    report.success = false;

    if (f3_cui_output.trimmed().isEmpty())
        return report;

    if (f3_cui_output.indexOf("Average reading speed:"))
        report.success = true;

    report.ReportedFree = f3_get_line_result(f3_cui_output,"Free space:");

    report.ActualFree = f3_get_line_result(f3_cui_output,"Data OK:");
    report.ActualFree.truncate(report.ActualFree.indexOf(" ("));

    report.LostSpace = f3_get_line_result(f3_cui_output,"Data LOST:");
    report.LostSpace.truncate(report.LostSpace.indexOf(" ("));

    report.availability = f3_capacity_ratio(report.ActualFree, report.ReportedFree);

    report.ReadingSpeed = f3_get_line_result(f3_cui_output,"Average reading speed:");
    if (report.ReadingSpeed.isEmpty())
        report.ReadingSpeed = "(N/A)";

    report.WritingSpeed = f3_get_line_result(f3_cui_output,"Average writing speed:");
    if (report.WritingSpeed.isEmpty())
        report.WritingSpeed = "(N/A)";
    return report;
}

int f3_launcher::parseOutput()
{
    int exitCode = f3_cui.exitCode();
    switch(exitCode)
    {
        case 0:     //Exit normally
            f3_cui_output.append("\n").append(f3_cui.readAllStandardOutput());
            break;
        case 1:     //No argument || No space
            f3_cui_output = f3_cui.readAllStandardOutput();
            if (f3_cui_output.indexOf("No space!") >= 0)
                emit f3_launcher_status_changed(f3_launcher_no_space);
            else
            f3_cui_output.clear();
            break;
        case 2:     //Device not exists
            emit f3_launcher_status_changed(f3_launcher_path_incorrect);
            break;
        case 13:   //Permission denied
            emit f3_launcher_status_changed(f3_launcher_no_permission);
        case 15:    //Terminated manually
        case 143:   //Terminated by other process
            emit f3_launcher_status_changed(f3_launcher_stopped);
            break;
        case 127:   //Command not found
            emit f3_launcher_status_changed(f3_launcher_no_cui);
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
