/**
 * @file edgeimage.h
 * A general purpose module for edge detection
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date Apr 2016
 */

#pragma once

#include <vector>
#include "image.h"
#include "vector2D.h"

// [FIXME] : make these parameters
#define expStep__  0.1
#define expCStep__ 0.88
#define horizonOffset__ 110

class EdgeImage : public Image
{
public:
    /**
     * @param image: constant reference of the original image
     */
    EdgeImage(const Image& image);
    ~EdgeImage();

    /**
     * @brief main update process; Calculate the stepwise edge points.
     */
    void update();
    /**
     * @return the set of the stepwise edge points
     */
    const std::vector<Vector2D>& edgePoints() const { return _edgePoints; }
    /**
     * @brief refine the precise edge according to the given point, with respect
     * to the edging step in relevance of that point.
     * @param point: the point to calculate the precise edge around
     */
    void refine(const Vector2D& point);
    /**
     * @brief refine the precise edge in the given rectangle.
     * @param topLeft, bottomRight: coordination of the given rectangle.
     */
    void refine(const Vector2D& topLeft, const Vector2D& bottomRight);
    /**
     * @brief calculate the edging step the given height.
     * @param y: pixel height with respect to the field boundary.
     * @return the step should be taken in the given height
     */
    static inline float edgeingStep(float y) { return y*expStep+expCStep; }

    int originY; // [FIXME] : move this somewhere else
    static float expStep;
    static float expCStep;
    static float horizonOffset;

private:
    const Image& _image; /**< The input image */
    std::vector<std::vector<Vector2D> > _scanGraphLookup; /**< the Look-Up Table for edging point (without respecting the horizon) */
    std::vector<Vector2D> _edgePoints; /**< the stepwise edge point */ //[TODO] : make this a raw array to speedup

    static Pixel black; /**< black pixel color; used for debugging purpose. */
    static Pixel red; /**< red pixel color; used for debugging purpose. */

    /**
     * @brief calculate and fill up the Look-Up Table for edging points
     */
    void createLookup();
    /**
     * @brief convolve the Sobel filter to the given pixel matrix.
     * @param topLeft: used to show the height of the top row of the pixel matrix
     * and the x of the left column of it.
     * @param middle: used to show the middle row and column of the pixel matrix.
     * @param bottomRight: used to show the height of the bottom row and the x of
     * the left column of the pixel matrix.
     * @param thresh: the threshold of for the grayscale edge image to use as
     * edge pixel. (between 0-255)
     * @return white if the pixel is edge and black otherwise.
     */
    Pixel calculateEdge(const Vector2D& topLeft, const Vector2D& middle, const Vector2D& bottomRight, int thresh=60);

    inline void getSafePixel(int x, int y, Pixel& getValue)
    {
        if (x < (int)_width && x > -1 && y < (int)_height && y > -1)
        {
            getValue = getPixel(x, y);
        }
    }

    inline void setSafePixel(int x, int y, const Pixel& setValue)
    {
        if (x < (int)_width && x > -1 && y < (int)_height && y > -1)
        {
            getPixel(x, y) = setValue;
        }
    }

    inline void getSafeOrginalPixel(int x, int y, Pixel& getValue)
    {
        if (x < (int)_width && x > -1 && y < (int)_height && y > -1)
        {
            getValue = _image.getPixel(x, y);
        }
    }

    inline float stepTable_X(int row, int col)
    {
#ifdef DEBUG
        if (row < 0 || col < 0 || row >= (int)_scanGraphLookup.size() || col >= (int)_scanGraphLookup.at(row).size())
            std::cerr << "invalid indexing..." << __FILE__ << " :: " << __LINE__ << "\n(" << row << ", " << col << ")\n";
#endif
        return _scanGraphLookup.at(row).at(col).x;
    }

    inline float stepTable_Y(int row, int col)
    {
#ifdef DEBUG
        if (row < 0 || col < 0 || row >= (int)_scanGraphLookup.size() || col >= (int)_scanGraphLookup.at(row).size())
            std::cerr << "invalid indexing..." << __FILE__ << " :: " << __LINE__ << "\n(" << row << ", " << col << ")\n";
#endif
        return (_scanGraphLookup.at(row).at(col).y+originY);
    }
};
