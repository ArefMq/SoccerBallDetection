#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <qpainter.h>
#include <unistd.h>

#include "modules/edgeimage.h"
#include "webcam.h"

#define XRES 640
#define YRES 480

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

#include "modules/coloranalyzer.h"

using namespace MVision;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ballDetector(BallDetector()),
    imageLuck(false),
    isRunning(false)
{
    ui->setupUi(this);
    himg = QImage(200, 200, QImage::Format_RGB888);

    startThread();
}

MainWindow::~MainWindow()
{
    isRunning = false;
    pthread_join(thread, 0);
    delete ui;
}

RGBImage frame;
void* runCamera(void* p)
{
    MainWindow* ptr = (MainWindow*)p;
    Webcam webcam("/dev/video1", XRES, YRES);
    usleep(1000000);

    ptr->isRunning = true;
    while (ptr->isRunning)
    {
        frame = webcam.frame();
        QImage qimg = QImage(frame.data, XRES, YRES, QImage::Format_RGB888);

        ptr->image.resize(qimg.width(), qimg.height());
        for (unsigned int y=0; y<(unsigned)qimg.height(); ++y)
            for (unsigned int x=0; x<(unsigned)qimg.width(); ++x)
            {
                Image::Pixel p;
                QColor c = QColor(qimg.pixel(x, y));

                p.y  = CRGB2Y(c.red(),  c.green(), c.blue());
                p.cr = CRGB2Cr(c.red(), c.green(), c.blue());
                p.cb = CRGB2Cb(c.red(), c.green(), c.blue());

                ptr->image.getPixel(x, y) = p;
            }

        ptr->monitor = ptr->image2qimage(ptr->image);
        ptr->ballDetector.update(ptr->image);
        ptr->edgeMonitor = ptr->image2qimage(ptr->ballDetector.debug_GetEdgeImage());

        QPainter* qpn = new QPainter(ptr->ui->radioButton->isChecked() ? (&ptr->monitor) : (&ptr->edgeMonitor));
        ptr->debugUpdate(qpn);

        ptr->ui->cycletime->setText(QString("Average time: ") + QString::number(ptr->ballDetector.averageCycleTime()));

        delete qpn;
        emit ptr->dataCame();
    }

    return 0;
}

void MainWindow::debugUpdate(QPainter* qpn)
{
    //-- Draw histogram
    {
        float hist[256];
        for (int i=0; i<256; ++i)
            hist[i] = log(ballDetector.debug_GetColorAnalyzer().debug_histogram[i] + 1);

        float stepMax=1;
        static int _i=0;
        if (_i++%10 == 0)
        {
            _i=0;
            for (int i=0; i<256; ++i)
                if (stepMax < hist[i])
                    stepMax = hist[i];
        }

        QPainter* qpn = new QPainter(&himg);
        qpn->setBrush(Qt::black);
        qpn->drawRect(0, 0, 200, 200);

        qpn->setBrush(Qt::green);
        for (int i=0; i<256; ++i)
        {
            if (abs(ballDetector.debug_GetColorAnalyzer().debug_peak-i) < 10)
                qpn->setPen(Qt::yellow);
            else
                qpn->setPen(Qt::green);

            qpn->drawLine(i, 200, i, 200-(hist[i] * 190.f / stepMax));
        }
        qpn->setPen(Qt::green);
        qpn->drawText(5, 20, QString("Cr : ") + QString::number(ballDetector.debug_GetColorAnalyzer().debug_peak));


        qpn->setPen(Qt::darkYellow);
        qpn->drawLine(100, 0, 100, 200);

        delete qpn;
    }

    if (ui->chk_ca->isChecked())
    {
        if (ui->chk_ca_fieldColor->isChecked())
        {
            qpn->setPen(Qt::green);
            for (unsigned int x=0; x<image.width(); ++x)
                for (unsigned int y=0; y<image.height(); ++y)
                    if (ballDetector.debug_GetColorAnalyzer().isGreen(x, y))
                        qpn->drawPoint(x, y);
        }

        qpn->setPen(QPen(Qt::red, 3));
        if (ui->chk_ca_runups->isChecked())
        {
            std::vector<Vector2D> debug_highestPoints = ballDetector.debug_GetColorAnalyzer().debug_highestPoints;
            for (unsigned i=0; i<debug_highestPoints.size(); ++i)
                qpn->drawPoint(debug_highestPoints.at(i).x, debug_highestPoints.at(i).y);
        }

        if (ui->chk_ca_fieldbndry->isChecked())
        {
            std::vector<int> fieldBoundaries = ballDetector.debug_GetColorAnalyzer().debug_getBoundaryPoints();
            for (unsigned int i=1; i<fieldBoundaries.size(); ++i)
                qpn->drawLine(i-1, fieldBoundaries.at(i-1), i, fieldBoundaries.at(i));
        }
    }

    typedef std::vector<Ball> Balls;
    const Balls& results = ballDetector.getResults();
    for (Balls::const_iterator itr=results.begin(); itr<results.end(); itr++)
    {
        const Ball& b = *itr;
        float x = b.PositionInImage()._translation.x,
              y = b.PositionInImage()._translation.y,
              r = b.PositionInImage()._radious;

        qpn->setPen(QPen(Qt::red, 3));
        qpn->drawEllipse(QPointF(x, y), r, r);
        qpn->setPen(QPen(Qt::red, 6));
        qpn->drawLine(x-r, y-r, x-r-30, y-r-30);
        qpn->drawLine(x-r, y-r, x-r-10, y-r-15);
        qpn->drawLine(x-r, y-r, x-r-15, y-r-10);
    }

}

void MainWindow::startThread()
{
    connect(this, SIGNAL(dataCame()), this, SLOT(dataRecieve()), Qt::QueuedConnection);
    pthread_create(&thread, 0, runCamera, this);
}

void MainWindow::dataRecieve()
{
    on_radioButton_clicked();
//    ui->monitor->setPixmap(QPixmap::fromImage(monitor));
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

void MainWindow::on_radioButton_clicked()
{
    if (ui->radioButton->isChecked())
        ui->monitor->setPixmap(QPixmap::fromImage(monitor));
    else
        ui->monitor->setPixmap(QPixmap::fromImage(edgeMonitor));
    ui->hist_monitor->setPixmap(QPixmap::fromImage(himg));
}

void MainWindow::on_radioButton_2_clicked()
{
    on_radioButton_clicked();
}

bool MainWindow::loadImage()
{
//    QImage qimg = QImage("../images/a-short.png").scaled(500, 600, Qt::KeepAspectRatio);
    QImage qimg = QImage("../images/b.jpg").scaled(500, 600, Qt::KeepAspectRatio);
    Image tempImage;
    image.resize(qimg.width(), qimg.height());
    tempImage.resize(qimg.width(), qimg.height());
    for (unsigned int y=0; y<(unsigned)qimg.height(); ++y)
        for (unsigned int x=0; x<(unsigned)qimg.width(); ++x)
        {
            Image::Pixel p;
            QColor c = QColor(qimg.pixel(x, y));

            p.y  = CRGB2Y(c.red(),  c.green(), c.blue());
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
