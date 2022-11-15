#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "sphere.h"

using namespace Ray;
using namespace Util;

////////////
// Sphere //
////////////

void Sphere::init( const LocalSceneData &data )
{
	// Set the material pointer
	if( _materialIndex<0 ) THROW( "negative material index: " , _materialIndex );
	else if( _materialIndex>=data.materials.size() ) THROW( "material index out of bounds: " , _materialIndex , " <= " , data.materials.size() );
	else _material = &data.materials[ _materialIndex ];
	_primitiveNum = 1;

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}
void Sphere::updateBoundingBox(void)
{
	Point3D r(radius, radius, radius);
	_bBox = BoundingBox3D(center - r, center + r);
}

void Sphere::initOpenGL( void )
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

bool Sphere::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	Polynomial3D<2> sph;	//a^2 - 2 a x + b^2 - 2 b y + c^2 - 2 c z + x^2 + y^2 + z^2 - r^2= 0
	sph.coefficient(2u, 0u, 0u) = 1;
	sph.coefficient(0u, 2u, 0u) = 1;
	sph.coefficient(0u, 0u, 2u) = 1;
	sph.coefficient(1u, 0u, 0u) = -2 * center[0];
	sph.coefficient(0u, 1u, 0u) = -2 * center[1];
	sph.coefficient(0u, 0u, 1u) = -2 * center[2];
	sph.coefficient(0u, 0u, 0u) = center[0] * center[0] + center[1] * center[1] + center[2] * center[2] - radius * radius;

	Polynomial1D<2> p1d;
	p1d = sph(ray);
	double roots[2];
	unsigned int n_roots = p1d.roots(roots);

	if (n_roots == 0) return false;
	double t = roots[0];
	if (n_roots == 2) {if ((t > roots[1] && roots[1] >= 0 && range.isInside(roots[1])) || t < 0 || !range.isInside(t)) t = roots[1];}
	if (t < 0 || !range.isInside(t)) return false;

	RayShapeIntersectionInfo iInfo;
	Point3D i_p = ray(t);
	iInfo.t = t;
	iInfo.position = i_p;
	iInfo.normal = (i_p - center).unit();
	rKernel(spInfo, iInfo);
	return true;
}

int Sphere::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	Polynomial3D<2> sph;	//a^2 - 2 a x + b^2 - 2 b y + c^2 - 2 c z + x^2 + y^2 + z^2 - r^2= 0
	sph.coefficient(2u, 0u, 0u) = 1;
	sph.coefficient(0u, 2u, 0u) = 1;
	sph.coefficient(0u, 0u, 2u) = 1;
	sph.coefficient(1u, 0u, 0u) = -2 * center[0];
	sph.coefficient(0u, 1u, 0u) = -2 * center[1];
	sph.coefficient(0u, 0u, 1u) = -2 * center[2];
	sph.coefficient(0u, 0u, 0u) = center[0] * center[0] + center[1] * center[1] + center[2] * center[2] - radius * radius;

	Polynomial1D<2> p1d = sph(ray);
	double roots[2];
	int n_roots = p1d.roots(roots);

	if (n_roots == 0) return 0;
	else if (n_roots == 2) {
		if ((roots[0] < 0 || !range.isInside(roots[0])) && (roots[1] < 0 || !range.isInside(roots[1]))) return 0;
		else if (roots[0] < 0 || !range.isInside(roots[0])){
			roots[0] = roots[1];
			n_roots--;
		}else if (roots[1] < 0 || !range.isInside(roots[1])) n_roots--;
	}
	if (n_roots == 0) return 0;

	for (double t:roots) {
		RayShapeIntersectionInfo iInfo;
		iInfo.t = t;
		Point3D i_p = ray(t);
		iInfo.position = i_p;
		iInfo.normal = (i_p - center).unit();
		rKernel(spInfo, iInfo);
	}
	return n_roots;
}

bool Sphere::isInside( Point3D p ) const
{
	return (p - center).length() < radius;
}

void Sphere::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
