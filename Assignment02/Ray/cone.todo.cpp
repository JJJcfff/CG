#include <cmath>
#include <Util/exceptions.h>
#include <Util/polynomial.h>
#include "scene.h"
#include "cone.h"

using namespace Ray;
using namespace Util;

//////////
// Cone //
//////////

void Cone::init( const LocalSceneData &data )
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

void Cone::updateBoundingBox( void )
{
	_bBox = BoundingBox3D(	
	Point3D{center[0] - radius, 
					center[1] - height/2.0,
					center[2] - radius},
	Point3D{center[0] + radius,
	 				center[1] + height/2.0,
	 				center[2] + radius}
	);
}

void Cone::initOpenGL( void )
{
	/////////////////////////////////////////
	// Do any necessary OpenGL set-up here //
	/////////////////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

bool Cone::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;
	bool is_cone = true;
	bool is_bplate = true;
	double h = height / 2.0;
	double s = radius / (height);
	Polynomial3D<2> cone;
	cone.coefficient(2U, 0U, 0U) = 1;
	cone.coefficient(0U, 0U, 2U) = 1;
	cone.coefficient(0U, 2U, 0U) = -pow(s,2);
	cone.coefficient(1U, 0U, 0U) = -2 * center[0];
	cone.coefficient(0U, 0U, 1U) = -2 * center[2];
	cone.coefficient(0U, 1U, 0U) = 2 * s * s * (center[1] + h);

	cone.coefficient(0U, 0U, 0U) = center[0] * center[0] + center[2] * center[2] - s * s * (center[1] +h) * (center[1] + h);
	
	Polynomial1D<2> p1d = cone(ray);
	double roots[2];
	int n_root = p1d.roots(roots);
	double t_cone = roots[0];
	Point3D i_p = ray(t_cone);
	if (n_root == 0) is_cone = false;
	if (n_root == 2 && (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h)) {
		t_cone = roots[1];
		i_p = ray(t_cone);
	}
	if (t_cone < 0 || !range.isInside(Point1D{ t_cone }) || !rFilter(t_cone) || abs(i_p[1] - center[1]) > h) is_cone = false;

	Polynomial3D<1> bottom_plate;
	bottom_plate.coefficient(0U, 1U, 0U) = 1;
	bottom_plate.coefficient(0U, 0U, 0U) = h-center[1];
	Polynomial1D<1> p1d_plate = bottom_plate(ray);
	double p_t[1];
	double t_plate_min;
	int n_broot = p1d_plate.roots(p_t);
	if (n_broot == 0) is_bplate = false;
	if (n_broot > 0) {
		t_plate_min = p_t[0];
		Point3D plane_int = ray(t_plate_min);
		if (t_plate_min < 0 || !range.isInside(Point1D{ t_plate_min }) || !rFilter(t_plate_min) || radius < sqrt(pow(plane_int[0] - center[0], 2) + pow(plane_int[2] - center[2], 2))) {
			is_bplate = false;
		}
	}

	Point3D normal;
	double t_min;
	if (is_cone && is_bplate) {
		if (t_cone < t_plate_min) {
			Polynomial3D<1> x = cone.d(0);
			Polynomial3D<1> y = cone.d(1);
			Polynomial3D<1> z = cone.d(2);
			normal = Point3D{x(ray(t_cone)), y(ray(t_cone)), z(ray(t_cone))};
			t_min = t_cone;
		}
		else {
			normal = Point3D{ 0, -1, 0 };
			t_min = t_plate_min;
		}
	}
	else if (is_cone) {
		Polynomial3D<1> x = cone.d(0);
		Polynomial3D<1> y = cone.d(1);
		Polynomial3D<1> z = cone.d(2);
		normal = Point3D{x(ray(t_cone)), y(ray(t_cone)), z(ray(t_cone))};
		t_min = t_cone;
	}
	else if (is_bplate) {
		normal = Point3D{ 0, -1, 0 };
		t_min = t_plate_min;
	}
	else return false;

	RayShapeIntersectionInfo rsiInfo;
	rsiInfo.t = t_min;
	rsiInfo.position = i_p;
	rsiInfo.normal = normal.unit();
	rKernel(spInfo, rsiInfo);

	return true;
}

int Cone::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	double h = height/2.0;
	double s = radius / height;
	Polynomial3D<2> cone;
	cone.coefficient(2U, 0U, 0U) = 1;
	cone.coefficient(0U, 2U, 0U) = -(s*s);
	cone.coefficient(0U, 0U, 2U) = 1;
	cone.coefficient(1U, 0U, 0U) = -2 * center[0];
	cone.coefficient(0U, 1U, 0U) = 2 * s * s * (center[1] + h);
	cone.coefficient(0U, 0U, 1U) = -2 * center[2];
	cone.coefficient(0U, 0U, 0U) = center[0] * center[0] + center[2] * center[2] - s * s * (center[1] +h) * (center[1] + h);
	int count = 0;
	double roots[2];
	double t;
	int n_roots = cone(ray).roots(roots);
	Point3D i_p;

	if (n_roots != 0){
		t = roots[0];
		i_p = ray(roots[0]);
		if (n_roots == 2) {
			if (t < 0 || !range.isInside(Point1D{ t }) || !rFilter(t) || abs(i_p[1] - center[1]) > h) {
				t = roots[1];
				i_p = ray(t);
				n_roots = 1;
			}
		}
		if (t < 0 || !range.isInside(Point1D{ t }) || !rFilter(t) || abs(i_p[1] - center[1]) > h) n_roots = 0;
	}
	for (int i = 0; i < n_roots; i++) {
			RayShapeIntersectionInfo rsInfo;
			rsInfo.t = roots[i];
			rsInfo.position = ray(roots[i]);
			rsInfo.normal = Point3D{ cone.d(0)(i_p), cone.d(1)(i_p), cone.d(2)(i_p) }.unit();
			rKernel(spInfo, rsInfo);
			count++;
	}
	if (count < 2){
		bool is_bplate = true;
		Polynomial3D<1> bottom_plate;
		bottom_plate.coefficient(0U, 1U, 0U) = 1;
		bottom_plate.coefficient(0U, 0U, 0U) = h-center[1];
		Polynomial1D<1> p1d_plate = bottom_plate(ray);
		double p_t[1];
		double t_plate_min;
		int n_broot = p1d_plate.roots(p_t);
		if (n_broot == 0) is_bplate = false;
		if (n_broot > 0) {
			t_plate_min = p_t[0];
			Point3D plane_int = ray(t_plate_min);
			if (t_plate_min < 0 || !range.isInside(Point1D{ t_plate_min }) || !rFilter(t_plate_min) || radius < sqrt(pow(plane_int[0] - center[0], 2) + pow(plane_int[2] - center[2], 2))) {
				is_bplate = false;
			}
		}
		if (is_bplate) {
			RayShapeIntersectionInfo rsInfo;
			rsInfo.t = t_plate_min;
			rsInfo.position = ray(t_plate_min);
			rsInfo.normal = Point3D{ 0, -1, 0 };
			rKernel(spInfo, rsInfo);
			count++;
		}
	}


	return count;
}

bool Cone::isInside( Point3D p ) const
{
	double h = height/2.0;
	if (p[1] < center[1] - h || p[1] > center[1] + h) return false;
	double s = radius / height;
	double height_diff = p[1]-center[1]-h;
	double distance_to_axis = sqrt((p[0]-center[0])*(p[0]-center[0]) + (p[2]-center[2])*(p[2]-center[2]));
	if (distance_to_axis > s*height_diff) return false;
	return true;
}

void Cone::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
