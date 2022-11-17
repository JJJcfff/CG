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
    _bBox = BoundingBox3D( center-(length/2) , center+(length/2) );
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

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

int Box::processAllIntersections( const Util::Ray3D &ray , const Util::BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return 0;
}
bool Box::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////
	// Determine if the point is inside the box here //
	///////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

void v_n_t_store(std::vector<float> *vertices, Point3D p, Point3D n, Point2D t)
{
	vertices->push_back(p[0]);
	vertices->push_back(p[1]);
	vertices->push_back(p[2]);
	vertices->push_back(n[0]);
	vertices->push_back(n[1]);
	vertices->push_back(n[2]);
	vertices->push_back(t[0]);
	vertices->push_back(t[1]);
}


void Box::drawOpenGL( GLSLProgram * glslProgram ) const
{
	glEnable(GL_TEXTURE_2D);
	_material->drawOpenGL( glslProgram );
	Point3D p1 = {center[0]-length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2};
	Point3D p2 = {center[0]+length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2};
	Point3D p3 = {center[0]+length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2};
	Point3D p4 = {center[0]-length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2};
	Point3D p5 = {center[0]-length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2};
	Point3D p6 = {center[0]+length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2};
	Point3D p7 = {center[0]+length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2};
	Point3D p8 = {center[0]-length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2};

	std::vector<float> *vertices = new std::vector<float>();
	//front
	v_n_t_store(vertices, p1, {0,0,1}, {0,0});
	v_n_t_store(vertices, p2, {0,0,1}, {1,0});
	v_n_t_store(vertices, p3, {0,0,1}, {1,1});
	v_n_t_store(vertices, p1, {0,0,1}, {0,0});
	v_n_t_store(vertices, p3, {0,0,1}, {1,1});
	v_n_t_store(vertices, p4, {0,0,1}, {0,1});
	//right
	v_n_t_store(vertices, p2, {1,0,0}, {0,0});
	v_n_t_store(vertices, p6, {1,0,0}, {1,0});
	v_n_t_store(vertices, p7, {1,0,0}, {1,1});
	v_n_t_store(vertices, p2, {1,0,0}, {0,0});
	v_n_t_store(vertices, p7, {1,0,0}, {1,1});
	v_n_t_store(vertices, p3, {1,0,0}, {0,1});
	//back
	v_n_t_store(vertices, p6, {0,0,-1}, {0,0});
	v_n_t_store(vertices, p5, {0,0,-1}, {1,0});
	v_n_t_store(vertices, p8, {0,0,-1}, {1,1});
	v_n_t_store(vertices, p6, {0,0,-1}, {0,0});
	v_n_t_store(vertices, p8, {0,0,-1}, {1,1});
	v_n_t_store(vertices, p7, {0,0,-1}, {0,1});
	//left
	v_n_t_store(vertices, p5, {-1,0,0}, {0,0});
	v_n_t_store(vertices, p1, {-1,0,0}, {1,0});
	v_n_t_store(vertices, p4, {-1,0,0}, {1,1});
	v_n_t_store(vertices, p5, {-1,0,0}, {0,0});
	v_n_t_store(vertices, p4, {-1,0,0}, {1,1});
	v_n_t_store(vertices, p8, {-1,0,0}, {0,1});
	//top
	v_n_t_store(vertices, p4, {0,1,0}, {0,0});
	v_n_t_store(vertices, p3, {0,1,0}, {1,0});
	v_n_t_store(vertices, p7, {0,1,0}, {1,1});
	v_n_t_store(vertices, p4, {0,1,0}, {0,0});
	v_n_t_store(vertices, p7, {0,1,0}, {1,1});
	v_n_t_store(vertices, p8, {0,1,0}, {0,1});
	//bottom
	v_n_t_store(vertices, p1, {0,-1,0}, {0,0});
	v_n_t_store(vertices, p5, {0,-1,0}, {1,0});
	v_n_t_store(vertices, p6, {0,-1,0}, {1,1});
	v_n_t_store(vertices, p1, {0,-1,0}, {0,0});
	v_n_t_store(vertices, p6, {0,-1,0}, {1,1});
	v_n_t_store(vertices, p2, {0,-1,0}, {0,1});
	
	GLuint box_VBO;
	glGenBuffers(1, &box_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, box_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size()*sizeof(float), vertices->data(), GL_STATIC_DRAW);
	
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
	
	delete vertices;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);

	
	// glBegin(GL_TRIANGLES);
	// //front
	// glNormal3f(0,0,1);
	// glVertex3f(p1[0],p1[1],p1[2]);
	// glVertex3f(p2[0],p2[1],p2[2]);
	// glVertex3f(p3[0],p3[1],p3[2]);
	// glVertex3f(p1[0],p1[1],p1[2]);
	// glVertex3f(p3[0],p3[1],p3[2]);
	// glVertex3f(p4[0],p4[1],p4[2]);
	// //right
	// glNormal3f(1,0,0);
	// glVertex3f(p2[0],p2[1],p2[2]);
	// glVertex3f(p6[0],p6[1],p6[2]);
	// glVertex3f(p7[0],p7[1],p7[2]);
	// glVertex3f(p2[0],p2[1],p2[2]);
	// glVertex3f(p7[0],p7[1],p7[2]);
	// glVertex3f(p3[0],p3[1],p3[2]);
	// //back
	// glNormal3f(0,0,-1);
	// glVertex3f(p6[0],p6[1],p6[2]);
	// glVertex3f(p5[0],p5[1],p5[2]);
	// glVertex3f(p8[0],p8[1],p8[2]);
	// glVertex3f(p6[0],p6[1],p6[2]);
	// glVertex3f(p8[0],p8[1],p8[2]);
	// glVertex3f(p7[0],p7[1],p7[2]);
	// //left
	// glNormal3f(-1,0,0);
	// glVertex3f(p5[0],p5[1],p5[2]);
	// glVertex3f(p1[0],p1[1],p1[2]);
	// glVertex3f(p4[0],p4[1],p4[2]);
	// glVertex3f(p5[0],p5[1],p5[2]);
	// glVertex3f(p4[0],p4[1],p4[2]);
	// glVertex3f(p8[0],p8[1],p8[2]);
	// //top
	// glNormal3f(0,1,0);
	// glVertex3f(p3[0],p3[1],p3[2]);
	// glVertex3f(p7[0],p7[1],p7[2]);
	// glVertex3f(p8[0],p8[1],p8[2]);
	// glVertex3f(p3[0],p3[1],p3[2]);
	// glVertex3f(p8[0],p8[1],p8[2]);
	// glVertex3f(p4[0],p4[1],p4[2]);
	// //bottom
	// glNormal3f(0,-1,0);
	// glVertex3f(p5[0],p5[1],p5[2]);
	// glVertex3f(p6[0],p6[1],p6[2]);
	// glVertex3f(p2[0],p2[1],p2[2]);
	// glVertex3f(p5[0],p5[1],p5[2]);
	// glVertex3f(p2[0],p2[1],p2[2]);
	// glVertex3f(p1[0],p1[1],p1[2]);
	// glEnd();


	// glBegin(GL_QUADS);
	// //front
	// glNormal3f(0,0,1);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// //back
	// glNormal3f(0,0,-1);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// //left
	// glNormal3f(-1,0,0);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// //right
	// glNormal3f(1,0,0);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// //top
	// glNormal3f(0,1,0);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]+length[1]/2,center[2]-length[2]/2);
	// //bottom
	// glNormal3f(0,-1,0);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]-length[2]/2);
	// glVertex3f(center[0]+length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// glVertex3f(center[0]-length[0]/2,center[1]-length[1]/2,center[2]+length[2]/2);
	// glEnd();

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
