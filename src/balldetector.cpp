#include "balldetector.h"
#include "modules/coloranalyzer.h"
#include "modules/edgeimage.h"
#include "modules/FRHT.h"
#include "modules/kinematicsprovider.h"
#include "modules/patternrecognizer.h"

#include <iostream>
#include <math.h>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#define STOP_WATCH_TICK SYSTEMTIME tv1, tv2; GetLocalTime(&tv1);
#define STOP_WATCH_TOCK GetLocalTime(&tv2); double cycleTime = ((double) (tv2.wMilliseconds - tv1.wMilliseconds) / 1000.0 + (double) (tv2.wSecond - tv1.wSecond));
#else
#include <sys/time.h>
#define STOP_WATCH_TICK struct timeval  tv1, tv2; gettimeofday(&tv1, NULL);
#define STOP_WATCH_TOCK gettimeofday(&tv2, NULL); double cycleTime = ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0 + (double) (tv2.tv_sec - tv1.tv_sec));
#endif

using namespace MVision;

BallDetector::BallDetector(DebugHelperInterface *debugHelper) :
    _averageCycleTime(-1),
    colorAnalyzer(new ColorAnalyzer(_image)),
    edgeImage(new EdgeImage(_image)),
    houghTransform(new FRHT(*edgeImage)),
    kinematicsProvider(new KinematicsProvider()),
    patternRecognizer(new PatternRecognizer())
{
    patternRecognizer->load();
    setDebugHelper(debugHelper);
}

BallDetector::~BallDetector()
{
    delete patternRecognizer;
    delete kinematicsProvider;
    delete houghTransform;
    delete edgeImage;
    delete colorAnalyzer;
}

void BallDetector::update(const Image& image)
{
    _image = image;
    _results.clear();

	STOP_WATCH_TICK;
    update();
	STOP_WATCH_TOCK;

    if (_averageCycleTime == -1)
        _averageCycleTime = cycleTime;
    else
        _averageCycleTime = _averageCycleTime * 0.95 + cycleTime * 0.5;
}

void BallDetector::update()
{
    colorAnalyzer->update();
    edgeImage->fieldBoundaries = colorAnalyzer->_boundaryPoints;
    edgeImage->update();
    houghTransform->update(_previousPoints);
//    _previousPoints.clear();
    while (_previousPoints.size() > 5)
        _previousPoints.erase(_previousPoints.begin());

    const std::vector<Circle>& circles = houghTransform->extractedCircles();
    std::vector<Circle> extCircles;
    int maxScore=0; Circle bestCircle;
    for (std::vector<Circle>::const_iterator itr = circles.begin(); itr < circles.end(); itr++)
    {
        Circle circle = *itr;
        houghTransform->resizeCirlcle(circle, colorAnalyzer->boundary(circle.translation().x()) /*kinematicsProvider.getHorizon()*/, *colorAnalyzer);
        houghTransform->resizeCirlcle(circle, colorAnalyzer->boundary(circle.translation().x()) /*kinematicsProvider.getHorizon()*/, *colorAnalyzer);

        //-- First Step Size Check
        if (circle.radious() < 2.5 || circle.radious() > 80) // [TODO] : make this config : ball max size in px
            continue;

        //-- Check green percentage
        if (!checkWhitePercentage(circle.translation().x(), circle.translation().y(), circle.radious()))
            continue;

        //-- Check ball pattern
        if (!checkBallTexture(circle))
            continue;

#ifdef SINGLE_BALL
        //-- Merging the results
        int scoreCount=0; Circle bestCircleCandidate = circle;
        for (unsigned int i=0; i<extCircles.size(); ++i)
        {
            if (std::abs(extCircles.at(i).translation().x() - circle.translation().x()) < 5 &&
                std::abs(extCircles.at(i).translation().y() - circle.translation().y()) < 5)
            {
                bestCircleCandidate.mutable_translation().mutable_x() += extCircles.at(i).translation().x();
                bestCircleCandidate.mutable_translation().mutable_y() += extCircles.at(i).translation().y();
                bestCircleCandidate.mutable_radious() += extCircles.at(i).radious();
                scoreCount++;
            }
        }

        if (scoreCount > maxScore)
        {
            //-- Apply average filter
            bestCircleCandidate.mutable_translation().mutable_x() /= scoreCount+1;
            bestCircleCandidate.mutable_translation().mutable_y() /= scoreCount+1;
            bestCircleCandidate.mutable_radious() /= scoreCount+1;
            maxScore = scoreCount;
            bestCircle = bestCircleCandidate;

            houghTransform->resizeCirlcle(bestCircle, colorAnalyzer->boundary(bestCircle.translation().x()) /*kinematicsProvider.getHorizon()*/, *colorAnalyzer);
        }

        extCircles.push_back(circle);
    }

    //-- Deliver the results
    Ball ball(bestCircle);
    _results.push_back(ball);
    _previousPoints.push_back(ball.PositionInImage().translation());

//    checkBallTexture(bestCircle);
#else

        //-- Deliver the results
        Ball ball(circle);
        _results.push_back(ball);
        _previousPoints.push_back(ball.PositionInImage().translation());
    }
#endif
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

double BallDetector::averageCycleTime() const
{
    return _averageCycleTime;
}

bool BallDetector::checkBallTexture(const Circle& ball)
{
    return patternRecognizer->predict(_image, ball);
}
