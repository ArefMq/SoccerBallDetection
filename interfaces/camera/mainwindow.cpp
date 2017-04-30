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


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ballDetector(BallDetector()),
    imageLuck(false),
    isRunning(false)
{
    ui->setupUi(this);
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
    Webcam webcam("/dev/video0", XRES, YRES);
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
        QPainter* qpn = new QPainter(&ptr->monitor);

        ptr->ballDetector.update(ptr->image);
        ptr->edgeMonitor = ptr->image2qimage(ptr->ballDetector.debug_GetEdgeImage());
        //    for (int i=0; i<ballDetector.edgeImage->_scanGraphLookup.size(); ++i)
        //        for (int j=0; j<ballDetector.edgeImage->_scanGraphLookup.at(i).size(); ++j)
        //            edgeMonitor.setPixel(ballDetector.edgeImage->_scanGraphLookup.at(i).at(j).x,
        //                                 ballDetector.edgeImage->_scanGraphLookup.at(i).at(j).y,
        //                                 QColor(Qt::red).rgb());

        typedef std::vector<Ball> Balls;
        const Balls& results = ptr->ballDetector.getResults();
        for (Balls::const_iterator itr=results.begin(); itr<results.end(); itr++)
        {
            const Ball& b = *itr;
            qpn->setPen(QPen(Qt::red, 3));
            qpn->drawEllipse(QPointF(b.PositionInImage()._translation.x, b.PositionInImage()._translation.y), b.PositionInImage()._radious, b.PositionInImage()._radious);
        }

        delete qpn;
        emit ptr->dataCame();
    }

    return 0;
}

void MainWindow::startThread()
{
    connect(this, SIGNAL(dataCame()), this, SLOT(dataRecieve()), Qt::QueuedConnection);//BlockingQueuedConnection);
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
