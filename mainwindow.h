#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "image.h"
#include "coloranalyzer.h"
#include "kinematicsprovider.h"
#include "EdgeImage.h"
#include "FRHT.h"

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
    void on_pushButton_clicked();
    void on_spn_horizon_valueChanged(double arg1);
    void on_spn_step_v1_valueChanged(double arg1);
    void on_spn_step_v2_valueChanged(double arg1);
    void on_spn_step_v3_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QImage monitor;
    QImage edgeMonitor;

    //-- Representations
    Image image;
    ColorAnalyzer colorAnalyzer;
    KinematicsProvider kinematicsProvider;
    EdgeImage edgeImage;
    FRHT circleDetection;

    //-- Functions
    bool loadImage();
    void colorAnalyze();
    void kinematicsUpdate();
    void edgeImageUpdate();
    void detectCircles();

    bool checkWhitePercentage(int cx, int cy, int r);
    int searchedColor(int x, int y, int& color, int& nonGreen);

    //-- Utulity Functions
    static QImage image2qimage(const Image& img);
    static inline double clamp(double num) { return (num>255)?255:(num<0?0:num); }
};

#endif // MAINWINDOW_H
