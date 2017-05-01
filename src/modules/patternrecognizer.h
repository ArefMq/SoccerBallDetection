/**
 * @file patternrecognizer.h
 * Ball Pattern Recognizer module
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date May 2016
 */

#pragma once

#include "tools/image.h"
#include "tools/circle.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"
#include "opencv/ml.h"

namespace MVision {

class PatternRecognizer
{
public:
    PatternRecognizer();
    ~PatternRecognizer();

    enum Pattern {Unknown=-1, Ball};

    void load();
    bool predict(const Image& image, const Circle& ROI, Pattern pattern);

private:
    CvBoost* boostTrainer;

    cv::Mat getGrayROI(const Image& image, const Circle& ROI);
};

} //-- End namespace MVision
