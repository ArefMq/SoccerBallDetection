#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pthread.h>

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
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QImage monitor, edgeMonitor;
    QImage himg;

    MVision::Image image;
    MVision::BallDetector ballDetector;
    bool imageLuck;

    //-- Utulity Functions
    static QImage image2qimage(const MVision::Image& img);
    bool loadImage();
    void debugUpdate(QPainter* qpn);

    //-- Camera part
    pthread_t thread; bool isRunning;
    void startThread();
    friend void* runCamera(void* p);

private slots:
    void dataRecieve();

signals:
    void dataCame();
};

#endif // MAINWINDOW_H
