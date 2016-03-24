#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "f3_launcher.h"

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

private:
    Ui::MainWindow *ui;
    f3_launcher cui;
    bool checking;

    void showStatus(const QString& string);
};

#endif // MAINWINDOW_H
