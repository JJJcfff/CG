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
	Point3D pList[3];
	for( int i=0 ; i<3 ; i++ ) pList[i] = _v[i]->position;
	_bBox = BoundingBox3D( pList , 3 );
	for( int i=0 ; i<3 ; i++ ) _bBox[0][i] -= Epsilon , _bBox[1][i] += Epsilon;
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

bool Triangle::processFirstIntersection( const Util::Ray3D &ray , const Util::BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

void Triangle::drawOpenGL( GLSLProgram * glslProgram ) const
{
	// glBegin( GL_TRIANGLES );
	// glNormal3f( _v[0]->normal[0] , _v[0]->normal[1] , _v[0]->normal[2] );
	// glTexCoord2f( _v[0]->texCoordinate[0] , _v[0]->texCoordinate[1] );
	// glVertex3f( _v[0]->position[0] , _v[0]->position[1] , _v[0]->position[2] );
	// glNormal3f( _v[1]->normal[0] , _v[1]->normal[1] , _v[1]->normal[2] );
	// glTexCoord2f( _v[1]->texCoordinate[0] , _v[1]->texCoordinate[1] );
	// glVertex3f( _v[1]->position[0] , _v[1]->position[1] , _v[1]->position[2] );
	// glNormal3f( _v[2]->normal[0] , _v[2]->normal[1] , _v[2]->normal[2] );
	// glTexCoord2f( _v[2]->texCoordinate[0] , _v[2]->texCoordinate[1] );
	// glVertex3f( _v[2]->position[0] , _v[2]->position[1] , _v[2]->position[2] );
	// glEnd();
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
