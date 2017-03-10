/**
 * @file EdgeImage.cpp
 * A general purpose module for edge detection
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam</a>
 * @date Apr 2016
 */

#include "EdgeImage.h"
#include <iostream>
#include <cmath>

#define pl //std::cout << __FILE__ << " :: " << __LINE__ << std::endl;

Image::Pixel EdgeImage::black;
Image::Pixel EdgeImage::red;
float EdgeImage::expStep;
float EdgeImage::expCStep;
float EdgeImage::horizonOffset;

EdgeImage::EdgeImage(const Image& image) :
    originY(0),
    _image(image)
{
    pl;
    black.cr = black.cb = 127; black.y = 0;
    red.cr = 250; red.cb = 0; red.y = 127;
}

EdgeImage::~EdgeImage()
{
}

void EdgeImage::createLookup()
{
    _scanGraphLookup.clear();
    std::cout << "creating edge lookup table..." << std::endl;
    resize(_image.width(), _image.height());

    for (float y=0; y<_height*3.f; y+=edgeingStep(y))
    {
        std::vector<Vector2D> scanRow;
        for (float x=_width/2.f; x<(float)_width; x+=edgeingStep(y))
            scanRow.push_back(Vector2D(x, y+horizonOffset));
        for (float x=_width/2.f; x>=0; x-=edgeingStep(y))
            scanRow.push_back(Vector2D(x, y+horizonOffset));
        _scanGraphLookup.push_back(scanRow);
    }

//    for (int y=0; y<(int)_height; y++)
//    {
//        std::vector<Vector2D> scanRow;
//        for (int x=0; x<(int)_width; x++)
//            scanRow.push_back(Vector2D(x, y));
//        _scanGraphLookup.push_back(scanRow);
//    }


    static int i=0;
    if (i++ > 10)
        std::cerr << "[It looks this message is keep popping out!]\n[it might be because the difference of the upper and lower camera resolution.]\n\n";
}

void EdgeImage::refine(const Vector2D& topLeft, const Vector2D& bottomRight)
{
    for (int y=topLeft.y; y<bottomRight.y; ++y)
        for (int x=topLeft.x; x<bottomRight.x; ++x)
        {
            Vector2D middle = Vector2D(x,y);
            Vector2D topLeftV = Vector2D(x-1, y-1);
            Vector2D bottomRightV = Vector2D(x+1, y+1);

            Pixel edgePixel = black;

            edgePixel = calculateEdge(topLeftV, middle, bottomRightV);
            setSafePixel(middle.x, middle.y, (edgePixel.y>100)?red:black);

            if (edgePixel.y < 128)
            {
                topLeftV = Vector2D(x-2, y-2);
                bottomRightV = Vector2D(x+2, y+2);

                edgePixel = calculateEdge(topLeftV, middle, bottomRightV, 120);
                setSafePixel(middle.x, middle.y, (edgePixel.y>140)?red:black);
            }
        }

}

void EdgeImage::refine(const Vector2D& point)
{
    // [FIXME] : I'm not sure about the step, revise the way of step calculation
    const int step = edgeingStep(point.y-originY);// + edgeingStep(point.y));

    const int startX = (point.x-step)>1 ? (point.x-step) : 1;
    const int startY = (point.y-step)>1 ? (point.y-step) : 1;

    const int endX = (point.x+step)<(_width-1)  ? (point.x+step) : (_width-1);
    const int endY = (point.y+step)<(_height-1) ? (point.y+step) : (_height-1);


    for (int y=startY; y<endY; ++y)
        for (int x=startX; x<endX; ++x)
        {
            if (x == point.x && y == point.y)
                continue;

            Vector2D middle = Vector2D(x,y);

            if (middle.x < (int)_width && middle.x > -1 && middle.y < (int)_height && middle.y > -1 && getPixel(middle.x, middle.y).y != 0)
                continue;

            Vector2D topLeft = Vector2D(x-1, y-1);
            Vector2D bottomRight = Vector2D(x+1, y+1);

            Pixel edgePixel = black;
            edgePixel = calculateEdge(topLeft, middle, bottomRight);
            setSafePixel(middle.x, middle.y, (edgePixel.y>127)?red:black);
        }
}

void EdgeImage::update()
{
    pl;
    _edgePoints.clear();

    if (_width != _image.width() || height() != _image.height())
        createLookup();
    pl;
    for (unsigned int y=0; y<_height; ++y)
        for (unsigned int x=0; x<_width; ++x)
            getPixel(x, y) = black; //_image[y*avStep][x*avStep];
    pl;
    for (unsigned row=0; row<_scanGraphLookup.size(); ++row)
        for (unsigned col=0; col<_scanGraphLookup.at(row).size(); ++col)
        {
            Vector2D middle = Vector2D(
                        stepTable_X(row, col),
                        stepTable_Y(row, col)
                        );

            Vector2D topLeft = Vector2D(
                        (col>0)?stepTable_X(row,col-1):stepTable_X(row,col)-1,
                        (row>0)?stepTable_Y(row-1,0):stepTable_Y(row,col)-1
                        );

            Vector2D bottomRight = Vector2D(
                        (col<_scanGraphLookup.at(row).size()-1)?stepTable_X(row,col+1):stepTable_X(row,col)+1,
                        (row<_scanGraphLookup.size()-1)?stepTable_Y(row+1,0):stepTable_Y(row,col)+1
                        );

            if (bottomRight.x >= _width || bottomRight.y >= _height)
                break;
            if (topLeft.x < 0 || topLeft.y < 0)
                continue;

            Pixel edgePixel = black;
            edgePixel = calculateEdge(topLeft, middle, bottomRight, 100);
            setSafePixel(middle.x, middle.y, edgePixel);
        }

    pl
}

Image::Pixel EdgeImage::calculateEdge(const Vector2D& topLeft, const Vector2D& middle, const Vector2D& bottomRight, int thresh)
{
pl;

if (topLeft.x < 0 || topLeft.x >= _width || topLeft.y < 0 || topLeft.y >= _height)
    std::cout << "invalid injection => top left...     (" << topLeft.x << ", " << topLeft.y << ") - (" << middle.x << ", " << middle.y << ") - (" << bottomRight.x << ", " << bottomRight.y << ")" << std::endl;
if (middle.x < 0 || middle.x >= _width || middle.y < 0 || middle.y >= _height)
    std::cout << "invalid injection => middle...       (" << topLeft.x << ", " << topLeft.y << ") - (" << middle.x << ", " << middle.y << ") - (" << bottomRight.x << ", " << bottomRight.y << ")" << std::endl;
if (bottomRight.x < 0 || bottomRight.x >= _width || bottomRight.y < 0 || bottomRight.y >= _height)
    std::cout << "invalid injection => bottom right... (" << topLeft.x << ", " << topLeft.y << ") - (" << middle.x << ", " << middle.y << ") - (" << bottomRight.x << ", " << bottomRight.y << ")" << std::endl;

    // [TODO] : implement a better filter

    //-- Implementation of Sobel Filter
    //   This is Vertical Sobel Filter Parameters:
    //   [ -1  -2  -1 ]
    //   [  0   0   0 ]
    //   [ +1  +2  +1 ]
    //   And it is the same for horizontal except with a counter clockwise flip

    Pixel a0; getSafeOrginalPixel(topLeft.x,     topLeft.y, a0);
    Pixel a1; getSafeOrginalPixel(middle.x,      topLeft.y, a1);
    Pixel a2; getSafeOrginalPixel(bottomRight.x, topLeft.y, a2);

    Pixel a3; getSafeOrginalPixel(topLeft.x,     middle.y, a3);
    Pixel a4; getSafeOrginalPixel(middle.x,      middle.y, a4);
    Pixel a5; getSafeOrginalPixel(bottomRight.x, middle.y, a5);

    Pixel a6; getSafeOrginalPixel(topLeft.x,     bottomRight.y, a6);
    Pixel a7; getSafeOrginalPixel(middle.x,      bottomRight.y, a7);
    Pixel a8; getSafeOrginalPixel(bottomRight.x, bottomRight.y, a8);
pl;

    const int sobelVerticalY  = ((-a0.y  - 2*a1.y  - a2.y)  /* + 0*a3.y  + 0*a4.y  + 0*a5.y  */ + (a6.y  + 2*a7.y  + a8.y))  / 4;
    const int sobelVerticalCb = ((-a0.cb - 2*a1.cb - a2.cb) /* + 0*a3.cb + 0*a4.cb + 0*a5.cb */ + (a6.cb + 2*a7.cb + a8.cb)) / 4;
    const int sobelVerticalCr = ((-a0.cr - 2*a1.cr - a2.cr) /* + 0*a3.cr + 0*a4.cr + 0*a5.cr */ + (a6.cr + 2*a7.cr + a8.cr)) / 4;

    const int sobelHorizontalY  = ((-a0.y  - 2*a3.y  - a6.y)  /* + 0*a1.y  + 0*a4.y  + 0*a7.y  */ + (a2.y  + 2*a5.y  + a8.y))  / 4;
    const int sobelHorizontalCb = ((-a0.cb - 2*a3.cb - a6.cb) /* + 0*a1.cb + 0*a4.cb + 0*a7.cb */ + (a2.cb + 2*a5.cb + a8.cb)) / 4;
    const int sobelHorizontalCr = ((-a0.cr - 2*a3.cr - a6.cr) /* + 0*a1.cr + 0*a4.cr + 0*a7.cr */ + (a2.cr + 2*a5.cr + a8.cr)) / 4;
pl;
    Pixel result;
    result.cb = result.cr = 127; result.y = 1;
    const int ans = sqrt(
                sobelVerticalY*sobelVerticalY + sobelVerticalCb*sobelVerticalCb + sobelVerticalCr*sobelVerticalCr +
                sobelHorizontalY*sobelHorizontalY + sobelHorizontalCb*sobelHorizontalCb + sobelHorizontalCr*sobelHorizontalCr);
pl;
    //-- Thresholding:
    if (ans > thresh) // [TODO] : make this threshold a configurable parameter.
    {
        result.y = 255;
        _edgePoints.push_back(middle);
    }
    //-- else: result.y remain 1 which means that this pixel has been processed
pl;
    return result;
}

