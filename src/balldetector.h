#ifndef BALL_DETECTOR_H
#define BALL_DETECTOR_H

#include "tools/image.h"
#include "tools/ball.h"

#include <vector>

class ColorAnalyzer;
class EdgeImage;
class FRHT;
class KinematicsProvider;

class BallDetector
{
public:
	BallDetector();
	~BallDetector();

    void update(const Image& image); //-- Update with given image
	const std::vector<Ball>& getResults() const;

	EdgeImage debug_GetEdgeImage(); // [FIXME] : remove this from here

private:
	Image _image;
	std::vector<Ball> _results;
	std::vector<Vector2D> _previousPoints;

	ColorAnalyzer*      colorAnalyzer;
	EdgeImage*          edgeImage;
	FRHT*               houghTransform;
	KinematicsProvider* kinematicsProvider;

	bool checkWhitePercentage(int cx, int cy, int r);
	int searchedColor(int x, int y, int& color, int& nonGreen);
};

#endif
