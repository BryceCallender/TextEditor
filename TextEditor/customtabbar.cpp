#include "customtabbar.h"

CustomTabBar::CustomTabBar(QWidget *parent): QTabBar(parent)
{
    connect(this,
            &QTabBar::tabMoved,
            this,
            &CustomTabBar::handleTabMovement);
}

void CustomTabBar::handleTabMovement(int from, int to)
{
    qDebug() << "Tab moved";
    if(from == count() - 1)
    {
        qDebug() << "Moved the +";
        moveTab(from-1, from);
    }
}

void CustomTabBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Left clicked";
        QPoint positionInTab = mapFromGlobal(mapToGlobal(event->pos()));

        int tab = tabAt(event->pos());
        QRect tabRect = this->tabRect(tab);

        QPixmap pixmap = QPixmap(tabRect.size());
        QPoint point;
        QRegion region(tabRect);
        this->render(&pixmap, point, region);

        QMimeData* mimeData = new QMimeData();

        QDrag* drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        //this->setCursor(Qt::OpenHandCursor);
        drag->setHotSpot(event->pos() - positionInTab);
        drag->exec();
    }

    QTabBar::mousePressEvent(event);
}

void CustomTabBar::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse moving";
    this->setCursor(Qt::ClosedHandCursor);

    QTabBar::mouseMoveEvent(event);
}

void CustomTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse released";
    this->setCursor(Qt::ArrowCursor);

    QTabBar::mouseReleaseEvent(event);
}
