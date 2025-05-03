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

void MainWindow::ScreenShot() {
    hide();
    QList<QScreen *> screens = QGuiApplication::screens();
    QScreen *targetScreen = nullptr;
    for (QScreen *screen : screens) {
        if (screen->geometry().contains(selected_area)) {
            targetScreen = screen;
            break;
        }
    }

    if (!targetScreen) {
        qWarning() << "Selected area does not intersect with any screen.";
        show();
        return;
    }

    QRect screenRect = targetScreen->geometry();
    qreal devicePixelRatio = targetScreen->devicePixelRatio();
    QPixmap screenPixmap = targetScreen->grabWindow(0);
    QRect scaledSelectedArea(
        (selected_area.x() - screenRect.x()) * devicePixelRatio,
        (selected_area.y() - screenRect.y()) * devicePixelRatio,
        selected_area.width() * devicePixelRatio,
        selected_area.height() * devicePixelRatio
    );

    QPixmap croppedPixmap = screenPixmap.copy(scaledSelectedArea);
    QImage croppedImage = croppedPixmap.toImage().convertToFormat(
        QImage::Format_ARGB32_Premultiplied);

    QApplication::clipboard()->setPixmap(croppedPixmap);
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

