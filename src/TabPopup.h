#ifndef TABPOPUP_H
#define TABPOPUP_H

#include <QWidget>

class TabPopup : public QWidget
{
    Q_OBJECT
public:
    explicit TabPopup(QWidget *parent = nullptr);

    void setContentWidget(QWidget *page);
    QWidget* contentWidget() const;

protected:
    bool event(QEvent*) override;

signals:
    void dragRelease(const QPoint &globalPos);
    void dragging(QPoint);
    
private:
    QWidget *content=nullptr;
};

#endif // TABPOPUP_H
