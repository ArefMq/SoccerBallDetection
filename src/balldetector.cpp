#include "balldetector.h"
#include "modules/coloranalyzer.h"
#include "modules/EdgeImage.h"
#include "modules/FRHT.h"
#include "modules/kinematicsprovider.h"

BallDetector::BallDetector() :
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

#include <iostream>

void BallDetector::update(const Image& image)
{
    _image = image;
    _results.clear();

    colorAnalyzer->update();
    edgeImage->update();
    houghTransform->update(_previousPoints);
    _previousPoints.clear();

    const std::vector<Circle>& circles = houghTransform->extractedCircles();
    for (std::vector<Circle>::const_iterator itr = circles.begin(); itr < circles.end(); itr++)
    {
        Circle circle = *itr;

        //-- Deliver the results
        Ball ball(circle);
        _results.push_back(ball);
//        _previousPoints.push_back(ball.PositionInImage()._translation);
    }

    std::cout << "c-size :: " << circles.size() << "\n";
}

const std::vector<Ball>& BallDetector::getResults() const
{
    return _results;
}

EdgeImage BallDetector::debug_GetEdgeImage() { return *edgeImage; }
