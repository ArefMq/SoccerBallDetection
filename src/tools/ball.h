#ifndef BALL_H
#define BALL_H

#include "circle.h"

namespace MVision {

class Ball
{
public:
    Ball(const Circle& position, float Confidence=1.0f);
    Ball(float X, float Y, float Radious, float Confidence=1.0f);
	Ball();

	inline const Circle& PositionInImage() const { return _positionInImage; }
	inline float Confidence() const { return _confidence; }

private:
	Circle _positionInImage;
	float _confidence;
};
}

#endif // BALL_H
