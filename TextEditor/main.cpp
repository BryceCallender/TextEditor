#include <QApplication>
#include <QSplashScreen>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.show();

    quint32 seconds = QRandomGenerator::global()->bounded(2000,2500);
    qDebug() << "Waiting for" << seconds << "seconds";

    MainWindow w;

    QTimer::singleShot(seconds, &splash, SLOT(close()));
    QTimer::singleShot(seconds, &w, SLOT(show()));

    return a.exec();
}
