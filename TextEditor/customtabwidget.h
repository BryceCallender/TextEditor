#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDrag>
#include <QPainter>

#include "TabTransferData.h"
#include "customtabbar.h"
#include "texttabwidget.h"

class CustomTabWidget: public QTabWidget
{
public:
    explicit CustomTabWidget(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;
    TextTabWidget* getCurrentTabWidget();

    static int tabParent;
    static int tabRemoving;
    static int tabIndex;
    int tabWidgetIndex;
};

#endif // CUSTOMTABWIDGET_H
