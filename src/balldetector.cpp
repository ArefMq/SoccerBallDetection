#include "balldetector.h"
#include "modules/coloranalyzer.h"
#include "modules/edgeimage.h"
#include "modules/FRHT.h"
#include "modules/kinematicsprovider.h"

#include <iostream>
#include <sys/time.h>

using namespace MVision;

BallDetector::BallDetector() :
    _averageCycleTime(-1),
    colorAnalyzer(new ColorAnalyzer(_image)),
    edgeImage(new EdgeImage(_image)),
    houghTransform(new FRHT(*edgeImage)),
    kinematicsProvider(new KinematicsProvider())
{
}

BallDetector::~BallDetector()
{
    delete kinematicsProvider;
    delete houghTransform;
    delete edgeImage;
    delete colorAnalyzer;
}

void BallDetector::update(const Image& image)
{
    _image = image;
    _results.clear();

    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    update();

    gettimeofday(&tv2, NULL);

    double cycleTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0 + (double) (tv2.tv_sec - tv1.tv_sec);
    if (_averageCycleTime == -1)
        _averageCycleTime = cycleTime;
    else
        _averageCycleTime = _averageCycleTime * 0.95 + cycleTime * 0.5;

}

void BallDetector::update()
{
    colorAnalyzer->update();
    edgeImage->update();
    houghTransform->update(_previousPoints);
    _previousPoints.clear();

    const std::vector<Circle>& circles = houghTransform->extractedCircles();
    for (std::vector<Circle>::const_iterator itr = circles.begin(); itr < circles.end(); itr++)
    {
        Circle circle = *itr;
        houghTransform->resizeCirlcle(circle, colorAnalyzer->boundary(circle._translation.x) /*kinematicsProvider.getHorizon()*/, *colorAnalyzer);

        //-- First Step Size Check
        if (circle._radious < 2.5 || circle._radious > 80) // [TODO] : make this config : ball max size in px
            continue;

        //-- Check green percentage
        if (!checkWhitePercentage(circle._translation.x, circle._translation.y, circle._radious))
            continue;

        //-- Check ball pattern
        if (!checkBallTexture(circle))
            continue;

        //-- Deliver the results
        Ball ball(circle);
        _results.push_back(ball);
//        _previousPoints.push_back(ball.PositionInImage()._translation);
    }
}

bool BallDetector::checkWhitePercentage(int cx, int cy, int r)
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

int BallDetector::searchedColor(int x, int y, int& color, int& nonGreen)
{
  if (x > -1 && x < (int)_image.width() && y > -1 && y < (int)_image.height())
  {
    nonGreen += colorAnalyzer->notGreen(x, y);
    color += colorAnalyzer->isWhite(x, y);
    return 1;
  }
  return 0;
}


const std::vector<Ball>& BallDetector::getResults() const
{
    return _results;
}

EdgeImage BallDetector::debug_GetEdgeImage()
{
    return *edgeImage;
}

double BallDetector::averageCycleTime() const
{
    return _averageCycleTime;
}

bool BallDetector::checkBallTexture(const Circle& ball)
{
    // [TODO] : implement this function
    return true;
}
