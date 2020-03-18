#include "TabTransferData.h"
#include "customtabbar.h"
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
    //Get data from the clipboard with this mime data tag and then read the bytes to convert it to TabTransferData
    QByteArray data = event->mimeData()->data("application/tab");
    QDataStream ds(&data, QIODevice::ReadWrite);
    TabTransferData testTabData;

    ds >> testTabData;

    TextTabWidget* newTab = new TextTabWidget();

    //Set the widget
    newTab->setTabsFileName(testTabData.filePath);
    newTab->getTextEdit()->setText(testTabData.text);
    setCurrentIndex(count() - 1);

    parent->insertTab(currentIndex(), newTab, "New Tab");

    setTabText(currentIndex(), testTabData.tabName);
}
