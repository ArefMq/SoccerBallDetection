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
//using namespace cv;
using namespace MVision;

#define tmp_path "/home/cafebazaar/Workspace/newmanoid/SoccerBallDetection/src/modules/ml/ballDetection"
#define CV_WIN_SIZE 128

PatternRecognizer::PatternRecognizer()
{
	try
	{
		mmlw = new MMLWrapper(tmp_path);
		grayROIImage = new char[CV_WIN_SIZE*CV_WIN_SIZE];
	}
	catch (const char* exp)
	{
		cerr << "Core Dump: " << exp << "\n\n";
		throw exp;
	}
}

PatternRecognizer::~PatternRecognizer()
{
	try
	{
		delete mmlw;
		delete grayROIImage;
	}
	catch (const char* exp)
	{
		cerr << "Core Dump: " << exp << "\n\n";
		throw exp;
	}
}

void PatternRecognizer::load()
{
}

void PatternRecognizer::getGrayROI(const Image& image, const Circle& ROI)
{
    const int r = ROI._radious + 5;
    const int startx = ROI._translation.x - r;
    const int starty = ROI._translation.y - r;

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

#define pl std::cout << __FILE__ << " :: " << __LINE__ << std::endl;

bool PatternRecognizer::predict(const Image& image, const Circle& ROI, Pattern )
{
    try
    {
        getGrayROI(image, ROI);
        double result = mmlw->run(grayROIImage, CV_WIN_SIZE * CV_WIN_SIZE);

        std::cout << "-----------> " << result << "(" << (result > 0.5 ? "True" : "False") << ")\n";
        return result > 0.5;
    }
    catch (const char* exp)
    {
        std::cerr << "\n\n--------------------------------------\n\n" << exp << "\n\n--------------------------------------\n\n";
        return false;
    }
}
