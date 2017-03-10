/**
 * @file FRHT.h
 * A general purpose fast random hough transform module for detecting circles
 * @author <a href="mailto:a.moqadam@mrl-spl.ir">Aref Moqadam</a>
 * @date Apr 2016
 */

#pragma once

#include "EdgeImage.h"
#include "vector2D.h"
#include "coloranalyzer.h"

#include <qpainter.h>

class FRHT
{
public:
  FRHT(EdgeImage& image);
  ~FRHT();

  void update(std::vector<Vector2D> preferredRandomPoints = std::vector<Vector2D>());
  const std::vector<Vector3D>& extractedCircles() const;
  void resizeCirlcle(Vector3D& circle, int maxBoundary, const ColorAnalyzer& colorAnalyzer, QPainter &qpn);

private:
  EdgeImage& _image;
  std::vector<Vector3D> _circles;

  void findCircle(const Vector2D& centerPoint, int step);
  void checkCircle(const Vector2D &p1, const Vector2D &p2, const Vector2D &p3);


  class SearchCell {
  public:
    SearchCell(int Distance, const Vector2D& Point) : distance(Distance), point(Point) {}
    int      distance;
    Vector2D point;
  };


public:
  //-- The result is in this format: (cx, cy), radius
  static Vector3D fitACircle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3);
};
