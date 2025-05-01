#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "overlaywindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QClipboard>
#include <QImage>
#include <QMessageBox>

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

    QScreen *screen = QGuiApplication::primaryScreen();
    QImage screenshot = screen->grabWindow(0).toImage();

    if (screenshot.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to capture screenshot.");
        return;
    }

    QImage croppedImage = screenshot.copy(selectionRect);
    QApplication::clipboard()->setImage(croppedImage);

    QMessageBox::information(this, "Success", "Screenshot copied to clipboard.");
}

