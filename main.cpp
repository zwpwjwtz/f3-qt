#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Enable scaling for HiDPI device
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
