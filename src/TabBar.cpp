#include "TabBar.h"
#include <QDebug>

#include <QApplication>
#include <QMouseEvent>

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setMovable(true);
    setTabsClosable(true);
    setElideMode(Qt::ElideNone);
    setDocumentMode(true);
}

void TabBar::mousePressEvent(QMouseEvent *event)
{
    draggedIndex = tabAt(event->pos());

    if(event->button()==Qt::LeftButton && draggedIndex >= 0)
    {
        theDragPress=true;

        QRect rect = tabRect(draggedIndex);
        mPressedOffset = rect.topLeft() - event->pos();
    }

    QTabBar::mousePressEvent(event);
}

void TabBar::mouseMoveEvent(QMouseEvent *event)
{
    if(theDragPress && event->buttons() & Qt::LeftButton)
    {
        emit dragging(draggedIndex, mPressedOffset);

        if (!tabRect(draggedIndex).contains(mapFromGlobal(QCursor::pos()))) {
            draggedIndex = -1;
        }
    }
    QTabBar::mouseMoveEvent(event);
}

void TabBar::mouseReleaseEvent(QMouseEvent *event)
{
    QTabBar::mouseReleaseEvent(event);
    theDragPress=false;
    theDragOut=false;
    qDebug() << "release";
}
