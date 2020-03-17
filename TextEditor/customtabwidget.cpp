#include "customtabwidget.h"
#include "texttabwidget.h"

int CustomTabWidget::tabParent = 0;
int CustomTabWidget::tabRemoving = 0;
int CustomTabWidget::tabIndex = 0;

CustomTabWidget::CustomTabWidget(QWidget *parent)
{
    setTabBar(new CustomTabBar());

    setMovable(true);
    setTabsClosable(true);

    clear();

    tabWidgetIndex = tabIndex;
    tabIndex++;

    addTab(new TextTabWidget(), "New Tab");
    addTab(new TextTabWidget(), "+");

    setAcceptDrops(true);

    qRegisterMetaTypeStreamOperators<TabTransferData>("TabTransferData");

#if defined(Q_OS_WIN) || defined(Q_OS_UNIX)
    tabBar()->tabButton(1, QTabBar::RightSide)->deleteLater();
    tabBar()->setTabButton(1, QTabBar::RightSide, 0);
#endif
#ifdef Q_OS_MACOS
    tabBar()->tabButton(1, QTabBar::LeftSide)->deleteLater();
    tabBar()->setTabButton(1, QTabBar::LeftSide, 0);
#endif
}

void CustomTabWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Left clicked";
        QPoint positionInTab = mapFromGlobal(mapToGlobal(event->pos()));

        //Getting the tab that was currently clicked
        int tab = tabBar()->tabAt(event->pos());
        QRect tabRect = tabBar()->tabRect(tab);
        tabParent = tabWidgetIndex;
        tabRemoving = tab;

        //Drawing the tab icon
        QPixmap pixmap = QPixmap(tabRect.size());
        QPoint point;
        QRegion region(tabRect);
        this->render(&pixmap, point, region);

        //Data going to be transferred
        TextTabWidget* textTab = (TextTabWidget*)widget(tab);
        TabTransferData data;

        data.text = textTab->getTextEdit()->toPlainText();
        data.highlighter = textTab->getSyntaxHighlighter();
        data.filePath = textTab->getTabFileName();
        QFileInfo fileInfo(data.filePath);
        data.tabName = fileInfo.fileName();

        QByteArray tabAsByte;
        QDataStream dataStream(&tabAsByte, QIODevice::ReadWrite);

        dataStream << data;

        QMimeData* mimeData = new QMimeData();
        mimeData->setData("application/tab", tabAsByte);

        //Start the drag event
        QDrag* drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        //this->setCursor(Qt::OpenHandCursor);
        drag->setHotSpot(event->pos() - positionInTab);
        Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    }

    QTabWidget::mousePressEvent(event);
}

TextTabWidget *CustomTabWidget::getCurrentTabWidget()
{
    int tabIndex = currentIndex();
    TextTabWidget* textTabWidget = (TextTabWidget*) widget(tabIndex);

    return textTabWidget;
}
