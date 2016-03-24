#ifndef F3_LAUNCHER_H
#define F3_LAUNCHER_H
#include <QMainWindow>
#include <QProcess>

enum f3_launcher_status
{
    f3_launcher_ready = 0,
    f3_launcher_running = 1,
    f3_launcher_finished = 2,
    f3_launcher_stopped = 3,
    f3_launcher_path_incorrect = 128,
    f3_launcher_unknownError = 255,
};


class f3_launcher : public QObject
{
    Q_OBJECT

public:
    f3_launcher();
    ~f3_launcher();
    void startCheck(QString& devPath);
    void stopCheck();
    QString f3_cui_output;

signals:
    void f3_launcher_status_changed(f3_launcher_status status);

private:
    QProcess f3_cui;
    QString devPath;
    int stage;
    int parseOutput();

private slots:
    void on_f3_cui_finished();
};

#endif // F3_LAUNCHER_H
