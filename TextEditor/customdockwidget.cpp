#include "customdockwidget.h"
#include "mainwindow.h"

CustomDockWidget::CustomDockWidget(QWidget *parent): QDockWidget(parent)
{

}

void CustomDockWidget::closeEvent(QCloseEvent *event)
{
    MainWindow* mainWindow = reinterpret_cast<MainWindow*>(parentWidget());


}

