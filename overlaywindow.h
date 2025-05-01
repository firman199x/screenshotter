#ifndef OVERLAYWINDOW_H
#define OVERLAYWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QRect>
#include <QPaintEvent>

class OverlayWindow : public QWidget {
    Q_OBJECT

public:
    OverlayWindow(QWidget *parent = nullptr);
    ~OverlayWindow() override = default;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void rectangleSelected(const QRect &rect);

private:
    bool isSelecting = false;
    QPoint startPoint;
    QRect selectionRect;
};

#endif // OVERLAYWINDOW_H

