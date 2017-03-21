#include "balldetector.h"

BallDetector::BallDetector()
{
}

void BallDetector::update(const Image& image)
{
}

const std::vector<Ball> BallDetector::getResults() const
{
    std::vector<Ball> results;
    results.push_back(Ball(100, 100, 10, 1));
    return results;
}
