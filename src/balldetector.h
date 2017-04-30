#ifndef BALL_DETECTOR_H
#define BALL_DETECTOR_H

#include "tools/image.h"
#include "tools/ball.h"

#include <vector>

namespace MVision {

class ColorAnalyzer;
class EdgeImage;
class FRHT;
class KinematicsProvider;

class BallDetector
{
public:
	BallDetector();
	~BallDetector();

	/**
	 * @brief main process function; run the ball detection module on given image.
	 * This do three things: 1) update the input image. 2) call the private
	 * update function to run the module process and 3) calculate the time of
	 * that function.
	 * @param image: image to detect the ball in
	 */
    void update(const Image& image);
    /**
     * @return the extracted ball in the image
     */
	const std::vector<Ball>& getResults() const;
	/**
	 * @return the average time took to process the image in seconds
	 */
	double averageCycleTime() const;

	EdgeImage debug_GetEdgeImage(); // [FIXME] : remove this from here

private:
	Image _image; /**< Reference to the input image */
	std::vector<Ball> _results; /**< Set of extracted ball in the image */
	std::vector<Vector2D> _previousPoints; /**< Points marked as prospective ball in previous cycle */
	double _averageCycleTime; /**< Average cycle time */

	//-- Main Modules
	ColorAnalyzer*      colorAnalyzer;
	EdgeImage*          edgeImage;
	FRHT*               houghTransform;
	KinematicsProvider* kinematicsProvider;

	/**
	 * @brief run the process need to detect a ball in the image
	 */
	void update();
	/**
	 * @brief check to see if the given circle contain enough white pixels
	 * and make sure it does not have any extra green pixels.
	 * @param cx, cy, r: parameters declaring circle.
	 * @return true if it is in a proper color fashion
	 */
	bool checkWhitePercentage(int cx, int cy, int r);
	/**
	 * @brief count the colors in given pixel
	 * @param x, y: position of the pixel
	 * @param color: plus-one this parameter if the pixel is white
	 * @param nonGreen: plus-one this parameter if the pixel is not green
	 * @return +1 if the pixel is valid (it is inside the image); 0 otherwise
	 */
	int searchedColor(int x, int y, int& color, int& nonGreen);
	/**
	 * @brief check the ball texture via machine learning to see if it matches
	 * the previously trained ball data.
	 * @param ball: the given position to check
	 * @return true if the texture matches the ball
	 */
	bool checkBallTexture(const Circle& ball);
};
}

#endif
