#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "f3_launcher.h"
#include "helpwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_cui_status_changed(f3_launcher_status status);

private slots:
    void on_buttonCheck_clicked();
    void on_buttonExit_clicked();
    void on_buttonSelectPath_clicked();
    void on_buttonHelp_clicked();
    void on_timer_timeout();

private:
    Ui::MainWindow *ui;
    f3_launcher cui;
    QTimer timer;
    HelpWindow help;
    bool checking;
    int timerTarget;

    void showStatus(const QString& string);
    void clearStatus();
    void showCapacity(int value);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
