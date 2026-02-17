#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>


QTranslator appTranslator;

int main(int argc, char *argv[])
{
    // Enable scaling for HiDPI device
    if (qgetenv("QT_SCALE_FACTOR").isEmpty() &&
        qgetenv("QT_SCREEN_SCALE_FACTORS").isEmpty())
    {
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    }

    QApplication a(argc, argv);

    QLocale currentLocale = QLocale::system();
    if (appTranslator.load(QString(":/translations/f3-qt_").
                           append(currentLocale.name())))
        a.installTranslator(&appTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
