#include <cmath>
#include <Util/exceptions.h>
#include "triangle.h"

using namespace Ray;
using namespace Util;

//////////////
// Triangle //
//////////////

void Triangle::init( const LocalSceneData &data )
{
	_primitiveNum = 1;
	// Set the vertex pointers
	for( int i=0 ; i<3 ; i++ )
	{
		if( _vIndices[i]==-1 ) THROW( "negative vertex index:" , _vIndices[i] );
		else if( _vIndices[i]>=data.vertices.size() ) THROW( "vertex index out of bounds: " , _vIndices[i] , " <= " , data.vertices.size() );
		else _v[i] = &data.vertices[ _vIndices[i] ];
	}


	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	

	WARN_ONCE( "method undefined" );
}

void Triangle::updateBoundingBox( void )
{
	Point3D v1 = _v[0]->position;
	Point3D v2 = _v[1]->position;
	Point3D v3 = _v[2]->position;

	double x_min = std::min(std::min(v1[0], v2[0]), v3[0]);
	double y_min = std::min(std::min(v1[1], v2[1]), v3[1]);
	double z_min = std::min(std::min(v1[2], v2[2]), v3[2]);

	double x_max = std::max(std::max(v1[0], v2[0]), v3[0]);
	double y_max = std::max(std::max(v1[1], v2[1]), v3[1]);
	double z_max = std::max(std::max(v1[2], v2[2]), v3[2]);

	_bBox = BoundingBox3D(Point3D(x_min,y_min,z_min), Point3D(x_max,y_max,z_max));
}

void Triangle::initOpenGL( void )
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

bool Triangle::processFirstIntersection(const Util::Ray3D& ray, const Util::BoundingBox1D& range, const RayIntersectionFilter& rFilter, const RayIntersectionKernel& rKernel, ShapeProcessingInfo spInfo, unsigned int tIdx) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	Point3D v1 = _v[0]->position;
	Point3D v2 = _v[1]->position;
	Point3D v3 = _v[2]->position;
	Point3D perp = Point3D::CrossProduct(v2 - v1, v3 - v1);
	Point3D normal = perp.unit();
	double d = -perp.dot(v1);

	Polynomial3D<1> plane;
	plane.coefficient(1u, 0u, 0u) = perp[0];
	plane.coefficient(0u, 1u, 0u) = perp[1];
	plane.coefficient(0u, 0u, 1u) = perp[2];
	plane.coefficient(0u, 0u, 0u) = d;

	Polynomial1D<1> r = plane(Ray3D{ray.position, ray.direction});
	double t[1];
	int num = r.roots(t);
	if (num <= 0 || t[0] <= 0 || !range.isInside(t[0])) return false;
	
	Point3D i_p = ray(t[0]);
	Point3D i_1 =  i_p - v1;
	Point3D i_2 =  i_p - v2;
	Point3D i_3 =  i_p - v3;
	Point3D a = Point3D::CrossProduct(i_2, i_3);
	Point3D b = Point3D::CrossProduct(i_1, i_2);
	Point3D g = Point3D::CrossProduct(i_3, i_1);
	double total_area = perp.dot(normal) / 2.0;
	double alpha = a.dot(normal) / 2.0 / total_area;
	double beta = g.dot(normal) / 2.0 / total_area;
	double gamma = b.dot(normal) / 2.0 / total_area;
	if (alpha < 0 || beta < 0 || gamma < 0) return false;
	
	RayShapeIntersectionInfo rsiInfo;
	rsiInfo.position = i_p;
	rsiInfo.t = t[0];
	rsiInfo.normal = _v[0]->normal * alpha + _v[1]->normal * beta + _v[2]->normal * gamma;
	rsiInfo.texture = alpha * _v[0]->texCoordinate + beta * _v[1]->texCoordinate + gamma * _v[2]->texCoordinate;
	rKernel(spInfo, rsiInfo);

	return true;
}

void Triangle::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
