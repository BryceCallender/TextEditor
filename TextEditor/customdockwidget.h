#ifndef CUSTOMDOCKWIDGET_H
#define CUSTOMDOCKWIDGET_H

#include <QDockWidget>
#include <QDragEnterEvent>
#include <QDebug>

#include "customtabwidget.h"

/*! \class CustomDockWidget
    \brief Docking widget with custom functionality.

    Custom functionality to handle application specific needs that default class does not support.
 */
class CustomDockWidget: public QDockWidget
{
public:
    //! Constructor
    /*!
      Responsible for setting up the rules for our docking widgets such as no movement, etc.
     */
    explicit CustomDockWidget(QWidget* parent = nullptr);
protected:
    //! closeEvent
    /*!
      This handles asking the user to save if they have edited tabs
     */
    void closeEvent(QCloseEvent *event);
private:
};

#endif // CUSTOMDOCKWIDGET_H
