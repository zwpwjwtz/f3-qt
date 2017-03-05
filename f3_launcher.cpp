#include "f3_launcher.h"
#include <QtMath>
#include <QDir>
#include <QFile>
#include <QTime>

#define F3_READ_COMMAND "f3read"
#define F3_WRITE_COMMAND "f3write"
#define F3_PROBE_COMMAND "f3probe"
#define F3_FIX_COMMAND "f3fix"
#define F3_OPTION_SHOW_PROGRESS "--show-progress=1"
#define F3_OPTION_MIN_MEM "--min-memory"
#define F3_OPTION_DESTRUCTIVE "--destructive"
#define F3_OPTION_TIME "--time-ops"

#define F3_VERSION_TAG1 "Copyright (C)"
#define F3_VERSION_TAG2 "F3 read"

#define F3_RESULT_TAG_READ_SPEED "Average reading speed:"
#define F3_RESULT_TAG_WRITE_SPEED "Average writing speed:"
#define F3_RESULT_TAG_SPACE_FREE "Free space:"
#define F3_RESULT_TAG_SPACE_OK "Data OK:"
#define F3_RESULT_TAG_SPACE_LOST "Data LOST:"
#define F3_RESULT_TAG_SIZE_ANNOUNCE "Announced size:"
#define F3_RESULT_TAG_SIZE_USABLE "*Usable* size:"
#define F3_RESULT_TAG_SIZE_BLOCK "Physical block size:"
#define F3_RESULT_TAG_SIZE_MODULE "Module:"
#define F3_RESULT_TAG_READ_SPEED2 "Read:"
#define F3_RESULT_TAG_WRITE_SPEED2 "Write:"
#define F3_RESULT_TAG_FIX_SUCCEED "was successfully fixed"
#define F3_RESULT_FORMAT_TIME "s.zzz's'"
#define F3_RESULT_FORMAT_TIME2 "m:ss\""

#define F3_ERROR_TAG_INACCESSIBLE "is damaged\n"
#define F3_ERROR_TAG_NO_SPACE "No space!"
#define F3_ERROR_TAG_NO_MEM "Out of memory"
#define F3_ERROR_TAG_NOT_DISK "is a partition of disk device"
#define F3_ERROR_TAG_NOT_ROOT "Your user doesn't have access to"
#define F3_ERROR_TAG_NOT_USB "is not backed by a USB device"
#define F3_ERROR_TAG_OVERSIZE "Can't have a partition outside the disk"

#define F3_DISK_PROBE_FILE "f3_qt_probe"
#define F3_FILE_FILTER "*.h2w"


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
        return -1;
    else
        return number1 / number2 / qPow(1000,grade2 - grade1);
}

QString f3_capacity_unit(const int grade)
{
    QString unit;
    switch(grade)
    {
        case 1:
            unit = "KB"; break;
        case 2:
            unit = "MB"; break;
        case 3:
            unit = "TB"; break;
        case 4:
            unit = "PB"; break;
        default:
            unit = "Byte";
    }
    return unit;
}

QTime f3_operation_time(QString time)
{
    time = time.trimmed();
    QTime temp;
    if (time.indexOf('s') >=0)
    {
        time.replace('s',"0s");
        temp = QTime::fromString(time, F3_RESULT_FORMAT_TIME);
    }
    else
    {
        time.replace('\'', ':');
        temp = QTime::fromString(time, F3_RESULT_FORMAT_TIME2);
    }
    return temp;
}

QString f3_operation_speed(const QString& operation,qint64 blockSize)
{
    int p = operation.indexOf('/');
    int operationSec = - f3_operation_time(operation.left(p)).secsTo(QTime(0,0,0,0));
    if (operationSec == 0)
        return "";
    qint64 blockCount = operation.mid(p + 1, operation.indexOf('=') - p - 1).toInt();
    float speed = blockCount * blockSize / operationSec;
    int grade = qLn(speed) / qLn(1000);
    return QString::number((speed / qPow(1000, grade)))
            .append(' ')
            .append(f3_capacity_unit(grade))
            .append("/s");
}

f3_launcher::f3_launcher()
{
    errCode = f3_launcher_ok;
    float version = probeVersion();
    if (version == 0)
    {
        emitError(f3_launcher_no_cui);
        return;
    }
    if (version < 4.0 || !probeCommand(F3_PROBE_COMMAND))
    {
        emitError(f3_launcher_no_quick);
    }
    if (version < 5.0 || !probeCommand(F3_FIX_COMMAND))
    {
        emitError(f3_launcher_no_fix);
    }
    if (version <= 6.0)
    {
        emitError(f3_launcher_no_progress);
        showProgress = false;
    }
    else
    {
        emit f3_launcher_status_changed(f3_launcher_ready);
        showProgress = true;
    }

    options["mode"] = "legacy";
    options["cache"] = "none";
    options["memory"] = "full";
    options["destructive"] = "no";
    options["autofix"] = "no";

    stage = 0;
    connect(&f3_cui,
            SIGNAL(finished(int,QProcess::ExitStatus)),
            this,
            SLOT(on_f3_cui_finished()));
    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(on_timer_timeout()));
    timer.setInterval(1500);

}

f3_launcher::~f3_launcher()
{
    f3_cui.terminate();
}

void f3_launcher::emitError(f3_launcher_error_code errorCode)
{
    errCode = errorCode;
    emit f3_launcher_error(errorCode);
}

f3_launcher_status f3_launcher::getStatus()
{
    return status;
}

f3_launcher_error_code f3_launcher::getErrCode()
{
    return errCode;
}

bool f3_launcher::setOption(QString key, QString value)
{
    if (key.isEmpty())
        return false;
    options[key] = value;
    return true;
}

QString f3_launcher::getOption(QString key)
{
    return options[key];
}

void f3_launcher::startCheck(QString devPath)
{
    if (stage != 0)
        stopCheck();

    f3_cui_output.clear();
    progress = 0;
    status = f3_launcher_running;
    emit f3_launcher_status_changed(f3_launcher_running);

    this->devPath = devPath;
    QString command;
    QStringList args;
    if (getOption("mode") == "quick")
    {
        command = QString(F3_PROBE_COMMAND);
        if (!probeCommand(command))
        {
            emitError(f3_launcher_no_quick);
            status = f3_launcher_stopped;
            emit f3_launcher_status_changed(f3_launcher_stopped);
            return;
        }
        if (getOption("memory") == "minimum")
            args << QString(F3_OPTION_MIN_MEM);
        if (getOption("destructive") == "true")
            args << QString(F3_OPTION_DESTRUCTIVE);
        args << QString(F3_OPTION_TIME);
        stage = 11;
        emit f3_launcher_status_changed(f3_launcher_staged);
    }
    else
    {
        command = QString(F3_WRITE_COMMAND);
        stage = 1;
        if (getOption("cache") == "write")
        {
            if (probeDiskFull(devPath) && probeCacheFile(devPath))
            {
                command = QString(F3_READ_COMMAND);
                stage = 2;
            }
            else
                emitError(f3_launcher_cache_nofound);
        }
        if (showProgress)
            args << QString(F3_OPTION_SHOW_PROGRESS);
        emit f3_launcher_status_changed(f3_launcher_staged);
    }
    args << devPath;
    f3_cui.start(command, args);

    if (showProgress)
    {
        timer.start();
    }
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

    bool legacyMode = getOption("mode") == "legacy";

    if ((legacyMode && f3_cui_output.indexOf(F3_RESULT_TAG_READ_SPEED)) ||
        f3_cui_output.indexOf(F3_RESULT_TAG_READ_SPEED2))
        report.success = true;
    else if (f3_cui_output.indexOf(F3_RESULT_TAG_FIX_SUCCEED))
    {
        report.success = true;
        report.ReportedFree = "(Fixed)";
        return report;
    }

    if (legacyMode)
    {
        report.ReportedFree = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SPACE_FREE);
        report.ActualFree = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SPACE_OK);
        report.LostSpace = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SPACE_LOST);
    }
    else
    {
        report.ReportedFree = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SIZE_ANNOUNCE);
        report.ReportedFree.truncate(report.ReportedFree.indexOf(" ("));
        report.ActualFree = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SIZE_USABLE);
    }
    report.ActualFree.truncate(report.ActualFree.indexOf(" ("));
    report.LostSpace.truncate(report.LostSpace.indexOf(" ("));
    report.availability = f3_capacity_ratio(report.ActualFree, report.ReportedFree);

    if (!legacyMode)
    {
        report.ModuleSize = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SIZE_MODULE);
        report.ModuleSize.truncate(report.ModuleSize.indexOf(" ("));
        report.BlockSize = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_SIZE_BLOCK);
        report.BlockSize.truncate(report.BlockSize.indexOf(" ("));        
    }


    if (legacyMode)
    {
        report.ReadingSpeed = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_READ_SPEED);
        report.WritingSpeed = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_WRITE_SPEED);
    }
    else
    {
        qint64 blockSize = report.BlockSize.left(report.BlockSize.indexOf(' ')).toFloat();
        blockSize *= qPow(1000, f3_capacity_grade(report.BlockSize));
        report.ReadingSpeed = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_READ_SPEED2);
        report.ReadingSpeed = f3_operation_speed(report.ReadingSpeed, blockSize);
        report.WritingSpeed = f3_get_line_result(f3_cui_output,F3_RESULT_TAG_WRITE_SPEED2);
        report.WritingSpeed = f3_operation_speed(report.WritingSpeed, blockSize);
    }

    return report;
}

int f3_launcher::getStage()
{
    return stage % 10;
}

void f3_launcher::startFix()
{
    if (devPath.isEmpty())
        return;

    QString size, blockSize;
    f3_launcher_report report = getReport();
    if (report.success ==false || report.BlockSize.isEmpty() || report.ActualFree.isEmpty())
    {
        emitError(f3_launcher_no_report);
        return;
    }
    else
    {
        size = report.ActualFree.size();
        blockSize = report.BlockSize;
    }

    qint64 sizeInByte = size.left(size.indexOf(' ')).toInt()
                            * qPow(1000, f3_capacity_grade(size));
    qint64 blockSizeInByte = blockSize.left(blockSize.indexOf(' ')).toInt()
                                * qPow(1000, f3_capacity_grade(blockSize));
    qint64 blockCount = sizeInByte / blockSizeInByte;

    f3_cui_output.clear();
    status = f3_launcher_running;
    emit f3_launcher_status_changed(f3_launcher_running);
    stage = 21;
    emit f3_launcher_status_changed(f3_launcher_staged);
    QStringList args;
    args << "-l" << QString::number(blockCount);
    args << devPath;
    f3_cui.start(F3_FIX_COMMAND, args);
}

bool f3_launcher::probeCommand(QString command)
{
    f3_cui.start(command);
    f3_cui.waitForStarted();
    f3_cui.waitForFinished();
    if (f3_cui.exitCode() == 255)
        return false;
    else
        return true;
}

float f3_launcher::probeVersion()
{
    if (!probeCommand(F3_WRITE_COMMAND) || !probeCommand(F3_READ_COMMAND))
        return 0;

    QString output = f3_cui.readAllStandardError();
    int p = output.indexOf(F3_VERSION_TAG1);
    if (p > 0)
    {
        return f3_get_line_result(output,F3_VERSION_TAG2).toFloat();
    }
    else
    {
        return 6.1;
    }

}

bool f3_launcher::probeDiskFull(QString& devPath)
{
    bool diskFull = false;
    QFile tempFile(QString(devPath).
                   append("/").
                   append(F3_DISK_PROBE_FILE));
    if (!tempFile.open(QFile::ReadWrite))
        diskFull = true;
    else
    {
        QByteArray data ("TestData");
        if (tempFile.write(data) < data.length())
            diskFull = true;
        else if (tempFile.read(data.length()) != data)
            diskFull = true;
    }
    tempFile.close();
    tempFile.remove();
    return diskFull;
}

bool f3_launcher::probeCacheFile(QString& devPath)
{
    QDir dir(devPath);
    QStringList fileList = dir.entryList(QStringList(F3_FILE_FILTER));
    if (!fileList.isEmpty())
        return true;
    else
        return false;
}

int f3_launcher::parseOutput()
{
    int exitCode = f3_cui.exitCode();
    switch(exitCode)
    {
        case 0:
            //Exit normally || Inaccessible
            f3_cui_output.append(f3_cui.readAllStandardOutput());
            if (f3_cui_output.indexOf(F3_ERROR_TAG_INACCESSIBLE) >= 0)
                emitError(f3_launcher_damaged);
            break;
        case 1:
            //No space || No memory || Not root || Not disk ||
            //Not USB || Oversize
            f3_cui_output = f3_cui.readAllStandardOutput();
            f3_cui_output.append(f3_cui.readAllStandardError());
            if (f3_cui_output.indexOf(F3_ERROR_TAG_NO_SPACE) >= 0)
                emitError(f3_launcher_no_space);
            else if (f3_cui_output.indexOf(F3_ERROR_TAG_NO_MEM) >=0 )
                emitError(f3_launcher_no_memory);
            else if (f3_cui_output.indexOf(F3_ERROR_TAG_NOT_DISK) >= 0)
                emitError(f3_launcher_not_disk);
            else if (f3_cui_output.indexOf(F3_ERROR_TAG_NOT_ROOT) >= 0)
                emitError(f3_launcher_no_permission);
            else if (f3_cui_output.indexOf(F3_ERROR_TAG_NOT_USB) >= 0)
                emitError(f3_launcher_not_USB);
            else if (f3_cui_output.indexOf(F3_ERROR_TAG_OVERSIZE) >= 0)
                emitError(f3_launcher_oversize);
            else
                f3_cui_output.clear();
            break;
        case 2:     //Path not exists
            emitError(f3_launcher_path_incorrect);
            break;
        case 13:   //Permission denied
            emitError(f3_launcher_no_permission);
        case 15:    //Terminated manually
            break;
        case 20:    //Not directory
            emitError(f3_launcher_not_directory);
        case 64:    //No argument
            break;
        case 143:   //Terminated by other process
            break;
        default:
            f3_cui_output = QString("Error:\n").append(f3_cui.readAllStandardError());
            emitError(f3_launcher_unknownError);
    }
    return exitCode;
}

void f3_launcher::on_f3_cui_finished()
{
    timer.stop();
    if (stage == 0)
        return;
    else if (stage == 1)
    {
        if (parseOutput() != 0)
        {
            stage = 0;
            status = f3_launcher_stopped;
            emit f3_launcher_status_changed(f3_launcher_stopped);
            return;
        }

        stage = 2;
        progress = 0;
        QStringList args;
        if (showProgress)
            args << QString(F3_OPTION_SHOW_PROGRESS);
        args << devPath;
        f3_cui.start(QString(F3_READ_COMMAND),args);
        emit f3_launcher_status_changed(f3_launcher_staged);

        if (showProgress)
        {
            timer.start();
        }
    }
    else if (stage == 11 && options["autofix"] == "true")
    {
        startFix();
    }
    else
    {
        stage = 0;

        if (parseOutput() == 0)
        {
            status = f3_launcher_finished;
            emit f3_launcher_status_changed(f3_launcher_finished);            
        }
        else
        {
            status = f3_launcher_stopped;
            emit f3_launcher_status_changed(f3_launcher_stopped);
        }
    }
}

void f3_launcher::on_timer_timeout()
{
    QString temp = f3_cui.readAllStandardOutput();
    if (temp.isEmpty()) return;
    temp.remove(QChar('\b'));
    int p = temp.indexOf("% --");
    if (p >= 0)
    {
        int p2 = temp.indexOf("... ", p - 7);
        int percentage = temp.mid(p2 + 4, p - p2 - 4).trimmed().toFloat();
        if (percentage > progress)
            progress = percentage;
        emit f3_launcher_status_changed(f3_launcher_progressed);
    }
    f3_cui_output.append(temp);
}
