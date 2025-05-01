#include "overlaywindow.h"
#include <QPainter>
#include <QPen>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug> // For console output

OverlayWindow::OverlayWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); // Enable transparency
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    QRect totalGeometry;
    for (const QScreen *screen : QGuiApplication::screens()) {
        totalGeometry |= screen->geometry();
    }
    resize(totalGeometry.width(), totalGeometry.height());
    move(totalGeometry.topLeft());
}

void OverlayWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isSelecting = true;
        startPoint = event->globalPosition().toPoint();
        selectionRect.setTopLeft(startPoint);
        selectionRect.setBottomRight(startPoint);
        update();
    }
}

void OverlayWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isSelecting && (event->buttons() & Qt::LeftButton)) {
        selectionRect.setBottomRight(event->globalPosition().toPoint());
        update();
    }
}

void OverlayWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (isSelecting && event->button() == Qt::LeftButton) {
        selectionRect.setBottomRight(event->globalPosition().toPoint());
        emit rectangleSelected(selectionRect);
        close();
    }
}

void OverlayWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Fill the background with semi-transparent white (50% opacity)
    painter.fillRect(rect(), QColor(255, 255, 255, 128)); // 128 = 50% opacity

    if (isSelecting && !selectionRect.isNull()) {
        painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
        painter.drawRect(selectionRect);
    }
}

