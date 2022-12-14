#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "torus.h"

using namespace Ray;
using namespace Util;

///////////
// Torus //
///////////

void Torus::init(const LocalSceneData &data)
{
	// Set the material pointer
	if (_materialIndex < 0)
		THROW("negative material index: ", _materialIndex);
	else if (_materialIndex >= data.materials.size())
		THROW("material index out of bounds: ", _materialIndex, " <= ", data.materials.size());
	else
		_material = &data.materials[_materialIndex];
	_primitiveNum = 1;

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE("method undefined");
}
void Torus::updateBoundingBox(void)
{
	Point3D p( iRadius+oRadius , oRadius , iRadius+oRadius );
	_bBox = BoundingBox3D( center-p , center+p );
}

void Torus::initOpenGL(void)
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE("method undefined");

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}

bool Torus::processFirstIntersection(const Ray3D &ray, const BoundingBox1D &range, const RayIntersectionFilter &rFilter, const RayIntersectionKernel &rKernel, ShapeProcessingInfo spInfo, unsigned int tIdx) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE("method undefined");
	return false;
}

int Torus::processAllIntersections(const Ray3D &ray, const BoundingBox1D &range, const RayIntersectionFilter &rFilter, const RayIntersectionKernel &rKernel, ShapeProcessingInfo spInfo, unsigned int tIdx) const
{
	RayTracingStats::IncrementRayPrimitiveIntersectionNum();
	spInfo.material = _material;

	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	WARN_ONCE("method undefined");
	return 0;
}

bool Torus::isInside(Point3D p) const
{
	////////////////////////////////////////////////////////
	// Determine if the point is inside the cylinder here //
	////////////////////////////////////////////////////////
	WARN_ONCE("method undefined");
	return false;
}


void v_store(std::vector<float>* vertices, float r, float c, int i, int j, int n, int m)
{
	float theta = 2 * M_PI * i / n;
	float phi = 2 * M_PI * j / m;
	float x = (r + c * cos(theta)) * cos(phi);
	float y = (r + c * cos(theta)) * sin(phi);
	float z = c * sin(theta);
	vertices->push_back(x);
	vertices->push_back(y);
	vertices->push_back(z);
}


void Torus::drawOpenGL(GLSLProgram *glslProgram) const
{
	glEnable(GL_TEXTURE_2D);
	const int num_slices = OpenGLTessellationComplexity+4;

	// std::vector<float> *vertices = new std::vector<float>();

	// //store the vertices of the torus
	// for (int i = 0; i < num_slices; i++)
	// {
	// 	for (int j = 0; j < num_slices; j++)
	// 	{
	// 		v_store(vertices, iRadius, oRadius, i, j, num_slices, num_slices);
	// 		v_store(vertices, iRadius, oRadius, i + 1, j, num_slices, num_slices);
	// 		v_store(vertices, iRadius, oRadius, i, j + 1, num_slices, num_slices);

	// 		v_store(vertices, iRadius, oRadius, i, j + 1, num_slices, num_slices);
	// 		v_store(vertices, iRadius, oRadius, i + 1, j, num_slices, num_slices);
	// 		v_store(vertices, iRadius, oRadius, i + 1, j + 1, num_slices, num_slices);
	// 	}
	// }
	// GLuint torus_VBO;
	// glGenBuffers(1, &torus_VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, torus_VBO);
	// glBufferData(GL_ARRAY_BUFFER, vertices->size()*sizeof(float), vertices->data(), GL_STATIC_DRAW);
	
	// glEnableClientState(GL_VERTEX_ARRAY);

	// glVertexPointer(3, GL_FLOAT, 8*sizeof(float), 0);

	// glDrawArrays(GL_TRIANGLES, 0, vertices->size()/3);

	// glDisableClientState(GL_VERTEX_ARRAY);
	
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// delete vertices;

	glEnable(GL_DEPTH_TEST);

   int i, j, k;
   double s, t, x, y, z;


   for (i = 0; i < num_slices; i++) {
      glBegin(GL_QUAD_STRIP);
      for (j = 0; j <= num_slices; j++) {
         for (k = 1; k >= 0; k--) {
            s = (i + k) % num_slices + 0.5;
            t = j % num_slices;

            x = (oRadius + iRadius * cos(s * 2*Pi / num_slices)) * cos(t * 2*Pi / num_slices);
            z = (oRadius + iRadius * cos(s * 2*Pi / num_slices)) * sin(t * 2*Pi / num_slices);
            y = iRadius * sin(s * 2*Pi / num_slices);
            glVertex3f(x, y, z);
         }
      }
      glEnd();
   }

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();
}
