/**
 * @file FRHT.cpp
 * A general purpose fast random hough transform module for detecting circles
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date Apr 2016
 */

#include "FRHT.h"
#include "vector3D.h"
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace MVision;

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

//    int _i = 0;
    while (preferredRandomPoints.size() < maxIterations)
    {
//        std::cout << "inwiles " << _i++ << "(" << preferredRandomPoints.size() << ")\n";
        const int randomID = rand() % edgePointsLastIndex;
        const Vector2D& p = _image.edgePoints().at(randomID);

        preferredRandomPoints.push_back(p);
    }
//    std::cout << "and finally -----> " << preferredRandomPoints.size() << "\n";

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
            if (x < 0 || y < 0 || x >= (int)_image.width() || y >= (int)_image.height())
                continue;

            if (_image.getPixel(x, y).y < 127)
                continue;

            // [TODO] : make this one lookup table in order to speed up
            const int distance = sqrt((x-centerPoint.x)*(x-centerPoint.x) + (y-centerPoint.y)*(y-centerPoint.y));

            for (std::vector<SearchCell>::const_iterator sp = searchPoints.begin(); sp < searchPoints.end(); sp++)
                if (sp->distance == distance)
                {
                    checkCircle(centerPoint, sp->point, Vector2D(x, y));
//                    return; // [FIXME] : check to see if it better to stop after first distance pair or not.
                }

            searchPoints.push_back(SearchCell(distance, Vector2D(x, y)));
            y+=2;
        }
}

void FRHT::checkCircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3)
{
    Circle circle = fitACircle(p1, p2, p3);
    if (std::isnan(circle._translation.x))
        return;
    if (std::isnan(circle._translation.y))
        return;
    if (std::isnan(circle._radious))
        return;

    if (std::isinf(circle._translation.x))
        return;
    if (std::isinf(circle._translation.y))
        return;
    if (std::isinf(circle._radious))
        return;

    _circles.push_back(circle);
}

const std::vector<Circle>& FRHT::extractedCircles() const
{
    return _circles;
}

Circle FRHT::fitACircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3)
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

    return Circle(Vector2D(cx, cy), r);
}


void FRHT::resizeCirlcle(Circle &circle, int maxBoundary, const ColorAnalyzer& colorAnalyzer)
{
    int noise, itr;
    const int maxSearchSpace = 100; //-- px
    std::vector<Vector2D> searchPoints;

//-- Some dirty hacks to avoid complex functions, I know it's still pretty complex...
#define IN_IMAGE(p) (p.x >= 0 && p.x < _image.width() && p.y >= 0 && p.y < _image.height() && p.y > maxBoundary)

#ifdef USE_EDGE_TO_REFINE
#define AFTER_SEARCH(invf) while (edgeImage[p.y][p.x].y < 64) p.y++; [FIXME]
#else
#define AFTER_SEARCH(invf) invf;
#endif

#define SEARCH(ofx, ofy, cfunc, invf)                                           \
    if (searchPoints.size() < 3) {                                              \
    Vector2D p(circle._translation.x + ofx, circle._translation.y + ofy);       \
    noise = 0;                                                                  \
    itr = 0;                                                                    \
    for (; IN_IMAGE(p) && itr < maxSearchSpace && noise < 3; itr++) {           \
        if (colorAnalyzer.isGreen(p.x, p.y))                                    \
            noise++;                                                            \
        else                                                                    \
            noise=0;                                                            \
        cfunc;                                                                  \
    }                                                                           \
    AFTER_SEARCH(invf)                                                          \
    if IN_IMAGE(p)                                                              \
        searchPoints.push_back(p); }
//-- End of dirty hack ---------------------------------------------------------------

    const float r = circle._radious;
    SEARCH(0, -r, p.y--, p.y += noise); //-- Search up
    SEARCH(+r, 0, p.x++, p.x -= noise); //-- Search right
    SEARCH(-r, 0, p.x--, p.x += noise); //-- Search left

    SEARCH(+r, 0, {p.y--; p.x++;}, {p.y += noise; p.x -= noise;}); //-- Search up-right
    SEARCH(+r, 0, {p.y--; p.x--;}, {p.y += noise; p.x += noise;}); //-- Search up-left
    SEARCH(+r, 0, {p.y++; p.x++;}, {p.y -= noise; p.x -= noise;}); //-- Search bottom-right
    SEARCH(+r, 0, {p.y++; p.x--;}, {p.y -= noise; p.x += noise;}); //-- Search bottom-left

    SEARCH(+r, 0, p.y++, p.y -= noise); //-- Search bottom


    if (searchPoints.size() > 2)
        circle = FRHT::fitACircle(searchPoints.at(0), searchPoints.at(1), searchPoints.at(2));
}
