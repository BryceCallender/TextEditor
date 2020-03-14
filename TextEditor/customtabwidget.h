#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDrag>
#include <QPainter>

#include "customtabbar.h"

class CustomTabWidget: public QTabWidget
{
public:
    explicit CustomTabWidget(QWidget *parent = nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // CUSTOMTABWIDGET_H
