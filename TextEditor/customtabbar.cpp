#include "TabTransferData.h"
#include "customtabbar.h"
#include "mainwindow.h"
#include "texttabwidget.h"

CustomTabBar::CustomTabBar(QWidget *parent): QTabBar(parent)
{
    connect(this,
            &QTabBar::tabMoved,
            this,
            &CustomTabBar::handleTabMovement);

    setAcceptDrops(true);

    this->parent = reinterpret_cast<CustomTabWidget*>(parent);
}

void CustomTabBar::handleTabMovement(int from, int to)
{
    Q_UNUSED(to);

    if(from == count() - 1)
    {
        moveTab(from-1, from);
    }
}

void CustomTabBar::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void CustomTabBar::dropEvent(QDropEvent *event)
{
    qDebug() << "Tab bar drop event detected";

    //Get data from the clipboard with this mime data tag and then read the bytes to convert it to TabTransferData
    QByteArray data = event->mimeData()->data("application/tab");
    QDataStream ds(&data, QIODevice::ReadWrite);
    TabTransferData testTabData;

    ds >> testTabData;

    TextTabWidget* newTab = new TextTabWidget(this->parent);

    //Set the widget
    newTab->getTextEdit()->setText(testTabData.text);
    int indexToInsert = count() - 1;
    parent->insertTab(indexToInsert, newTab, "New Tab");
    setTabText(indexToInsert, testTabData.tabName);
    newTab->setSyntaxHighlighter(testTabData.highlighter);
    newTab->setTabsFileName(testTabData.filePath);
    setCurrentIndex(indexToInsert);

    QTextCursor cursor = newTab->getTextEdit()->textCursor();
    cursor.setPosition(testTabData.cursorPosition);

    newTab->getTextEdit()->setTextCursor(cursor);

    MainWindow *mainWindow;

    //Parent of the CustomTabWidget is directly the mainwindow (standard beginning tabwidget)
    if(dynamic_cast<MainWindow*>(this->parent->parentWidget()) != nullptr)
    {
        mainWindow = reinterpret_cast<MainWindow*>(this->parent->parentWidget());
    }
    else //Parent of the CustomTabWidget is a QDockWidget and its parent is the MainWindow
    {
        mainWindow = reinterpret_cast<MainWindow*>(this->parent->parentWidget()->parentWidget());
    }


    CustomTabWidget::currentSelectedTabIndex = parent->tabWidgetIndex;
    qDebug() << "Current tab widget" << CustomTabWidget::currentSelectedTabIndex;
    mainWindow->removeTabFromWidget(CustomTabWidget::tabParent, CustomTabWidget::tabRemoving);
    mainWindow->setWindowTitle(testTabData.filePath);
    QObject::connect(newTab->getTextEdit(), &QTextEdit::textChanged, mainWindow, &MainWindow::fileChanged);
}
