#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDrag>
#include <QPainter>

#include "TabTransferData.h"
#include "customtabbar.h"
#include "texttabwidget.h"

/*! \class CustomTabWidget
    \brief Tab Widget with custom functionality.

    This Custom tab widget is responsible for taking the behavior of the parent while also adding additional functionality such as supporting multiple tab widgets on one screen.
 */
class CustomTabWidget: public QTabWidget
{
public:
    //! Constructor
    /*!
      This constructor will set up the needed elements like custom tab bar, allowing tabs to be closable and move, and allowing the widget to accept tab drop events.
     */
    explicit CustomTabWidget(QWidget *parent = nullptr);

    //! Mouse Event
    /*!
      This handles the mouse press event. It is responsible for showing the tab move with the cursor if the user decides to move the tab to another widget.
     */
    void mousePressEvent(QMouseEvent *event) override;

    //! Getter
    /*!
      This will get the TextTabWidget that is associated with the current active tab in the widget. The TextTabWidget will have all the information necessary for other functionality.
     */
    TextTabWidget* getCurrentTabWidget();

    void tabCloseRequest(int index);
    void tabClicked(int index);

    static int tabParent; /*!< The parent index of the tab being ripped off. */
    static int tabRemoving; /*!< The index of the tab being ripped off. */
    static int tabIndex; /*!< The counter of how many tab widgets exist. */
    int tabWidgetIndex; /*!< The unqiue index of the current tab widget. */
};

#endif // CUSTOMTABWIDGET_H
