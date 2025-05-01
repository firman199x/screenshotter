#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "overlaywindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QClipboard>
#include <QImage>
#include <QMessageBox>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_defineRectangleButton_clicked()
{
    hide(); // Hide main window
    overlay = new OverlayWindow(); // Create overlay without parent
    connect(overlay, &OverlayWindow::rectangleSelected, this, &MainWindow::handleRectangleSelected);

    // Ensure overlay is visible and focused:
    overlay->show();
    overlay->raise();
    overlay->activateWindow();
}

void MainWindow::handleRectangleSelected(const QRect &rect)
{
    selectionRect = rect.normalized();
    overlay->deleteLater();
    show(); // Show main window again
}

void MainWindow::on_screenshotButton_clicked()
{
    if (!selectionRect.isValid()) {
        QMessageBox::warning(this, "Warning", "Please define a rectangle first.");
        return;
    }

    // Hide the main window to exclude it from the screenshot
    hide();

    // Capture all screens and combine into one image
    QList<QScreen *> screens = QGuiApplication::screens();
    QRect totalGeometry = screens.first()->virtualGeometry();
    QImage combinedImage(totalGeometry.width(), totalGeometry.height(), QImage::Format_ARGB32_Premultiplied);
    combinedImage.fill(Qt::transparent);

    QPainter painter(&combinedImage);
    for (QScreen *screen : screens) {
        QRect screenRect = screen->geometry();
        QImage screenShot = screen->grabWindow(0).toImage();
        painter.drawImage(screenRect.topLeft(), screenShot);
    }
    painter.end();

    // Crop the combined image using the global selection rectangle
    QImage croppedImage = combinedImage.copy(selectionRect);

    QApplication::clipboard()->setImage(croppedImage);

    // Show the main window again
    show();

}

