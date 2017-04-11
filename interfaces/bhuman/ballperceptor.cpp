/**
 * @file BallPerceptor.cpp
 * Interface module to port the ball detection module to B-human framework.
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date April 2017
 */

#include "ballperceptor.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Math/Geometry.h"
#include "Tools/ImageProcessing/ColorModelConversions.h"



MAKE_MODULE(BallPerceptor, Perception)

BallPerceptor::BallPerceptor()
{
}

void BallPerceptor::update(BallPercept& ballPercept)
{
    ballPercept.ballWasSeen = false;
    ballPercept.status = BallPercept::notSeen;

    if (!theCameraMatrix.isValid)
        return;

    ballDetector.update(theImage);

    if (ballDetector.getResults().size() > 0)
    {
        const Ball& b ballDetector.getResults().at(0);
        ballPercept.positionInImage = Vector2<>(b.PositionInImage()._translation.x, b.PositionInImage()._translation.y);
        ballPercept.radiusInImage = b.PositionInImage()._radious;
        Geometry::Circle c;
        if(calculateBallOnField(ballPercept))
        {
            ballPercept.status = BallPercept::seen;
            ballPercept.ballWasSeen = true;

            return;
        }
    }
}
