/**
 * @file coloranalyzer.h
 * This module analyze the image in order to find the field green color
 * and calculate the field boundary.
 *
 * @author <a href="mailto:aref.moqadam@gmail.com">Aref Moqadam Mehr</a>
 * @date March 2017
 */

#ifndef COLORANALYZER_H
#define COLORANALYZER_H

#include "image.h"
#include <vector>
#include <vector2D.h>

namespace MVision {

class ColorAnalyzer
{
public:
    /**
     * @brief Constructor function.
     * @param InputImage: reference of the input image used in further calculations
     */
    ColorAnalyzer(const Image& InputImage);

    //-- Actuator Functions
    /**
     * @brief Update the color analyzer module by running the histogram
     * analyzer and field boundary detector.
     */
    void update();

    //-- Interface Functions
    /**
     * @brief get the pixel of given coordinate from input image and then compare
     * the value with the green peak to see if it belongs to the field green set
     * or not.
     * @param x, y: given coordinate
     * @return true if the color is field-green
     */
    bool isGreen(int x, int y) const;
    /**
     * @brief get the pixel of given coordinate from input image and then compare
     * the value with the green peak to see if it does not belongs to the field
     * green set or not.
     * @param x, y: given coordinate
     * @return false if the color is field-green
     */
    bool notGreen(int x, int y) const;
    /**
     * @brief get the pixel of given coordinate from input image and then compare
     * it to the static threshold.
     * @param x, y: given coordinate
     * @return true if the color is field-green
     */
    bool isWhite(int x, int y) const;
    /**
     * @brief get the pixel of given coordinate from input image and then compare
     * it to the static threshold.
     * @param x, y: given coordinate
     * @return false if the color is field-green
     */
    bool notWhite(int x, int y) const;
    /**
     * @brief gives the height of the field boundary in the given x
     * @param x: position of the column to check
     * @return the position of the field boundary in the given column
     */
    inline int boundary(int x) const { return _boundaryPoints.at(x < 0 ? 0 : (x >= (int)_inputImage.width() - 1 ? (int)_inputImage.width() - 1 : x)); }

private:
    const Image& _inputImage; /**< Reference of the input image used for processing */
    int _greenPeak; /**< The value of Chroma (Cr) channel of the field-green color; peak of Cr histogram. Always between [0-255] */
public: // [FIXME] : remove this line
    std::vector<int> _boundaryPoints; /**< Set of field boundary highest point for each column of the image. (Vector size = Image width) */
private: // [FIXME] : remove this line
    /**
     * @brief Calculate the image histogram. Then select the Cr peak (peaks less
     * than 127) as the field-green color reference.
     */
    void histogramAnalysis();

    /**
     * @brief Find the highest point of field then use Andrew's Monotone Algorithm
     * to calculate the convex hull of the field boundary point.
     */
    void fieldBoundaryDetection();

    /**
     * @brief check if three points are clockwise arranges. In the other words
     * if the convex of these points are downward.
     * @param p1, p2, p3: given points
     * @return true if the given points are clockwise.
     */
    inline double ccw(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3) { return (p2.x() - p1.x())*(p3.y() - p1.y()) - (p2.y() - p1.y())*(p3.x() - p1.x()); }

#ifdef DEBUG
public:
    int debug_histogram[256];
    int debug_peak;
    const std::vector<int>& debug_getBoundaryPoints() const { return _boundaryPoints; }
    std::vector<Vector2D> debug_highestPoints;
#endif
};
} // End namespace MVision
#endif // COLORANALYZER_H
