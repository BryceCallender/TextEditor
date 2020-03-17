#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QWidget>
#include <QTabBar>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
#include <QMimeData>
#include <QDrag>

class CustomTabBar: public QTabBar
{
public:
    explicit CustomTabBar(QWidget *parent = nullptr);

    void handleTabMovement(int from, int to);

//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//protected:
//    void dropEvent(QDropEvent *event);
};

#endif // CUSTOMTABBAR_H
