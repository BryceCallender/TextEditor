#include "customdockwidget.h"
#include "mainwindow.h"

CustomDockWidget::CustomDockWidget(QWidget *parent): QDockWidget(parent)
{

}

void CustomDockWidget::closeEvent(QCloseEvent *event)
{
    MainWindow* mainWindow = reinterpret_cast<MainWindow*>(parentWidget());

    QVector<CustomTabWidget*> tabWidget = mainWindow->getTabWidgets();

    int count = 0;
    for(int i = 0; i < tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->count(); i++)
    {
        if(tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i).back() == '*')
        {
            QString message, temp;
            if(tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i).left(tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i).size() - 1) == "New Tab")
            {
                temp.setNum(count + 1);
                count++;
                message = "New Tab " + temp;
            }
            else
            {
                message = tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i).left(tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i).size() - 1);
            }
            int clicked = QMessageBox::warning(this, "Save?", "Would you like to save " + message + "?", QMessageBox::Ok ,QMessageBox::No);
            if(clicked == QMessageBox::Ok)
            {
                mainWindow->save();
                qDebug() << "Closing " + QString::number(i);
                tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->removeTab(i);
                i--;
            }
            else if(clicked == QMessageBox::No)
            {
                qDebug() << "Closing " + QString::number(i);
                tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->removeTab(i);
                i--;
            }
        }
        else if(tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->tabText(i) != "+")
        {
            tabWidget.at(CustomTabWidget::currentSelectedTabIndex)->removeTab(i);
            i--;
        }
    }



}

