#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QClipboard>
#include "overlaywindow.h" // Include the overlay window

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_defineRectangleButton_clicked();
    void on_screenshotButton_clicked();
    void handleRectangleSelected(const QRect &rect); // New slot

private:
    Ui::MainWindow *ui;
    QRect selectionRect;
    OverlayWindow *overlay = nullptr; // Overlay instance
};

#endif // MAINWINDOW_H

