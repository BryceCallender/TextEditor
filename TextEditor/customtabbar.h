#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QWidget>
#include <QTabBar>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
#include <QMimeData>
#include <QDrag>

#include "customtabwidget.h"

class CustomTabWidget;

/*! \class CustomTabBar
    \brief Tab bar with custom functionality.

    Custom functionality to handle application specific needs that default class does not support.
 */
class CustomTabBar: public QTabBar
{
public:
    //! Constructor
    /*!
      Connects the \a handleTabMovement to the QTabBar moved signal and allows the tab bar to accept drops when a tab is attempted to be dropped here.
     */
    explicit CustomTabBar(QWidget *parent = nullptr);

    //! Tab movement
    /*!
      Handles cases for when the current tab is moved beyond the '+' tab so that the '+' will always be at the end.
     */
    void handleTabMovement(int from, int to);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    CustomTabWidget* parent;
};

#endif // CUSTOMTABBAR_H
