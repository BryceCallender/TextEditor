#include "customtabwidget.h"

#include "texttabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent)
{
    setTabBar(new CustomTabBar());

    setAcceptDrops(true);
}

void CustomTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "test";
    QPoint contactPosition = mapFromGlobal(mapToGlobal(event->pos()));

    //If the contact point is showing some sort of desire to detach
    if(contactPosition.x() > (size().width() * 0.75))
    {
        qDebug() << "New Tab window location";
        addTab(new TextTabWidget(), "Test");
    }
}

void CustomTabWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "test2";
}
