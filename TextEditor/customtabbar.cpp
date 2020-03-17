#include "customtabbar.h"
#include "texttabwidget.h"

CustomTabBar::CustomTabBar(QWidget *parent): QTabBar(parent)
{
    connect(this,
            &QTabBar::tabMoved,
            this,
            &CustomTabBar::handleTabMovement);

    setAcceptDrops(true);
}

void CustomTabBar::handleTabMovement(int from, int to)
{
    if(from == count() - 1)
    {
        moveTab(from-1, from);
    }
}

//void CustomTabBar::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::RightButton)
//    {
//        qDebug() << "Left clicked";
//        QPoint positionInTab = mapFromGlobal(mapToGlobal(event->pos()));

//        //Getting the tab that was currently clicked
//        int tab = tabAt(event->pos());
//        QRect tabRect = this->tabRect(tab);

//        //Drawing the tab icon
//        QPixmap pixmap = QPixmap(tabRect.size());
//        QPoint point;
//        QRegion region(tabRect);
//        this->render(&pixmap, point, region);

//        //Data going to be transferred
//        TextTabWidget* data = tabData(tab).value<TextTabWidget*>();

//        QByteArray tabAsByte;
//        QDataStream dataStream(&tabAsByte, QIODevice::ReadWrite);

//        dataStream << tabAsByte;


//        QMimeData* mimeData = new QMimeData();
//        mimeData->setData("", tabAsByte);

//        //Start the drag event
//        QDrag* drag = new QDrag(this);

//        drag->setMimeData(mimeData);
//        drag->setPixmap(pixmap);
//        //this->setCursor(Qt::OpenHandCursor);
//        drag->setHotSpot(event->pos() - positionInTab);
//        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction);

//        qDebug() << dropAction;
//    }

//    QTabBar::mousePressEvent(event);
//}

//void CustomTabBar::mouseMoveEvent(QMouseEvent *event)
//{
//    qDebug() << "Mouse moving";
//    this->setCursor(Qt::ClosedHandCursor);

//    QTabBar::mouseMoveEvent(event);
//}

//void CustomTabBar::mouseReleaseEvent(QMouseEvent *event)
//{
//    //qDebug() << "Mouse released";
//    this->setCursor(Qt::ArrowCursor);

//    QTabBar::mouseReleaseEvent(event);
//}

//void CustomTabBar::dropEvent(QDropEvent *event)
//{
//    qDebug() << "Dropped";
//}
