#ifndef F3_LAUNCHER_H
#define F3_LAUNCHER_H
#include <QProcess>
#include <QTimer>
#include <QMap>


enum f3_launcher_status
{
    f3_launcher_ready = 0,
    f3_launcher_running = 1,
    f3_launcher_finished = 2,
    f3_launcher_stopped = 3,
    f3_launcher_staged = 4,
    f3_launcher_progressed = 5,
};
enum f3_launcher_error_code
{
    f3_launcher_ok = 0,
    f3_launcher_path_incorrect = 128,
    f3_launcher_no_cui = 129,
    f3_launcher_no_permission = 130,
    f3_launcher_no_space = 131,
    f3_launcher_no_progress = 132,
    f3_launcher_no_quick = 133,
    f3_launcher_cache_nofound = 134,
    f3_launcher_no_memory = 135,
    f3_launcher_not_directory = 136,
    f3_launcher_not_disk = 137,
    f3_launcher_not_USB = 138,
    f3_launcher_no_fix = 139,
    f3_launcher_no_report = 140,
    f3_launcher_oversize = 141,
    f3_launcher_damaged = 142,
    f3_launcher_unknownError = 255,
};

struct f3_launcher_report
{
    bool success;
    QString ReadingSpeed;
    QString WritingSpeed;
    QString ReportedFree;
    QString ActualFree;
    QString LostSpace;
    float availability;
    QString ModuleSize;
    QString BlockSize;
};


class f3_launcher : public QObject
{
    Q_OBJECT

public:
    f3_launcher();
    ~f3_launcher();
    f3_launcher_status getStatus();
    f3_launcher_error_code getErrCode();
    void startCheck(QString devPath);
    void stopCheck();
    f3_launcher_report getReport();
    int getStage();
    bool setOption(QString key, QString value);
    QString getOption(QString key);
    void startFix();
    QString f3_cui_output;
    int progress;

signals:
    void f3_launcher_status_changed(f3_launcher_status status);
    void f3_launcher_error(f3_launcher_error_code errCode);

private:
    QProcess f3_cui;
    QTimer timer;
    QString devPath;
    QString f3_path;
    QMap<QString,QString> options;
    bool showProgress;
    int stage;
    f3_launcher_status status;
    f3_launcher_error_code errCode;

    void emitError(f3_launcher_error_code errorCode);
    bool probeCommand(QString command);
    float probeVersion();
    bool probeDiskFull(QString& devPath);
    bool probeCacheFile(QString& devPath);
    int parseOutput();

private slots:
    void on_f3_cui_finished();
    void on_timer_timeout();
};

#endif // F3_LAUNCHER_H
