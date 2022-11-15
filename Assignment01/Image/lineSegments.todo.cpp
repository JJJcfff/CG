#include "lineSegments.h"
#include <math.h>
#include <Util/exceptions.h>

using namespace Util;
using namespace Image;

////////////////////////////
// Image processing stuff //
////////////////////////////
double OrientedLineSegment::length( void ) const
{
  double dx = endPoints[1][0] - endPoints[0][0];
  double dy = endPoints[1][1] - endPoints[0][1];
	return sqrt(dx*dx + dy*dy);
}
double OrientedLineSegment::distance( Point2D p ) const // shortest distance from point p to current line segment
{
  double x = p[0],
         y = p[1],
         x1 = endPoints[0][0],
         y1 = endPoints[0][1],
         x2 = endPoints[1][0],
         y2 = endPoints[1][1];
  double u = (x-x1)*(x2-x1) + (y-y1)*(y2-y1);
  if ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) != 0) {u /= (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
  }
  if (u<0){
    return sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
  } else if (u>1){
    return sqrt((x-x2)*(x-x2) + (y-y2)*(y-y2));
  } else {
    return fabs((y2-y1)*x - (x2-x1)*y + x2*y1 - y2*x1) / sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
  }
}

Point2D OrientedLineSegment::perpendicular( void ) const
{
  double dx = endPoints[1][0] - endPoints[0][0];
  double dy = endPoints[1][1] - endPoints[0][1];
  double len = sqrt(dx*dx + dy*dy);
  return Point2D(dy/len, -dx/len);
}

Point2D OrientedLineSegment::GetSourcePosition( const OrientedLineSegment& source , const OrientedLineSegment& destination , Point2D target )
{
  Point2D s = {source.endPoints[1][0] - source.endPoints[0][0], source.endPoints[1][1] - source.endPoints[0][1]}, d = {destination.endPoints[1][0] - destination.endPoints[0][0], destination.endPoints[1][1] - destination.endPoints[0][1]};
  Point2D d_perp = destination.perpendicular();
  Point2D s_perp = source.perpendicular();
  Point2D t = {target[0] - destination.endPoints[0][0],target[1] - destination.endPoints[0][1]};
  double v = t[0]*d_perp[0] + t[1]*d_perp[1];
  double u = (t[0]*d[0] + t[1]*d[1]) / pow(d.length(),2) ;
  Point2D result = {source.endPoints[0][0] + u*s[0] + s_perp[0]*v,
                        source.endPoints[0][1] + u*s[1] + s_perp[1]*v};
  return result;
}