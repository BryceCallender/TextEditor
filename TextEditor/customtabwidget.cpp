#include "customtabwidget.h"
#include "mainwindow.h"

#include <QMessageBox>

int CustomTabWidget::tabParent = 0;
int CustomTabWidget::tabRemoving = 0;
int CustomTabWidget::tabIndex = 0;
int CustomTabWidget::currentSelectedTabIndex = 0;

CustomTabWidget::CustomTabWidget(QWidget *parent): QTabWidget(parent)
{
    setTabBar(new CustomTabBar(this));

    setMovable(true);
    setTabsClosable(true);

    clear();

    tabWidgetIndex = tabIndex;
    qDebug() << tabWidgetIndex;
    tabIndex++;

    addTab(new TextTabWidget(this), "New Tab");
    addTab(new TextTabWidget(this), "+");

    //Disable the + tab from accepting text incase the user tries to right text in it
    TextTabWidget* textTabWidget = (TextTabWidget*) widget(1);
    textTabWidget->getTextEdit()->setDisabled(true);
    textTabWidget->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);
    TextTabWidget* textConnection = (TextTabWidget*) widget(0);

    setAcceptDrops(true);

    //Registers the tab transfer data << and >> operators with this metatype (serialization)
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
    connect(textConnection->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
      this, SLOT(showContextMenu(const QPoint&)));
}

void CustomTabWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Left clicked";
        QPoint positionInTab = mapFromGlobal(mapToGlobal(event->pos()));

        //Getting the tab that was currently clicked
        int tab = tabBar()->tabAt(event->pos());

        MainWindow* mainWindow = nullptr;

        //If this has no dock widget take this route
        if(dynamic_cast<MainWindow*>(parentWidget()) != nullptr)
        {
            mainWindow = reinterpret_cast<MainWindow*>(parentWidget());
        }
        else
        { //everything else is inside of a dock widget so it has one more parent to get through
            mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget());
        }

        if(tab == -1)
        {    
            mainWindow->showContextMenu(positionInTab);
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

        data.text = textTab->getTextEdit()->toHtml();
        data.filePath = textTab->getTabFileName();
        data.tabName = tabBar()->tabText(tab);
        data.cursorPosition = textTab->getTextEdit()->textCursor().position();
        data.fontInformation = mainWindow->getFontInformation();

        QByteArray tabAsByte;
        QDataStream dataStream(&tabAsByte, QIODevice::ReadWrite);

        dataStream << data;

        QMimeData* mimeData = new QMimeData();
        mimeData->setData("application/tab", tabAsByte);

        //Start the drag event
        QDrag* drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos() - positionInTab);
        drag->exec(Qt::MoveAction);
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
    CustomTabWidget::currentSelectedTabIndex = tabWidgetIndex;

    if(tabText(index).back() == '*')
    {
        int clicked = QMessageBox::warning(this, "Save?", "Would you like to save the file?", QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::No);
        if(clicked == QMessageBox::Ok)
        {
            //on_actionSave_triggered();

            MainWindow* mainWindow;
            //If this has no dock widget take this route
            if(dynamic_cast<MainWindow*>(parentWidget()) != nullptr)
            {
                mainWindow = reinterpret_cast<MainWindow*>(parentWidget());
            }
            else
            { //everything else is inside of a dock widget so it has one more parent to get through
                mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget());
            }

            mainWindow->save();
            qDebug() << "Closing " + QString::number(index);
            removeTab(index);
        }
        else if(clicked == QMessageBox::No)
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
    CustomTabWidget::currentSelectedTabIndex = tabWidgetIndex;
    qDebug() << "Tab widget index" << tabWidgetIndex;

    //This will make sure to insert where the last tab is and they cannot move it or itll make a new tab
    if(index == count() - 1)
    {
        insertTab(index, new TextTabWidget(this), "New Tab");
        setCurrentIndex(index);

    }
    else
    {
        MainWindow* mainWindow;
        //If this has no dock widget take this route
        if(dynamic_cast<MainWindow*>(parentWidget()) != nullptr)
        {
            mainWindow = reinterpret_cast<MainWindow*>(parentWidget());
        }
        else
        { //everything else is inside of a dock widget so it has one more parent to get through
            mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget());
        }

        qDebug() << mainWindow;

        mainWindow->setWindowTitle(getCurrentTabWidget()->getTabFileName());
        getCurrentTabWidget()->getTextEdit()->setFocus();

    }
    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, dynamic_cast<MainWindow*>(parentWidget()->parentWidget()), &MainWindow::fileChanged);

    qDebug() << "Current tab widget:" << CustomTabWidget::currentSelectedTabIndex;
}
