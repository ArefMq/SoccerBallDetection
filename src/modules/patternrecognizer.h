/**
 * @file patternrecognizer.h
 * Ball Pattern Recognizer module
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date May 2016
 */

#pragma once

#include "tools/image.h"
#include "tools/circle.h"

namespace MVision {

class PatternRecognizer
{
public:
    PatternRecognizer();
    ~PatternRecognizer();

    enum Pattern {Unknown=-1, Ball};

    void load();
    bool predict(const Image& image, const Circle& ROI, Pattern pattern);
};

} //-- End namespace MVision
