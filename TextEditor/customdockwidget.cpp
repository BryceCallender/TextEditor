#include "customdockwidget.h"

CustomDockWidget::CustomDockWidget(QWidget *parent): QDockWidget(parent)
{
    setAcceptDrops(true);
}

void CustomDockWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "Dockwidget drag enter event";
    QPoint contactPosition = mapFromGlobal(mapToGlobal(event->pos()));

    //If the contact point is showing some sort of desire to detach
    if(contactPosition.x() > (size().width() * 0.75))
    {
        qDebug() << "New Tab window location";
    }
}

void CustomDockWidget::dragMoveEvent(QDragMoveEvent *event)
{

}

void CustomDockWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "test2";
}
