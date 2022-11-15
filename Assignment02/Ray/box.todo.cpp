#include <cmath>
#include  <Util/exceptions.h>
#include "scene.h"
#include "box.h"

using namespace Ray;
using namespace Util;

/////////
// Box //
/////////

void Box::init( const LocalSceneData& data )
{
	// Set the material pointer
	if( _materialIndex<0 ) THROW( "negative material index: " , _materialIndex );
	else if( _materialIndex>=data.materials.size() ) THROW( "material index out of bounds: " , _materialIndex , " <= " , data.materials.size() );
	else _material = &data.materials[ _materialIndex ];
	_primitiveNum = 1;

	//////////////////////////////////
	// Do any necessary set-up here //
	//////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Box::updateBoundingBox( void )
{
	Point3D min = center - (length / 2.0);
	Point3D max = center + (length / 2.0);
	_bBox = BoundingBox3D(min, max);
}

void Box::initOpenGL( void )
{
	/////////////////////////////////////////
	// Do any necessary OpenGL set-up here //
	/////////////////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
bool Box::processFirstIntersection( const Util::Ray3D &ray , const Util::BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	Point3D p1 = center - length / 2.0;
	Point3D p2 = center + length / 2.0;

	double tmin = -Infinity;
	double tmax = Infinity;
	for (int i = 0; i < 3; i++) {
		double t1 = (p1[i] - ray.position[i]) / ray.direction[i];
		double t2 = (p2[i] - ray.position[i]) / ray.direction[i]; 
		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));
	}
	
	if ((tmin < 0 && tmax < 0) || (tmin > tmax))return false;
	if (!range.isInside(Point1D{ tmin }) || !rFilter(tmin)) tmin = tmax;
	if (!range.isInside(Point1D{ tmin }) || !rFilter(tmin)) return false;


	Point3D normal;
	Point3D i_p = ray(tmin);
	RayShapeIntersectionInfo rsInfo;
	rsInfo.position = i_p;
	rsInfo.t = tmin;
	if (     abs(i_p[0] - p1[0]) < Epsilon) normal = Point3D(-1, 0 , 0);
	else if (abs(i_p[0] - p2[0]) < Epsilon) normal = Point3D(1, 0 , 0);
	else if (abs(i_p[1] - p1[1]) < Epsilon) normal = Point3D(0, -1 , 0);
	else if (abs(i_p[1] - p2[1]) < Epsilon) normal = Point3D(0, 1 , 0);
	else if (abs(i_p[2] - p1[2]) < Epsilon) normal = Point3D(0, 0 , -1);
	else if (abs(i_p[2] - p2[2]) < Epsilon) normal = Point3D(0, 0 , 1);
	rsInfo.normal = normal;
	rKernel(spInfo, rsInfo);
	
	return true;
}

int Box::processAllIntersections( const Util::Ray3D &ray , const Util::BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	Point3D p1 = center - length / 2.0;
	Point3D p2 = center + length / 2.0;
	double tmin = -Infinity;
	double tmax = Infinity;
	for (int i = 0; i < 3; i++) {
		double t1 = (p1[i] - ray.position[i]) / ray.direction[i];
		double t2 = (p2[i] - ray.position[i]) / ray.direction[i]; 
		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));
	}
	if ((tmin < 0 && tmax < 0)||(tmin > tmax)) return false;
	double t[2] = { tmin, tmax };

	int count = 0;
	for (double r : t) {
		if (range.isInside(Point1D{r}) && rFilter(r)) {		
			Point3D i_p = ray(r);
			RayShapeIntersectionInfo rsInfo;
			rsInfo.position = i_p;
			rsInfo.t = r;
			Point3D normal;
			if (     abs(i_p[0] - p1[0]) < Epsilon) normal = Point3D(-1, 0 , 0);
			else if (abs(i_p[0] - p2[0]) < Epsilon) normal = Point3D(1, 0 , 0);
			else if (abs(i_p[1] - p1[1]) < Epsilon) normal = Point3D(0, -1 , 0);
			else if (abs(i_p[1] - p2[1]) < Epsilon) normal = Point3D(0, 1 , 0);
			else if (abs(i_p[2] - p1[2]) < Epsilon) normal = Point3D(0, 0 , -1);
			else if (abs(i_p[2] - p2[2]) < Epsilon) normal = Point3D(0, 0 , 1);
			rsInfo.normal = normal;
			rKernel(spInfo, rsInfo);
			count++;
		}
	}
	return count;
}


bool Box::isInside( Point3D p ) const
{
	return p[0] < _bBox[1][0] 
			&& p[0] > _bBox[0][0] 
			&& p[1] < _bBox[1][1] 
			&& p[1] > _bBox[0][1] 
			&& p[2] < _bBox[1][2] 
			&& p[2] > _bBox[0][2];
}

void Box::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
