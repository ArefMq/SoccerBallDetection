#ifndef COLORANALYZER_H
#define COLORANALYZER_H

#include "image.h"
#include <vector>
#include <vector2D.h>

class ColorAnalyzer
{
public:
    ColorAnalyzer(const Image& InputImage);

    //-- Acutator Functions
    void update();

    //-- Interface Functions
    bool isGreen(int x, int y) const;
    bool notGreen(int x, int y) const;
    bool isWhite(int x, int y) const;
    bool notWhite(int x, int y) const;
    inline int boundary(int x) const { return _boundaryPoints.at(x<0?0:(x>=_inputImage.width()-1?_inputImage.width()-1:x)); }

    int* test;
    int test2;
//    std::vector<Vector2D> test3;

private:
    const Image& _inputImage;
    int greenPeak;
    std::vector<int> _boundaryPoints;

    void histogramAnalysis();
    void fieldBoundaryDetection();

    inline double ccw(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3) { return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x); }
};

#endif // COLORANALYZER_H
