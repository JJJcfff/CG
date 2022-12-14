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
	Point3D p( radius , height/2 , radius );
	_bBox = BoundingBox3D( center-p , center+p );
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

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

int Cone::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return 0;
}
bool Cone::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////
	// Determine if the point is inside the box here //
	///////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

void v_n_t_store_cone(std::vector<float> *vertices, Vertex v)
{
	vertices->push_back(v.position[0]);
	vertices->push_back(v.position[1]);
	vertices->push_back(v.position[2]);
	vertices->push_back(v.normal[0]);
	vertices->push_back(v.normal[1]);
	vertices->push_back(v.normal[2]);
	vertices->push_back(v.texCoordinate[0]);
	vertices->push_back(v.texCoordinate[1]);
}

void Cone::drawOpenGL( GLSLProgram * glslProgram ) const
{
	glEnable(GL_TEXTURE_2D);
	_material->drawOpenGL( glslProgram );
	const int num_slices = OpenGLTessellationComplexity+3;
	std::vector<float>* vertices = new std::vector<float>();

	Vertex top_center;
	top_center.position = Point3D( center[0] , center[1]+height/2 , center[2] );
	top_center.normal = Point3D( 0 , 1 , 0 );
	top_center.texCoordinate = Point2D( 0.5 , 0.5 );

	Vertex bottom_center;
	bottom_center.position = Point3D( center[0] , center[1]-height/2 , center[2] );
	bottom_center.normal = Point3D( 0 , -1 , 0 );
	bottom_center.texCoordinate = Point2D( 0.5 , 0.5 );

	// Draw the bottom cap
	for( int i=0 ; i<num_slices ; i++ )
	{
		float theta = 2*M_PI*i/num_slices;
		float x = radius*cos(theta);
		float y = radius*sin(theta);

		float theta2 = 2*M_PI*(i+1)/num_slices;
		float x2 = radius*cos(theta2);
		float y2 = radius*sin(theta2);

		Vertex v1;
		v1.position = Point3D( center[0]+x , center[1]-height/2 , center[2]+y );
		v1.normal = Point3D( 0 , -1 , 0 );
		v1.texCoordinate = Point2D( (x+radius)/(2*radius) , (y+radius)/(2*radius) );

		Vertex v2;
		v2.position = Point3D( center[0]+x2 , center[1]-height/2 , center[2]+y2 );
		v2.normal = Point3D( 0 , -1 , 0 );
		v2.texCoordinate = Point2D( (x2+radius)/(2*radius) , (y2+radius)/(2*radius) );

		v_n_t_store_cone(vertices, bottom_center);
		v_n_t_store_cone(vertices, v1);
		v_n_t_store_cone(vertices, v2);
	}

	//draw the side of the cone
	for( int i=0 ; i<num_slices ; i++ )
	{
		float theta = 2*M_PI*i/num_slices;
		float x = radius*cos(theta);
		float y = radius*sin(theta);

		float theta2 = 2*M_PI*(i+1)/num_slices;
		float x2 = radius*cos(theta2);
		float y2 = radius*sin(theta2);

		Vertex v1;
		v1.position = Point3D( center[0]+x , center[1]-height/2 , center[2]+y );
		v1.normal = Point3D( x , height , y ).unit();
		v1.texCoordinate = Point2D( (x+radius)/(2*radius) , (y+radius)/(2*radius) );

		Vertex v2;
		v2.position = Point3D( center[0]+x2 , center[1]-height/2 , center[2]+y2 );
		v2.normal = Point3D( x2 , height , y2 ).unit();
		v2.texCoordinate = Point2D( (x2+radius)/(2*radius) , (y2+radius)/(2*radius) );

		v_n_t_store_cone(vertices, top_center);
		v_n_t_store_cone(vertices, v2);
		v_n_t_store_cone(vertices, v1);
	}

	GLuint cone_VBO;
	glGenBuffers(1, &cone_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, cone_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size()*sizeof(float), &(*vertices)[0], GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 8*sizeof(float), 0);
	glNormalPointer(GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
	glTexCoordPointer(2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, vertices->size()/8);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDeleteBuffers(1, &cone_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glDisable(GL_TEXTURE_2D);
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
