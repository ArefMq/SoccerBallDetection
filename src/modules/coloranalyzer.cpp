/**
 * @file coloranalyzer.cpp
 * This module analyze the image in order to find the field green color
 * and calculate the field boundary.
 *
 * @author <a href="mailto:aref.moqadam@gmail.com">Aref Moqadam Mehr</a>
 * @date March 2017
 */

#include "coloranalyzer.h"

#include <iostream>
#include <cmath>
#include <algorithm>

using namespace MVision;

ColorAnalyzer::ColorAnalyzer(const Image &InputImage) :
    Debugable("color_analyzer"),
    _inputImage(InputImage)
{
}

void ColorAnalyzer::update()
{
    histogramAnalysis();
    fieldBoundaryDetection();
}

void ColorAnalyzer::histogramAnalysis()
{
    int histogram[256];
    for (int i=0; i<256; ++i)
        histogram[i] = 0;

    for (unsigned int x=0; x<_inputImage.width(); ++x)
        for (unsigned int y=0; y<_inputImage.height(); ++y)
            histogram[_inputImage.getPixel(x, y).cr]++;

    unsigned char peak=0;
    int peakValue=0;
    for (int i=0; i<127; ++i)
        if (histogram[i] > peakValue)
        {
            peakValue = histogram[i];
            peak = i;
        }
    _greenPeak = peak;

#ifdef DEBUG
#define HISTOGRAM_BOX 100
    for (int i=0; i<256; ++i)
        draw_line(i, HISTOGRAM_BOX, i, HISTOGRAM_BOX - (HISTOGRAM_BOX * histogram[i] / peakValue), DebugHelperInterface::Green);
    draw_line(peak, HISTOGRAM_BOX, peak, HISTOGRAM_BOX - HISTOGRAM_BOX, DebugHelperInterface::Red);
#endif
}

void ColorAnalyzer::fieldBoundaryDetection()
{
    _boundaryPoints.resize(_inputImage.width(), 0);

    //-- Calculate Highest Points
    std::vector<Vector2D> highestPoints;
    for (unsigned int x=0; x<_inputImage.width(); ++x)
    {
        int noise=0, whiteSkip=0;
        int y=_inputImage.height()-1;
        for (; y > 0; --y)
        {
            if (isGreen(x, y))
            {
                noise = 0;
                whiteSkip = 0;
            }
            else if (isWhite(x, y))
            {
                whiteSkip++;
            }
            else if (noise < 10)
            {
                noise++;
            }
            else
                break;
        }

        y += noise + whiteSkip;
        if (y < 100) // [FIXME] : [FIXME] : change this with horizon
            y = 100;
        highestPoints.push_back(Vector2D(x, y));
    }

#ifdef DEBUG
    debug_highestPoints = highestPoints;
#endif

    //-- Andrew's Monotone Algorithm
    int n = highestPoints.size(), k = 0;
    std::vector<Vector2D> H(2*n);

    // Sort points lexicographically
    std::sort(highestPoints.begin(), highestPoints.end());

    // Build lower hull
    for (int i = 0; i < n; ++i)
    {
        while (k >= 2 && ccw(H[k-2], H[k-1], highestPoints[i]) <= 0)
            k--;
        H[k++] = highestPoints[i];
    }
    H.resize(k);

    for (unsigned int x=0, i=0; x<_inputImage.width(); x++)
    {
        if ((i >= H.size()) ||
            (i == 0 && x < H.at(i).x()) ||
            (i == H.size()-1 && x > H.at(i).x()))
        {
            _boundaryPoints.at(x) = _inputImage.height();
        }

        else if (x == H.at(i).x())
        {
            _boundaryPoints.at(x) = H.at(i).y();
            i++;
        }

        else if (x < H.at(i).x())
        {
            const float dx = (float)(x-H.at(i-1).x()) / (float)(H.at(i).x()-H.at(i-1).x());
            const float dy = (float)(H.at(i).y()-H.at(i-1).y());
            _boundaryPoints.at(x) = dx*dy + H.at(i-1).y();
        }

        else // if x > H.at(i).x
            std::cerr << "fault detected... [" << __FILE__ << " :: " << __LINE__
                      << "]\n\tDetail:" << x << " => (" << H.at(i).x() << ", "
                      << H.at(i).y() << ") -- i:" << i << std::endl;
    }
}

bool ColorAnalyzer::isGreen(int x, int y) const
{
    const int d = ((int)_inputImage.getPixel(x, y).cr - _greenPeak);
    return (d < 10 && d > -10);
}

bool ColorAnalyzer::notGreen(int x, int y) const
{
    return !isGreen(x, y);
}

bool ColorAnalyzer::isWhite(int x, int y) const
{
    const Image::Pixel p = _inputImage.getPixel(x, y);
    return (p.y > 180 && std::abs(p.cb - 127) < 50 && std::abs(p.cr - 127) < 50);
}

bool ColorAnalyzer::notWhite(int x, int y) const
{
    return !isWhite(x, y);
}
