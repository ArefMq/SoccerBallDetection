/**
 * @file patternrecognizer.cpp
 * Ball Pattern Recognizer module
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date May 2016
 */

#include "patternrecognizer.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <stdlib.h>

using namespace std;
using namespace MVision;

#define CV_WIN_SIZE 128

PatternRecognizer::PatternRecognizer() : Debugable("pattern_recognizer")
{
}

PatternRecognizer::~PatternRecognizer()
{
}

void PatternRecognizer::load()
{
}

void PatternRecognizer::getGrayROI(const Image& image, const Circle& ROI)
{
    const int r = ROI.radious() + 5;
    const int startx = ROI.translation().x() - r;
    const int starty = ROI.translation().y() - r;

    for (int hx=0; hx<CV_WIN_SIZE; ++hx)
        for (int hy=0; hy<CV_WIN_SIZE; ++hy)
        {
            const int x  = startx + hx * r * 2 / CV_WIN_SIZE;
            const int y  = starty + hy * r * 2 / CV_WIN_SIZE;

            if (x >= 0 && y >= 0 && x < (int)image.width() && y < (int)image.height() &&
                (hx-64)*(hx-64)+(hy-64)*(hy-64) < 4096)
            	grayROIImage[hx * CV_WIN_SIZE + hy] = image.getPixel(x, y).y;
            else
            	grayROIImage[hx * CV_WIN_SIZE + hy] = 0;
        }
}

bool PatternRecognizer::predict(const Image& image, const Circle& ROI)
{
    return true;
}
