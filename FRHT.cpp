/**
 * @file FRHT.cpp
 * A general purpose fast random hough transform module for detecting circles
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam</a>
 * @date Apr 2016
 */

#include "FRHT.h"
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include <iostream>

// [TODO] : make these configurable parameters
//#define FRHT_ITERATIONS 150
#define FRHT_ITERATIONS 150

FRHT::FRHT(EdgeImage& image) :
    _image(image)
{
    srand(time(0));
}

FRHT::~FRHT()
{
}

void FRHT::update(std::vector<Vector2D> preferredRandomPoints)
{
    _circles.clear();

    if (!_image.edgePoints().size())
        return;

    const int maxIterations = FRHT_ITERATIONS; // [FIXME] : This could be a function of the camera tilt
    int edgePointsLastIndex = _image.edgePoints().size();
    while (preferredRandomPoints.size() < maxIterations)
    {
        int randomID = rand() % edgePointsLastIndex;
        preferredRandomPoints.push_back(_image.edgePoints().at(randomID));
    }

    for (unsigned int i=0; i<preferredRandomPoints.size(); ++i)
    {
        Vector2D& point = preferredRandomPoints.at(i);
        int step = EdgeImage::edgeingStep(point.y-_image.originY) / 2;

        edgePointsLastIndex = _image.edgePoints().size();
        _image.refine(point);

        const int additionalPoints = _image.edgePoints().size() - edgePointsLastIndex;
        if (additionalPoints > 0) //-- If any new point added to _image.edgePoints
        {
            const int randomID = (rand() % additionalPoints) + edgePointsLastIndex;
            point = _image.edgePoints().at(randomID);
            step = EdgeImage::edgeingStep(point.y-_image.originY) / 2;
            _image.refine(point);
        }

        findCircle(point, step);

    }
}

void FRHT::findCircle(const Vector2D& centerPoint, int step)
{
    std::vector<SearchCell> searchPoints;

    // [FIXME] : there is a bug here, sometimes one point is pushed in some place with no edge in.


    for (int y=centerPoint.y-step; y<centerPoint.y+step; ++y)
        for (int x=centerPoint.x-step; x<centerPoint.x+step; ++x)
        {
            if (x < 0 || y < 0 || x > (int)_image.width() || y > (int)_image.height())
                continue;

            if (_image.getPixel(x, y).y < 127)
                continue;

            // [TODO] : make this one lookup table in order to speed up
            const int distance = sqrt((x-centerPoint.x)*(x-centerPoint.x) + (y-centerPoint.y)*(y-centerPoint.y));

            for (std::vector<SearchCell>::const_iterator sp = searchPoints.begin(); sp < searchPoints.end(); sp++)
                if (sp->distance == distance)
                {
                    checkCircle(centerPoint, sp->point, Vector2D(x, y));
                    //          return; // [FIXME] : check to see if it better to stop after first distance pair or not.
                }

            searchPoints.push_back(SearchCell(distance, Vector2D(x, y)));
            y+=2;
        }
}

void FRHT::checkCircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3)
{
    Vector3D circle = fitACircle(p1, p2, p3);
    if (isnan(circle.x))
        return;
    if (isnan(circle.y))
        return;
    if (isnan(circle.z))
        return;

    if (isinf(circle.x))
        return;
    if (isinf(circle.y))
        return;
    if (isinf(circle.z))
        return;

    _circles.push_back(circle);
}

const std::vector<Vector3D>& FRHT::extractedCircles() const
{
    return _circles;
}

Vector3D FRHT::fitACircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3)
{
    const Vector3D P1 = Vector3D(p1.x, p1.y, 0);
    const Vector3D P2 = Vector3D(p2.x, p2.y, 0);
    const Vector3D P3 = Vector3D(p3.x, p3.y, 0);

    const Vector3D t = P2-P1; // u
    const Vector3D u = P3-P1; // v
    const Vector3D v = P3-P2;

    //-- Cross Product of : t x u
    const Vector3D w = Vector3D(
                t.y*u.z - t.z*u.y,
                t.z*u.x - t.x*u.z,
                t.x*u.y - t.y*u.x
                );

    const float t2 = t.x*t.x + t.y*t.y + t.z*t.z;
    const float u2 = u.x*u.x + u.y*u.y + u.z*u.z;
    const float w2 = w.x*w.x + w.y*w.y + w.z*w.z;

    const Vector3D mid = u * (t2 * (u.x*v.x + u.y*v.y + u.z*v.z)) -
            t * (u2 * (t.x*v.x + t.y*v.y + t.z*v.z));

    const float cx = P1.x + (mid / (2*w2)).x;
    const float cy = P1.y + (mid / (2*w2)).y;
    const float r = 0.5f * sqrt( t2 * u2 * (v.x*v.x + v.y*v.y + v.z*v.z) / w2 );

    return Vector3D(cx, cy, r);
}


void FRHT::resizeCirlcle(Vector3D& circle, int maxBoundary, const ColorAnalyzer& colorAnalyzer, QPainter& qpn)
{
    int noise, itr;
    qpn.setPen(Qt::green);

    const int maxSearchSpace = 100; //-- px

    //-- Upper Search
    Vector2D p1(circle.x, circle.y-circle.z);
    noise = 0;
    itr = 0;
    for (; p1.y > 0 && itr < maxSearchSpace && p1.y > maxBoundary && noise < 3; p1.y--, itr++)
        if (colorAnalyzer.isGreen(p1.x, p1.y))
            noise++;
        else
        {
            qpn.drawPoint(p1.x, p1.y);
            noise=0;
        }
#ifdef USE_EDGE_TO_REFINE
    while (edgeImage[p1.y][p1.x].y < 64)
        p1.y++;
#else
    p1.y += noise;
#endif

    //-- Left Search
    Vector2D p2(circle.x-circle.z, circle.y);
    noise = 0;
    itr = 0;
    for (; p2.x > 0 && itr < maxSearchSpace && noise < 3; p2.x--, itr++)
        if (colorAnalyzer.isGreen(p2.x, p2.y))
            noise++;
        else
        {
            qpn.drawPoint(p2.x, p2.y);
            noise=0;
        }
#ifdef USE_EDGE_TO_REFINE
    while (edgeImage[p2.y][p2.x].y < 64)
        p2.x++;
#else
    p2.x += noise;
#endif

    //-- Right Search
    Vector2D p3(circle.x+circle.z, circle.y);
    noise = 0;
    itr = 0;
    for (; p3.x < _image.width() && itr < maxSearchSpace && noise < 3; p3.x++, itr++)
        if (colorAnalyzer.isGreen(p3.x, p3.y))
            noise++;
        else
        {
            qpn.drawPoint(p3.x, p3.y);
            noise=0;
        }
#ifdef USE_EDGE_TO_REFINE
    while (edgeImage[p3.y][p3.x].y < 64)
        p3.x--;
#else
    p3.x -= noise;
#endif

    circle = FRHT::fitACircle(p1, p2, p3);
}
