#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "cylinder.h"

using namespace Ray;
using namespace Util;

//////////////
// Cylinder //
//////////////

void Cylinder::init( const LocalSceneData &data )
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

void Cylinder::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Cylinder::initOpenGL( void )
{
	/////////////////////////////////////////
	// Do any necessary OpenGL set-up here //
	/////////////////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

bool Cylinder::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;
	double h=height/2;
	Polynomial3D<2> cyn;
	cyn.coefficient(2U, 0U, 0U) = 1;
	cyn.coefficient(0U, 0U, 2U) = 1;
	cyn.coefficient(1U, 0U, 0U) = -2 * center[0];
	cyn.coefficient(0U, 0U, 1U) = -2 * center[2];
	cyn.coefficient(0U, 0U, 0U) = pow(center[0], 2) + pow(center[2], 2) - pow(radius, 2);
	
	Polynomial1D<2> p1d = cyn(ray);
	double roots[2];
	int n_root = p1d.roots(roots);
	double t_cone = roots[0];
	Point3D i_p = ray(t_cone);
	if (n_root == 0) return false;
	if (n_root == 2 && (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h)) {
		t_cone = roots[1];
		i_p = ray(t_cone);
	}
	if (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h) return false;

	Point3D normal;
	Polynomial3D<1> x = cyn.d(0);
	Polynomial3D<1> y = cyn.d(1);
	Polynomial3D<1> z = cyn.d(2);
	normal = Point3D{x(ray(t_cone)), y(ray(t_cone)), z(ray(t_cone))};

	RayShapeIntersectionInfo rsiInfo;
	rsiInfo.t = t_cone;
	rsiInfo.position = i_p;
	rsiInfo.normal = normal.unit();
	rKernel(spInfo, rsiInfo);

	return true;

}

int Cylinder::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;
	double h=height/2;
	int count = 0;
	Polynomial3D<2> cyn;
	cyn.coefficient(2U, 0U, 0U) = 1;
	cyn.coefficient(0U, 0U, 2U) = 1;
	cyn.coefficient(1U, 0U, 0U) = -2 * center[0];
	cyn.coefficient(0U, 0U, 1U) = -2 * center[2];
	cyn.coefficient(0U, 0U, 0U) = pow(center[0], 2) + pow(center[2], 2) - pow(radius, 2);
	
	Polynomial1D<2> p1d = cyn(ray);
	double roots[2];
	int n_root = p1d.roots(roots);
	double t_cone = roots[0];
	Point3D i_p = ray(t_cone);
	if (n_root == 0) return 0;
	if (n_root == 2 && (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h)) {
		t_cone = roots[1];
		i_p = ray(t_cone);
		n_root = 1;
		if (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h) n_root--;
	}

	for (int i = 0; i < n_root; i++) {
		t_cone = roots[i];
		Point3D normal;
		Polynomial3D<1> x = cyn.d(0);
		Polynomial3D<1> y = cyn.d(1);
		Polynomial3D<1> z = cyn.d(2);
		normal = Point3D{x(ray(t_cone)), y(ray(t_cone)), z(ray(t_cone))};
		RayShapeIntersectionInfo rsiInfo;
		rsiInfo.t = t_cone;
		rsiInfo.position = i_p;
		rsiInfo.normal = normal.unit();
		rKernel(spInfo, rsiInfo);
	}
	return count;
}

bool Cylinder::isInside( Point3D p ) const
{
	if (abs(p[1] - center[1]) >= height/2.0) return false;
	if (sqrt(pow(p[0] - center[0], 2) + pow(p[2] - center[2], 2)) >= radius) return false;
	return true;
}

void Cylinder::drawOpenGL( GLSLProgram *glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
