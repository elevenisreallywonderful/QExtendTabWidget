#include "TabPopup.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>

TabPopup::TabPopup(QWidget *parent)
    : QWidget(parent)
{}

void TabPopup::setContentWidget(QWidget *page)
{
    if(!page)
        return;
    content=page;
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(page);
}

QWidget* TabPopup::contentWidget() const
{
    return content;
}

bool TabPopup::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseButtonRelease:
    case QEvent::NonClientAreaMouseButtonRelease:
    {
       emit dragRelease(QCursor::pos());
    }
        break;
    case QEvent::Move:
    {
        emit dragging(QCursor::pos());
    }
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
