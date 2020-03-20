#include "customtabwidget.h"
#include "texttabwidget.h"

#include <QMessageBox>

int CustomTabWidget::tabParent = 0;
int CustomTabWidget::tabRemoving = 0;
int CustomTabWidget::tabIndex = 0;

CustomTabWidget::CustomTabWidget(QWidget *parent)
{
    setTabBar(new CustomTabBar(this));

    setMovable(true);
    setTabsClosable(true);

    clear();

    tabWidgetIndex = tabIndex;
    tabIndex++;

    addTab(new TextTabWidget(), "New Tab");
    addTab(new TextTabWidget(), "+");

    //Disable the + tab from accepting text incase the user tries to right text in it
    TextTabWidget* textTabWidget = (TextTabWidget*) widget(1);
    textTabWidget->getTextEdit()->setDisabled(true);

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

    QObject::connect(this, &CustomTabWidget::tabCloseRequested, this, &CustomTabWidget::tabCloseRequest);
    QObject::connect(this, &CustomTabWidget::tabBarClicked, this, &CustomTabWidget::tabClicked);
}

void CustomTabWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Left clicked";
        QPoint positionInTab = mapFromGlobal(mapToGlobal(event->pos()));

        //Getting the tab that was currently clicked
        int tab = tabBar()->tabAt(event->pos());

        if(tab == -1)
        {
            return;
        }

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

void CustomTabWidget::tabCloseRequest(int index)
{
    if(tabText(index).back() == '*')
    {
        int clicked = QMessageBox::warning(this, "Save?", "Would you like to save the file?", QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::Close);
        if(clicked == QMessageBox::Ok)
        {
            //on_actionSave_triggered();
            qDebug() << "Closing " + QString::number(index);
            removeTab(index);
        }
        else if(clicked == QMessageBox::Close)
        {
            qDebug() << "Closing " + QString::number(index);
            removeTab(index);
        }
    }
    else
    {
        qDebug() << "Closing " + QString::number(index);
        removeTab(index);
    }

    if(index > 0)
    {
        setCurrentIndex(index - 1);
    }
}

void CustomTabWidget::tabClicked(int index)
{
    //This will make sure to insert where the last tab is and they cannot move it or itll make a new tab
    if(index == count() - 1)
    {
        insertTab(index, new TextTabWidget(), "New Tab");
        setCurrentIndex(index);
    }
}
