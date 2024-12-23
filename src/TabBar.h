#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = nullptr);

protected:
    void mousePressEvent (QMouseEvent *event) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;

signals:
    void dragging(int index, QPoint offset);
    void dropped(QPoint offset);

private:
    bool theDragPress=false;

    int draggedIndex;
    QPoint mPressedOffset;
};


#endif // TABBAR_H
