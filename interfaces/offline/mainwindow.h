#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "image.h"
#include "balldetector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QImage monitor, edgeMonitor;

    MVision::Image image;
    MVision::BallDetector ballDetector;

    //-- Utulity Functions
    static QImage image2qimage(const MVision::Image& img);
    bool loadImage();
};

#endif // MAINWINDOW_H
