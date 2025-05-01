#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QClipboard>
#include <QMainWindow>
#include <QRect>

#include "overlaywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:

    // Cannot rename this two, because the generated ui_mainwindow will auto
    // generate this name as below.
    void on_defineRectangleButton_clicked();
    void on_screenshotButton_clicked();

    void HandleRectangleSelected(const QRect &rect);

  private:
    Ui::MainWindow *ui;
    QRect selected_area;
    OverlayWindow *overlay = nullptr;

    void ReadRectFromFile();
    void WriteRectToFile();

    void ScreenShot();

    static constexpr auto filename_ = "./rectValue";
};

#endif  // MAINWINDOW_H

