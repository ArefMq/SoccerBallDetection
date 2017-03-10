#include "coloranalyzer.h"

#include <iostream>

ColorAnalyzer::ColorAnalyzer(const Image &InputImage) :
    _inputImage(InputImage)
{
    test = new int[256];
}

void ColorAnalyzer::update()
{
//    std::cout << "DEBUG 1: (" << _inputImage.width() << "x" << _inputImage.height() << "=" << _inputImage.width() * _inputImage.height() << std::endl;
    int histogram[256];
    for (int i=0; i<256; ++i)
        histogram[i] = 0;

    for (unsigned int x=0; x<_inputImage.width(); ++x)
        for (unsigned int y=0; y<_inputImage.height(); ++y)
//        {
            histogram[_inputImage.getPixel(x, y).cr]++;
//            std::cout << (int)_inputImage.getPixel(x, y).cb << "] " << histogram[_inputImage.getPixel(x, y).cb] << std::endl;
//        }

    unsigned char peak=0;
    int peakValue=0;
//    for (int i=0; i<256; ++i)
    for (int i=0; i<127; ++i)
        if (histogram[i] > peakValue)
        {
            peakValue = histogram[i];
            peak = i;
        }

    for (int i=0; i<256; ++i)
        test[i] = histogram[i];
    test2 = peakValue;
    greenPeak = peak;
}

bool ColorAnalyzer::isGreen(int x, int y) const
{
    const int d = ((int)_inputImage.getPixel(x, y).cr - greenPeak);
    return (d < 10 && d > -10);
}

bool ColorAnalyzer::notGreen(int x, int y) const
{
    return !isGreen(x, y);
}

