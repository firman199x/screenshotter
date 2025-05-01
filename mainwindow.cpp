#include "mainwindow.h"

#include <QClipboard>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QScreen>
#include <QTimer>

#include "overlaywindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ReadRectFromFile();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_defineRectangleButton_clicked()
{
    hide();
    overlay = new OverlayWindow();
    connect(overlay, &OverlayWindow::RectangleSelected, this,
            &MainWindow::HandleRectangleSelected);

    overlay->show();
    overlay->raise();
    overlay->activateWindow();
}

void MainWindow::HandleRectangleSelected(const QRect &rect)
{
    selected_area = rect.normalized();
    WriteRectToFile();
    overlay->deleteLater();
    show();

    QTimer::singleShot(1, [this]() {
            ScreenShot();
    });
}

void MainWindow::on_screenshotButton_clicked()
{
    if (!selected_area.isValid()) {
        QMessageBox::warning(this, "Warning",
                             "Please define a rectangle first.");
        return;
    }
    ScreenShot();
}

void MainWindow::ScreenShot()
{
    hide();

    QList<QScreen *> screens = QGuiApplication::screens();
    QRect totalGeometry = screens.first()->virtualGeometry();
    QImage combinedImage(totalGeometry.width(), totalGeometry.height(),
                         QImage::Format_ARGB32_Premultiplied);
    combinedImage.fill(Qt::transparent);

    QPainter painter(&combinedImage);
    for (QScreen *screen : screens) {
        QRect screenRect = screen->geometry();
        QImage screenShot = screen->grabWindow(0).toImage();
        painter.drawImage(screenRect.topLeft(), screenShot);
    }
    painter.end();

    QImage croppedImage = combinedImage.copy(selected_area);

    QApplication::clipboard()->setImage(croppedImage);

    show();
}

void MainWindow::ReadRectFromFile()
{
    QFile file(filename_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << filename_;
        return;
    }

    QTextStream in(&file);
    int x, y, width, height;
    in >> x >> y >> width >> height;
    file.close();

    selected_area = QRect(x, y, width, height);
}

void MainWindow::WriteRectToFile()
{
    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filename_;
        return;
    }

    QTextStream out(&file);
    out << selected_area.x() << " " << selected_area.y() << " "
        << selected_area.width() << " " << selected_area.height();
    file.close();
}

