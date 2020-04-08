#include <QApplication>
#include <QSplashScreen>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.show();

    quint32 seconds = QRandomGenerator::global()->bounded(4000,5000);
    qDebug() << "Waiting for" << seconds << "seconds";

    MainWindow w;

    QTimer::singleShot(seconds, &splash, SLOT(close()));
    QTimer::singleShot(seconds, &w, SLOT(show()));

    w.show();
    splash.finish(&w);
    return a.exec();
}
