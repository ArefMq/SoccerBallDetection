#ifndef COLORANALYZER_H
#define COLORANALYZER_H

#include "image.h"

class ColorAnalyzer
{
public:
    ColorAnalyzer(const Image& InputImage);

    //-- Acutator Functions
    void update();

    //-- Interface Functions
    bool isGreen(int x, int y) const;
    bool notGreen(int x, int y) const;

    int* test;
    int test2;

private:
    const Image& _inputImage;
    int greenPeak;
};

#endif // COLORANALYZER_H
