/**
 * @file EdgeImage.h
 * A general purpose module for edge detection
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam</a>
 * @date Apr 2016
 */

#pragma once

#include <vector>
#include "image.h"
#include "vector2D.h"

// [FIXME] : make these parameters
//#define expStep  0.0625
//#define expStep__  0.098155
//#define expCStep__ 0.7
//#define horizonOffset__ 20

#define expStep__  0.1
#define expCStep__ 0.88
#define horizonOffset__ 30


class EdgeImage : public Image
{
public:
  EdgeImage(const Image& image);
  ~EdgeImage();

  void update();
  const std::vector<Vector2D>& edgePoints() const { return _edgePoints; }
  void refine(const Vector2D& point);
  void refine(const Vector2D& topLeft, const Vector2D& bottomRight);
  static inline float edgeingStep(float y) { return y*expStep+expCStep; }

  int originY; // [FIXME] : move this somewhere else
  static float expStep;
  static float expCStep;
  static float horizonOffset;

private:
  const Image& _image;
  std::vector<std::vector<Vector2D> > _scanGraphLookup;
  std::vector<Vector2D> _edgePoints; // [TODO] : make this raw array

  static Pixel black;
  static Pixel red;

  void createLookup();
  Pixel calculateEdge(const Vector2D& topLeft, const Vector2D& middle, const Vector2D& bottomRight, int thresh=60);

  inline void getSafePixel(int x, int y, Pixel& getValue) { if (x < (int)_width && x > -1 && y < (int)_height && y > -1) { getValue = getPixel(x, y); } }
  inline void setSafePixel(int x, int y, const Pixel& setValue) { if (x < (int)_width && x > -1 && y < (int)_height && y > -1) { getPixel(x, y) = setValue; } }
  inline void getSafeOrginalPixel(int x, int y, Pixel& getValue) { if (x < (int)_width && x > -1 && y < (int)_height && y > -1) { getValue = _image.getPixel(x, y); } }
  inline float stepTable_X(int row, int col) { return _scanGraphLookup.at(row).at(col).x; }
  inline float stepTable_Y(int row, int col) { return (_scanGraphLookup.at(row).at(col).y+originY); }
};
