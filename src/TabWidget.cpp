#include "TabWidget.h"
#include "TabPopup.h"
#include "TabBar.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPixmap>
#include <QMimeData>
#include <QPainter>
#include <QCursor>
#include <QDrag>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent),
      mpLastPopup(nullptr),
      mnLastPos(-1)
{
    setAttribute(Qt::WA_StyledBackground);
    initTabBar();
}

void TabWidget::initTabBar()
{
    TabBar *bar = new TabBar(this);
    setTabBar(bar);
    connect(bar, &QTabBar::tabCloseRequested, this, &QTabWidget::removeTab);
    connect(bar, &TabBar::dragging, this, &TabWidget::draggingTabBar);
    connect(bar, &TabBar::dropped, this, &TabWidget::dropPopup);
}

void TabWidget::detachTab(int index)
{
    if (index < 0 || index >= count()) {
        return;
    }

    QWidget *draggedWidget = widget(index);
    if (!draggedWidget) {
        return;
    }

    mpLastPopup = new TabPopup();
    mpLastPopup->setWindowIcon(tabIcon(index));
    mpLastPopup->setWindowTitle(tabText(index));

    mpLastPopup->activateWindow();
    mpLastPopup->resize(draggedWidget->size());
    mpLastPopup->setAttribute(Qt::WA_DeleteOnClose);
    mpLastPopup->setContentWidget(draggedWidget);
    mpLastPopup->setFocus();
    mpLastPopup->show();
    draggedWidget->show();

    connect(mpLastPopup, &TabPopup::dragging, this, &TabWidget::draggingPopup);
    connect(mpLastPopup, &TabPopup::dragRelease, this, &TabWidget::dropPopup);
}

int TabWidget::calcPos(QPoint pt)
{
    QPoint localPt = tabBar()->mapFromGlobal(pt);
    QRect rt = tabBar()->contentsRect();
    if (rt.contains(localPt))
    {
        for (int i = 0; i < count(); i++) {
            rt = tabBar()->tabRect(i);
            if (rt.contains(localPt)) {
                return (localPt.x() < rt.center().x()) ? i : i + 1;
            }
        }
        return count();
    }
    else
    {
        return -1;
    }
}

void TabWidget::draggingTabBar(int i, QPoint offset)
{
    if (!tabBar()->tabRect(i).contains(tabBar()->mapFromGlobal(QCursor::pos()))) {
        if (i >= 0) {
            detachTab(i);
        }
        if (mpLastPopup) {
            mpLastPopup->move(QCursor::pos() + QPoint(-10, 0) +  offset);
        }
    }
}

void TabWidget::draggingPopup(QPoint pt)
{
    if (!mpLastPopup) {
        return;
    }
    if (tabBar()->contentsRect().contains(tabBar()->mapFromGlobal(pt))) {
        mpLastPopup->setWindowOpacity(0.5);
        int i = calcPos(pt);
        if (mnLastPos == -1) {
            if (mpLastPopup->windowTitle().isEmpty()) {
                return;
            }

            mnLastPos = insertTab(i, new QWidget(), mpLastPopup->windowTitle());
            tabBar()->setCurrentIndex(mnLastPos);
        } else {
            if (mnLastPos != i) {
                removeTab(mnLastPos);
                mnLastPos = insertTab(i, new QWidget(), mpLastPopup->windowTitle());
                tabBar()->setCurrentIndex(mnLastPos);
            }
        }
    } else {
        if (!mpLastPopup) return;
        mpLastPopup->setWindowOpacity(1);

        if (mnLastPos != -1) {
            removeTab(mnLastPos);
            mnLastPos = -1;
        }
    }

}

void TabWidget::dropPopup(QPoint pt)
{
    if (!mpLastPopup) {
        return;
    }
    mpLastPopup->setWindowOpacity(1);

    if (mnLastPos != -1) {
        removeTab(mnLastPos);
        mnLastPos = -1;
    }
    QWidget *widget = mpLastPopup->contentWidget();
    if (!widget) {
        return;
    }
    int i = calcPos(pt);
    if (i < 0) return;
    insertTab(i, widget, mpLastPopup->windowIcon(), mpLastPopup->windowTitle());
    setCurrentIndex(i);
    delete mpLastPopup;
    mpLastPopup = nullptr;
}
