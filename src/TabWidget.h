#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

private:
    void initTabBar();
    void detachTab(int index);

    int calcPos(QPoint pt);

    TabPopup* mpLastPopup;
    int mnLastPos;

    

private slots:
    void draggingTabBar(int i, QPoint);
    void draggingPopup(QPoint);
    void dropPopup(QPoint);
};

#endif // TABWIDGET_H
