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
  // Cannot rename this method because inheritted from QWidget
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

signals:
    void RectangleSelected(const QRect &rect);

private:
    bool isSelecting = false;
    QPoint start_point_;
    QPoint end_point_;
    QRect selected_area;
    QRect drawRec;
};

#endif // OVERLAYWINDOW_H

