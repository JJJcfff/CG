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
void Sphere::updateBoundingBox( void )
{
	Point3D p( radius , radius , radius );
	_bBox = BoundingBox3D( center-p , center+p );
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

	//////////////////////////////////////////////////////////////
	// Compute the intersection of the sphere with the ray here //
	//////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

int Sphere::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	//////////////////////////////////////////////////////////////
	// Compute the intersection of the sphere with the ray here //
	//////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return 0;
}

bool Sphere::isInside( Point3D p ) const
{
	//////////////////////////////////////////////////////
	// Determine if the point is inside the sphere here //
	//////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

void Sphere::drawOpenGL( GLSLProgram * glslProgram ) const
{
	_material->drawOpenGL(glslProgram);
	const int cplx = OpenGLTessellationComplexity+4;
	const float latitude = Pi/cplx;
	const float longtitude = 2.0*Pi/cplx;
	
	float phi;
	float theta;
	std::vector<Vertex> vertexData[cplx][cplx];

	for (int i = 0; i < cplx; i++) {
		phi = Pi / 2 - i * latitude;
		float z = radius * sin(phi);
		for (int j = 0; j < cplx; j++) {
			Vertex v;
			theta = j * longtitude;
			float x = radius * cos(phi) * cos(theta);
			float y = radius * cos(phi) * sin(theta);
			Point3D p(x, y, z);
			v.position = p;
			v.normal = (p - center).unit();
			v.texCoordinate = Point2D((float)j / cplx, (float)i / cplx);
			vertexData[i][j].push_back(v);
		}
	}

	// std::vector<float> vertexArray;
	// std::vector<unsigned int> indexArray;
	// GLuint sph_vbo;
	// GLuint sph_ebo;

	// for (int i = 0; i < cplx; i++)
	// {
	// 	for (int j = 0; j < cplx; j++)
	// 	{
	// 		//get the four vertices
	// 		Vertex v1 = vertexData[i][j][0];
	// 		Vertex v2 = vertexData[(i + 1) % cplx][j][0];
	// 		Vertex v3 = vertexData[(i + 1) % cplx][(j + 1) % cplx][0];
	// 		Vertex v4 = vertexData[i][(j + 1) % cplx][0];

	// 		vertexArray.push_back(v1.position[0]);
	// 		vertexArray.push_back(v1.position[1]);
	// 		vertexArray.push_back(v1.position[2]);
	// 		vertexArray.push_back(v1.normal[0]);
	// 		vertexArray.push_back(v1.normal[1]);
	// 		vertexArray.push_back(v1.normal[2]);
	// 		vertexArray.push_back(v1.texCoordinate[0]);
	// 		vertexArray.push_back(v1.texCoordinate[1]);
	// 		indexArray.push_back(i*j);

	// 		vertexArray.push_back(v2.position[0]);
	// 		vertexArray.push_back(v2.position[1]);
	// 		vertexArray.push_back(v2.position[2]);
	// 		vertexArray.push_back(v2.normal[0]);
	// 		vertexArray.push_back(v2.normal[1]);
	// 		vertexArray.push_back(v2.normal[2]);
	// 		vertexArray.push_back(v2.texCoordinate[0]);
	// 		vertexArray.push_back(v2.texCoordinate[1]);
	// 		indexArray.push_back((i + 1) % cplx * j);

	// 		vertexArray.push_back(v3.position[0]);
	// 		vertexArray.push_back(v3.position[1]);
	// 		vertexArray.push_back(v3.position[2]);
	// 		vertexArray.push_back(v3.normal[0]);
	// 		vertexArray.push_back(v3.normal[1]);
	// 		vertexArray.push_back(v3.normal[2]);
	// 		vertexArray.push_back(v3.texCoordinate[0]);
	// 		vertexArray.push_back(v3.texCoordinate[1]);
	// 		indexArray.push_back((i + 1) % cplx * (j + 1) % cplx);
			
	// 		vertexArray.push_back(v1.position[0]);
	// 		vertexArray.push_back(v1.position[1]);
	// 		vertexArray.push_back(v1.position[2]);
	// 		vertexArray.push_back(v1.normal[0]);
	// 		vertexArray.push_back(v1.normal[1]);
	// 		vertexArray.push_back(v1.normal[2]);
	// 		vertexArray.push_back(v1.texCoordinate[0]);
	// 		vertexArray.push_back(v1.texCoordinate[1]);
	// 		indexArray.push_back(i*j);

	// 		vertexArray.push_back(v3.position[0]);
	// 		vertexArray.push_back(v3.position[1]);
	// 		vertexArray.push_back(v3.position[2]);
	// 		vertexArray.push_back(v3.normal[0]);
	// 		vertexArray.push_back(v3.normal[1]);
	// 		vertexArray.push_back(v3.normal[2]);
	// 		vertexArray.push_back(v3.texCoordinate[0]);
	// 		vertexArray.push_back(v3.texCoordinate[1]);
	// 		indexArray.push_back((i + 1) % cplx * (j + 1) % cplx);

	// 		vertexArray.push_back(v4.position[0]);
	// 		vertexArray.push_back(v4.position[1]);
	// 		vertexArray.push_back(v4.position[2]);
	// 		vertexArray.push_back(v4.normal[0]);
	// 		vertexArray.push_back(v4.normal[1]);
	// 		vertexArray.push_back(v4.normal[2]);
	// 		vertexArray.push_back(v4.texCoordinate[0]);
	// 		vertexArray.push_back(v4.texCoordinate[1]);
	// 		indexArray.push_back(i * (j + 1) % cplx);
	// 	}
	// }

	// glGenBuffers(1, &sph_vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, sph_vbo);
	// glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), &vertexArray[0], GL_STATIC_DRAW);

	// glGenBuffers(1, &sph_ebo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sph_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(unsigned int), &indexArray[0], GL_STATIC_DRAW);

	// //draw
	// glBindBuffer(GL_ARRAY_BUFFER, sph_vbo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sph_ebo);

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// glEnableVertexAttribArray(2);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, (void*)0);

	// glDisableVertexAttribArray(0);
	// glDisableVertexAttribArray(1);
	// glDisableVertexAttribArray(2);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (int i = 0; i < cplx; i++)
	{
		for (int j = 0; j < cplx; j++)
		{
			//get the four vertices
			Vertex v1 = vertexData[i][j][0];
			Vertex v2 = vertexData[(i + 1) % cplx][j][0];
			Vertex v3 = vertexData[(i + 1) % cplx][(j + 1) % cplx][0];
			Vertex v4 = vertexData[i][(j + 1) % cplx][0];
			//draw the two triangles
			glBegin(GL_TRIANGLES);
			glNormal3f(v1.normal[0], v1.normal[1], v1.normal[2]);
			glVertex3f(v1.position[0], v1.position[1], v1.position[2]);
			glNormal3f(v2.normal[0], v2.normal[1], v2.normal[2]);
			glVertex3f(v2.position[0], v2.position[1], v2.position[2]);
			glNormal3f(v3.normal[0], v3.normal[1], v3.normal[2]);
			glVertex3f(v3.position[0], v3.position[1], v3.position[2]);
			
			glNormal3f(v1.normal[0], v1.normal[1], v1.normal[2]);
			glVertex3f(v1.position[0], v1.position[1], v1.position[2]);
			glNormal3f(v3.normal[0], v3.normal[1], v3.normal[2]);
			glVertex3f(v3.position[0], v3.position[1], v3.position[2]);
			glNormal3f(v4.normal[0], v4.normal[1], v4.normal[2]);
			glVertex3f(v4.position[0], v4.position[1], v4.position[2]);
			glEnd();
		}
	}
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

