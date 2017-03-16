#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <qpainter.h>


#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    image(Image()),
    colorAnalyzer(image),
    edgeImage(image),
    circleDetection(edgeImage)
{
    ui->setupUi(this);

    ui->spn_horizon->setValue(100);
    ui->spn_step_v1->setValue(expStep__);
    ui->spn_step_v2->setValue(expCStep__);
    ui->spn_step_v3->setValue(horizonOffset__);

    if (!loadImage())
        return;
    on_pushButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage MainWindow::image2qimage(const Image& img)
{
    QImage result = QImage(img.width(), img.height(), QImage::Format_RGB888);
    for (unsigned int y=0; y<img.height(); ++y)
        for (unsigned int x=0; x<img.width(); ++x)
        {
            Image::Pixel p = img.getPixel(x, y);

            QColor c;

            c.setRed(  CYCbCr2R(p.y, p.cb, p.cr));
            c.setGreen(CYCbCr2G(p.y, p.cb, p.cr));
            c.setBlue( CYCbCr2B(p.y, p.cb, p.cr));

            result.setPixel(x, y, c.rgb());
        }

    return result;
}

void MainWindow::detectCircles()
{
//    return;
    circleDetection.update(preferedPoints);
    preferedPoints.clear();

    QPainter* qpn = new QPainter(&monitor);


    std::vector<Vector3D> circles = circleDetection.extractedCircles();
    for (unsigned i=0; i<circles.size(); ++i)
    {
        Vector3D& circle = circles.at(i);
        qpn->setPen(Qt::darkYellow);
        qpn->drawEllipse(QPointF(circle.x, circle.y), (qreal)circle.z, (qreal)circle.z);


        //-- Run Sanity checks:
        circleDetection.resizeCirlcle(circle, colorAnalyzer.boundary(circle.x) /*kinematicsProvider.getHorizon()*/, colorAnalyzer, *qpn);


        //-- First Step Size Check
        if (circle.z < 2.5 || circle.z > 80) // [TODO] : make this config : ball max size in px
            continue;

        qpn->setPen(Qt::darkGreen);
        qpn->drawEllipse(QPointF(circle.x, circle.y), (qreal)circle.z, (qreal)circle.z);

        //-- Check green percentage
        if (!checkWhitePercentage(circle.x, circle.y, circle.z))
            continue;

        //-- Adding Prefered Points
        preferedPoints.push_back(Vector2D(circle.x, circle.y-circle.z));
        preferedPoints.push_back(Vector2D(circle.x, circle.y));

        qpn->setPen(QPen(Qt::yellow, 3));
        qpn->drawEllipse(QPointF(circle.x, circle.y), (qreal)circle.z, (qreal)circle.z);

        // [TODO] : sorting is needed in order to deliver the best result
        break; //-- Deliver only one result
    }

    delete qpn;

    edgeMonitor = image2qimage(edgeImage);
    edgeMonitor = edgeMonitor.scaled(edgeMonitor.width(), edgeMonitor.height());
}

bool MainWindow::checkWhitePercentage(int cx, int cy, int r)
{
  int whitePixel=0, nonGreenPixels = 0, totalSearchedPixel=0;
  for (int sx=0; sx<r; ++sx)
  {
    const int& sY = sqrt(r*r - sx*sx); //-- calculating maximum sy in the mentioned sx
    for (int sy=0; sy<sY; ++sy)
    {
      totalSearchedPixel += searchedColor(cx+sx, cy+sy, whitePixel, nonGreenPixels);
      totalSearchedPixel += searchedColor(cx-sx, cy+sy, whitePixel, nonGreenPixels);

      totalSearchedPixel += searchedColor(cx+sx, cy-sy, whitePixel, nonGreenPixels);
      totalSearchedPixel += searchedColor(cx-sx, cy-sy, whitePixel, nonGreenPixels);
    }
  }

  if (totalSearchedPixel == 0)
    return false;

// [FIXME] : move these
#define minWhitePercentage (0.05)
#define minNonGreenPercentage (0.7)


  //-- Reject balls with less than 85% white pixels
  if ((float)whitePixel/(float)totalSearchedPixel < minWhitePercentage ||
      (float)nonGreenPixels/(float)totalSearchedPixel < minNonGreenPercentage)
    return false;
  return true;
}

int MainWindow::searchedColor(int x, int y, int& color, int& nonGreen)
{
  if (x > -1 && x < image.width() && y > -1 && y < image.height())
  {
    nonGreen += colorAnalyzer.notGreen(x, y);
    color += colorAnalyzer.isWhite(x, y);
    return 1;
  }
  return 0;
}

void MainWindow::edgeImageUpdate()
{
    edgeImage.originY = kinematicsProvider.getHorizon();
    edgeImage.update();

    const std::vector<Vector2D>& points = edgeImage.edgePoints();
    for (unsigned int i=0; i<points.size(); i++)
    {
        if (points.at(i).x < 0 || points.at(i).y < 0 || points.at(i).x>= monitor.width() || points.at(i).y>= monitor.height())
            std::cerr << points.at(i).x << ", " << points.at(i).y << std::endl;
        else
            monitor.setPixel(points.at(i).x, points.at(i).y, QColor(Qt::red).rgb());

//        monitor.setPixel(points.at(i).x-1, points.at(i).y-1, QColor(Qt::red).rgb());
//        monitor.setPixel(points.at(i).x,   points.at(i).y-1, QColor(Qt::red).rgb());
//        monitor.setPixel(points.at(i).x+1, points.at(i).y-1, QColor(Qt::red).rgb());

//        monitor.setPixel(points.at(i).x-1, points.at(i).y, QColor(Qt::red).rgb());
//        monitor.setPixel(points.at(i).x+1, points.at(i).y, QColor(Qt::red).rgb());

//        monitor.setPixel(points.at(i).x-1, points.at(i).y+1, QColor(Qt::red).rgb());
//        monitor.setPixel(points.at(i).x,   points.at(i).y+1, QColor(Qt::red).rgb());
//        monitor.setPixel(points.at(i).x+1, points.at(i).y+1, QColor(Qt::red).rgb());
    }
}

void MainWindow::kinematicsUpdate()
{
    for (int x=0; x<monitor.width(); ++x)
        monitor.setPixel(x, kinematicsProvider.getHorizon(), QColor(Qt::red).rgb());
}

bool MainWindow::loadImage()
{
//    QImage qimg = QImage("/home/aref/workspace/humanoid/images/a-short.png").scaled(500, 600, Qt::KeepAspectRatio);
    QImage qimg = QImage("/home/aref/workspace/humanoid/images/b.jpg").scaled(500, 600, Qt::KeepAspectRatio);
    Image tempImage;
    image.resize(qimg.width(), qimg.height());
    tempImage.resize(qimg.width(), qimg.height());
    for (unsigned int y=0; y<(unsigned)qimg.height(); ++y)
        for (unsigned int x=0; x<(unsigned)qimg.width(); ++x)
        {
            Image::Pixel p;
            QColor c = QColor(qimg.pixel(x, y));

            p.y  = CRGB2Y(c.red(), c.green(), c.blue());
            p.cr = CRGB2Cr(c.red(), c.green(), c.blue());
            p.cb = CRGB2Cb(c.red(), c.green(), c.blue());

            tempImage.getPixel(x, y) = p;
        }

    //-- Convolute a median filter 3x3
    for (unsigned int y=0; y<image.height(); ++y)
        for (unsigned int x=0; x<image.width(); ++x)
        {
            Image::Pixel result;

            if (x == 0 || x == image.width()-1 || y == 0 || y == image.height()-1)
            {
                result = tempImage.getPixel(x, y);
            }
            else
            {
                std::vector<unsigned char> ch1, ch2, ch3;

                for (int i=-1; i<=1; ++i)
                    for (int j=-1; j<=1; ++j)
                    {
                        ch1.push_back(tempImage.getPixel(x+j, y+j).y);
                        ch2.push_back(tempImage.getPixel(x+j, y+j).cb);
                        ch3.push_back(tempImage.getPixel(x+j, y+j).cr);
                    }

                std::sort(ch1.begin(), ch1.end());
                std::sort(ch2.begin(), ch2.end());
                std::sort(ch3.begin(), ch3.end());


                result = Image::Pixel(ch1.at(5), ch2.at(5), ch3.at(5));
            }
            image.getPixel(x, y) = result;
        }

    return true;
}

void MainWindow::colorAnalyze()
{
    colorAnalyzer.update();

//    for (unsigned int y=0; y<image.height(); ++y)
//        for (unsigned int x=0; x<image.width(); ++x)
//            if (colorAnalyzer.isWhite(x, y))
//                monitor.setPixel(x, y, QColor(Qt::red).rgb());


    QPainter* qpn = new QPainter(&monitor);
    qpn->setPen(QPen(Qt::red, 1));
    for (unsigned i=1; i<image.width(); ++i)
        qpn->drawLine(i, colorAnalyzer.boundary(i), i-1, colorAnalyzer.boundary(i-1));
    qpn->setPen(QPen(Qt::yellow, 3));
    for (unsigned i=0; i<image.width(); ++i)
        qpn->drawPoint(i, colorAnalyzer.boundary(i));


//    qpn->setPen(QPen(Qt::darkRed, 1));
//    for (unsigned i=1; i<colorAnalyzer.test3.size(); ++i)
//        qpn->drawLine(colorAnalyzer.test3.at(i).x, colorAnalyzer.test3.at(i).y, colorAnalyzer.test3.at(i-1).x, colorAnalyzer.test3.at(i-1).y);
//    qpn->setPen(QPen(Qt::green, 3));
//    for (unsigned i=0; i<colorAnalyzer.test3.size(); ++i)
//        qpn->drawPoint(colorAnalyzer.test3.at(i).x, colorAnalyzer.test3.at(i).y);

    delete qpn;

//    for (unsigned int y=0; y<image.height(); ++y)
//        for (unsigned int x=0; x<image.width(); ++x)
//            if (colorAnalyzer.isGreen(x, y))
//                monitor.setPixel(x, y, QColor(Qt::green).rgb());

//    for (unsigned int x=0; x<256; ++x)
//    {
//        const double d = (double)image.height() * (double)colorAnalyzer.test[x] / (double)colorAnalyzer.test2;
//        for (int y=0; y<d; ++y)
//            monitor.setPixel(x, image.height() - y - 1, QColor(Qt::red).rgb());
//    }
}

void MainWindow::on_radioButton_clicked()
{
    if (ui->radioButton->isChecked())
        ui->monitor->setPixmap(QPixmap::fromImage(monitor));
    else
        ui->monitor->setPixmap(QPixmap::fromImage(edgeMonitor));

}

void MainWindow::on_radioButton_2_clicked()
{
    on_radioButton_clicked();
}

void MainWindow::on_pushButton_clicked()
{
    monitor = image2qimage(image);

    EdgeImage::expStep = ui->spn_step_v1->value();
    EdgeImage::expCStep = ui->spn_step_v2->value();
    EdgeImage::horizonOffset = ui->spn_step_v3->value();

    colorAnalyze();
    kinematicsUpdate();
    edgeImageUpdate();
    detectCircles();

    on_radioButton_clicked();
}

void MainWindow::on_spn_horizon_valueChanged(double )
{
    //    on_pushButton_clicked();
}

void MainWindow::on_spn_step_v1_valueChanged(double )
{
    //    edgeImage.createLookup();
    //    on_pushButton_clicked();
}

void MainWindow::on_spn_step_v2_valueChanged(double )
{
    //    edgeImage.createLookup();
    //    on_pushButton_clicked();
}

void MainWindow::on_spn_step_v3_valueChanged(double )
{
    //    edgeImage.createLookup();
    //    on_pushButton_clicked();
}
