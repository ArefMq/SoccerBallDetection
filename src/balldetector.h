#ifndef BALL_DETECTOR_H
#define BALL_DETECTOR_H

#include "tools/image.h"
#include "tools/ball.h"

class ColorAnalyzer;
class EdgeImage;
class FRHT;
class KinematicsProvider;

#include <vector>

class BallDetector
{
public:
	BallDetector();
	~BallDetector();

	void update(const Image& image); //-- Update with given image
	const std::vector<Ball>& getResults() const;

	EdgeImage debug_GetEdgeImage();

private:
	Image _image;
	std::vector<Ball> _results;
	std::vector<Vector2D> _previousPoints;

	ColorAnalyzer*      colorAnalyzer;
	EdgeImage*          edgeImage;
	FRHT*               houghTransform;
	KinematicsProvider* kinematicsProvider;
};

#endif
