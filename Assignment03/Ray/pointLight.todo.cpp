#include <cmath>
#include <Util/exceptions.h>
#include "pointLight.h"
#include "scene.h"

using namespace Ray;
using namespace Util;

////////////////
// PointLight //
////////////////

Point3D PointLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo & iInfo , const Material &material ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return Point3D();
}

Point3D PointLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return Point3D();
}

Point3D PointLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return Point3D();
}

bool PointLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape &shape , unsigned int tIdx ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

Point3D PointLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , unsigned int samples , unsigned int tIdx ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return Point3D( 1. , 1. , 1. );
}

void PointLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	GLfloat pos[4] = { (GLfloat)_location[0], (GLfloat)_location[1], (GLfloat)_location[2], 0.0f };
	GLfloat amb[4] = {  (GLfloat)_ambient[0],    (GLfloat)_ambient[1],    (GLfloat)_ambient[2], 1.0f };
	GLfloat dif[4] = {  (GLfloat)_diffuse[0],    (GLfloat)_diffuse[1],    (GLfloat)_diffuse[2], 1.0f };
	GLfloat spe[4] = {  (GLfloat)_specular[0],   (GLfloat)_specular[1],   (GLfloat)_specular[2], 1.0f };
	glLightfv(GL_LIGHT0 + index, GL_POSITION, pos);
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, spe);
	glLightf(GL_LIGHT0 + index, GL_CONSTANT_ATTENUATION, (GLfloat)_constAtten);
	glLightf(GL_LIGHT0 + index, GL_LINEAR_ATTENUATION, (GLfloat)_linearAtten);
	glLightf(GL_LIGHT0 + index, GL_QUADRATIC_ATTENUATION, (GLfloat)_quadAtten);
	glEnable(GL_LIGHT0 + index);
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

