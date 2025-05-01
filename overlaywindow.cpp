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

    // Debug prints
    qDebug() << "Overlay size:" << size();
    qDebug() << "Overlay position:" << pos();
    qDebug() << "Total screen geometry:" << totalGeometry;

    // Ensure the overlay is visible and on top
    raise();
    activateWindow();
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
        // QPoint overlayPos = pos(); // Overlay's global top-left corner
        // QRect adjustedRect = selectionRect.translated(-overlayPos.x(), -overlayPos.y());
        // painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
        // painter.drawRect(selectionRect);
    }
}

void OverlayWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isSelecting = true;
        startPoint = event->globalPosition().toPoint(); // Global coordinates
        selectionRect.setTopLeft(startPoint);
        selectionRect.setBottomRight(startPoint);
        update();
    }
}

void OverlayWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isSelecting && (event->buttons() & Qt::LeftButton)) {
        selectionRect.setBottomRight(event->globalPosition().toPoint()); // Global coordinates
        update();
    }
}

