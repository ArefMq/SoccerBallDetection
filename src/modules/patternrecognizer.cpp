/**
 * @file patternrecognizer.cpp
 * Ball Pattern Recognizer module
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date May 2016
 */

#include "patternrecognizer.h"
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace cv;
using namespace MVision;

PatternRecognizer::PatternRecognizer() :
    boostTrainer(new CvBoost())
{
}

PatternRecognizer::~PatternRecognizer()
{
    delete boostTrainer;
}

void PatternRecognizer::load()
{
    boostTrainer->load("/home/aref/workspace/humanoid/SoccerBallDetection/config/boostModel.xml","boost");
}

#define CV_WIN_SIZE 128
#define pl std::cout << __FILE__ << "  ::  " << __LINE__ << "\n";

Mat PatternRecognizer::getGrayROI(const Image& image, const Circle& ROI)
{
    const int r = ROI._radious + 5;
    const int startx = ROI._translation.x - r;
    const int starty = ROI._translation.y - r;

    Mat result(CV_WIN_SIZE, CV_WIN_SIZE, CV_8U);
    for (int hx=0; hx<CV_WIN_SIZE; ++hx)
        for (int hy=0; hy<CV_WIN_SIZE; ++hy)
        {
            const int x  = startx + hx * r * 2 / CV_WIN_SIZE;
            const int y  = starty + hy * r * 2 / CV_WIN_SIZE;

            if (x >= 0 && y >= 0 && x < (int)image.width() && y < (int)image.height() &&
                (hx-64)*(hx-64)+(hy-64)*(hy-64) < 4096)
                result.at<unsigned char>(Point(hx, hy)) = image.getPixel(x, y).y;
            else
                result.at<unsigned char>(Point(hx, hy)) = 0;
        }

    return result;
}

#include <qstring.h>
#include <qimage.h>
#include <qcolor.h>

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define sampleFilePath   "/home/aref/workspace/humanoid/AdaBoost/HogClassifier/samples/b_small.jpg"
#define sampleFileFolder "/home/aref/workspace/humanoid/AdaBoost/HogClassifier/samples/"

bool PatternRecognizer::predict(const Image& image, const Circle& ROI, Pattern )
{
//    Circle ROI = Circle(Vector2D(202, 215), 35);
    Mat grayROIImage = getGrayROI(image, ROI);

//
//    QImage orginal_image(sampleFilePath);
//    cout << orginal_image.width() << ", ";
//    const int SC = 0.5;
//    orginal_image = orginal_image.scaled(64, 64);
//    cout << orginal_image.width() << "\n";
//
//    Mat grayROIImage(CV_WIN_SIZE, CV_WIN_SIZE, CV_8U);
//    for (int hx=0; hx<CV_WIN_SIZE; ++hx)
//        for (int hy=0; hy<CV_WIN_SIZE; ++hy)
//        {
//            const int x  = hx * orginal_image.width() / CV_WIN_SIZE;
//            const int y  = hy * orginal_image.height() / CV_WIN_SIZE;
//
//            if (x >= 0 && y >= 0 && x < (int)orginal_image.width() && y < (int)orginal_image.height())
//            {
//                const QColor& c = QColor(orginal_image.pixel(x, y));
//                unsigned char v = RGB2Y(c.red(), c.green(), c.blue());
//                grayROIImage.at<unsigned char>(Point(hx, hy)) = v;
//            }
//            else
//                grayROIImage.at<unsigned char>(Point(hx, hy)) = 0;
//        }

    //-- constants
    static const Size trainingPadding = Size(0, 0);
    static const Size winStride = Size(8, 8);

    //-- variables
    vector<float> sample;
    vector<Point> locations;
    HOGDescriptor hog;

    //-- real shit
    hog.winSize = Size(CV_WIN_SIZE, CV_WIN_SIZE);
    hog.compute(grayROIImage, sample, winStride, trainingPadding, locations);
    sample.insert(sample.begin(), 1);

    Mat sampleMat = Mat(sample);
    transpose(sampleMat,sampleMat);

    //-- results
    const float r = boostTrainer->predict(sampleMat);
    const bool result = r == 1;


//    Mat grayROIImage = getGrayROI(image, ROI);
//
//    static const Size trainingPadding = Size(0, 0);
//    static const Size winStride = Size(8, 8);
//
//    vector<float> sample;
//    vector<Point> locations;
//    HOGDescriptor hog;
//
//
//    hog.winSize = Size(CV_WIN_SIZE, CV_WIN_SIZE);
//    hog.compute(grayROIImage, sample, winStride, trainingPadding, locations);
//    sample.insert(sample.begin(), 1);
//
//    Mat sampleMat = Mat(sample);
//    transpose(sampleMat,sampleMat);
//
//    float r = boostTrainer->predict(sampleMat);
//
//    static int i=0;
//    const char* title = (QString::number(i++) + ") " + (result?"ball":"not-ball")).toStdString().c_str();
//    imshow(title, grayROIImage);
//    imwrite((QString(sampleFileFolder) + "res_" + QString::number(i++) + ".jpg").toStdString().c_str(), grayROIImage);

    return result;
}
