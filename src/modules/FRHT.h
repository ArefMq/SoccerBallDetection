/**
 * @file FRHT.h
 * A general purpose fast random hough transform module for detecting circles
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date Apr 2016
 */

#pragma once

#include "EdgeImage.h"
#include "circle.h"
#include "coloranalyzer.h"

class FRHT
{
public:
    /**
     * @param image: reference of the input edge image
     */
    FRHT(EdgeImage& image);
    ~FRHT();

    /**
     * @brief main update function. This function first select random edge points
     * (in addition to previously given points) and refine the edge image near
     * the points. And then apply the Fast Random Hough Transform (FRHT) algorithm
     * to check if the given point belongs to a circle or not.
     * @param preferredRandomPoints: preferred given points to add to the random
     * set (assumed to be the points marked as circle in previous cycle)
     */
    void update(std::vector<Vector2D> preferredRandomPoints = std::vector<Vector2D>());
    /**
     * @return the circles in the image
     */
    const std::vector<Circle>& extractedCircles() const;
    /**
     * @brief resize the given circle by searching left, right, and top of the
     * given circle to find the green field (and also some precise edges). and
     * then re-factor the circle to the new parameters.
     * @param circle: reference of the circle to be refined. (the function
     * deliver via pass by reference method.)
     * @param maxBoundary: the field boundary in the circle center
     * @param colorAnalyzer: the color analyzer module
     */
    void resizeCirlcle(Circle& circle, int maxBoundary, const ColorAnalyzer& colorAnalyzer); //[TODO] : remove this parameter and use directly from color analyzer

private:
    EdgeImage& _image; /**< Reference of the input image */
    std::vector<Circle> _circles; /**< Detected circle */

    /**
     * @brief apply FRTH algorithm on the center point. The nearby edges of the
     * given is checked to see they belong to a circle or not. And if the check
     * is successful, then the result is pushed to `_circles' vector.
     * @param centerPoint: initial point to check
     * @param step: Steps for searching in that area (relative to the edge resolution).
     */
    void findCircle(const Vector2D& centerPoint, int step);
    /**
     * @brief if the given points forms a valid circle, push the circle in the
     * `_circle' vector.
     * @param p1, p2, p3: given points to check.
     */
    void checkCircle(const Vector2D &p1, const Vector2D &p2, const Vector2D &p3);


    /**
     * @brief This class is used in FRHT searching mechanism in order to find
     * the two points with similar distance relative to initial point.
     */
    class SearchCell {
    public:
        SearchCell(int Distance, const Vector2D& Point) : distance(Distance), point(Point) {}
        int      distance;
        Vector2D point;
    };


public:
    /**
     * @brief fit a circle to the given points.
     * @param p1, p2, p3: points to form a circle
     * @return the circle passed the three given points
     */
    static Circle fitACircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3);
};
