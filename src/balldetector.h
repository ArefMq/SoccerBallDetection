#ifndef BALL_DETECTOR_H
#define BALL_DETECTOR_H

#include "tools/image.h"
#include "tools/ball.h"

#include <vector>

class BallDetector
{
public:
	BallDetector();

	void update(const Image& image); //-- Update with given image
	const std::vector<Ball> getResults() const;

private:
	Image _image;
};

#endif
