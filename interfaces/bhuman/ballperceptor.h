/**
 * @file BallPerceptor.h
 * Interface module to port the ball detection module to B-human framework.
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam Mehr</a>
 * @date April 2017
 */

#pragma once

#include "Tools/Module/Module.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Perception/ImageCoordinateSystem.h"
#include "Representations/Perception/BallPercept.h"
#include "Representations/Perception/ColorReference.h"
#include "Representations/Perception/FieldBoundary.h"
#include "Representations/Perception/BodyContour.h"
#include "Tools/Debugging/DebugImages.h"
#include "Representations/Infrastructure/JointData.h"

#include "../../src/balldetector.h"

class Image;

MODULE(BallPerceptor)
  REQUIRES(FieldDimensions)
  REQUIRES(Image)
  REQUIRES(CameraMatrix)
  REQUIRES(ImageCoordinateSystem)
  REQUIRES(CameraInfo)
  REQUIRES(ColorReference)
  REQUIRES(FieldBoundary)
  REQUIRES(BodyContour)
  REQUIRES(FilteredJointData)
  PROVIDES_WITH_MODIFY_AND_OUTPUT_AND_DRAW(BallPercept)
END_MODULE

class BallPerceptor: public BallPerceptorBase
{
public:
  BallPerceptor();

private:
  void update(BallPercept& ballPercept);

  BallDetector ballDetector;
};
