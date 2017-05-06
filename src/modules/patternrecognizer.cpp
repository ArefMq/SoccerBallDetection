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

Mat PatternRecognizer::getGrayROI(const Image& image, const Circle& ROI)
{
    const int startx = ROI._translation.x - ROI._radious;
    const int starty = ROI._translation.y - ROI._radious;
    const int r = ROI._radious * ROI._radious;

    Mat result(CV_WIN_SIZE, CV_WIN_SIZE, CV_8U);
    for (int hx=0; hx<CV_WIN_SIZE; ++hx)
        for (int hy=0; hy<CV_WIN_SIZE; ++hy)
        {
            const int x = startx + hx * ROI._radious * 2 / CV_WIN_SIZE;
            const int y = starty + hy * ROI._radious * 2 / CV_WIN_SIZE;

            if (x >= 0 && y >= 0 && x < (int)image.width() && y < (int)image.height() &&
                x*x+y*y < r*r)
                result.at<unsigned char>(hx, hy) = image.getPixel(x, y).y;
            else
                result.at<unsigned char>(hx, hy) = 0;
        }

    return result;
}

bool PatternRecognizer::predict(const Image& image, const Circle& ROI, Pattern )
{
    Mat grayROIImage = getGrayROI(image, ROI);

    static const Size trainingPadding = Size(0, 0);
    static const Size winStride = Size(8, 8);

    vector<float> sample;
    vector<Point> locations;
    HOGDescriptor hog;


    hog.winSize = Size(CV_WIN_SIZE, CV_WIN_SIZE);
    hog.compute(grayROIImage, sample, winStride, trainingPadding, locations);
    sample.insert(sample.begin(), 1);

    Mat sampleMat = Mat(sample);
    transpose(sampleMat,sampleMat);

    float r = boostTrainer->predict(sampleMat);
    return r > 0.5;
}
