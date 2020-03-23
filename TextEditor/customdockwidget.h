#ifndef CUSTOMDOCKWIDGET_H
#define CUSTOMDOCKWIDGET_H

#include <QDockWidget>
#include <QDragEnterEvent>
#include <QDebug>

class CustomDockWidget: public QDockWidget
{
public:
    explicit CustomDockWidget(QWidget* parent = nullptr);
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CUSTOMDOCKWIDGET_H
