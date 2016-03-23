#ifndef F3_LAUNCHER_H
#define F3_LAUNCHER_H
#include <QMainWindow>
#include <QProcess>


class f3_launcher
{
public:
    f3_launcher();
    void startCheck(QMainWindow* window,QString& devPath);
    void stopCheck();
private:
    QProcess f3_cui;
};

#endif // F3_LAUNCHER_H
