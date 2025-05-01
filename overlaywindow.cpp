#include "overlaywindow.h"

#include <QDebug>  // For console output
#include <QGuiApplication>
#include <QPainter>
#include <QPen>
#include <QScreen>

OverlayWindow::OverlayWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    // Calculate combined screen geometry
    QRect totalGeometry;
    for (const QScreen *screen : QGuiApplication::screens()) {
        totalGeometry |= screen->geometry();
    }
    resize(totalGeometry.width(), totalGeometry.height());
    move(totalGeometry.topLeft());

    raise();
    activateWindow();
}

void OverlayWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        end_point_ = event->pos();
        drawRec = QRect(start_point_, end_point_).normalized();
        selected_area.setBottomRight(event->globalPosition().toPoint());
        emit RectangleSelected(selected_area);
        close();
    }
}

void OverlayWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(255, 255, 255, 50));  // 128 = 50% opacity

    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(drawRec);
}

void OverlayWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        start_point_ = event->pos();
        end_point_ = start_point_;
        drawRec = QRect(start_point_, end_point_);
        selected_area.setTopLeft(event->globalPosition().toPoint());
        update();
    }
}

void OverlayWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        end_point_ = event->pos();
        drawRec = QRect(start_point_, end_point_).normalized();
        selected_area.setBottomRight(event->globalPosition().toPoint());
        update();
    }
}

