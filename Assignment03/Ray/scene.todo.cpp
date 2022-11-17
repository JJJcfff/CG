#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"

using namespace Ray;
using namespace Util;

///////////
// Scene //
///////////
Point3D Scene::Reflect( Point3D v , Point3D n )
{
	//////////////////
	// Reflect here //
	//////////////////
	WARN_ONCE( "method undefined" );
	return Point3D();
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit , unsigned int lightSamples , unsigned int tIdx )
{
	Point3D color;
	RayTracingStats::IncrementRayNum();
	ShapeProcessingInfo spInfo;
	RayIntersectionFilter rFilter = []( double ){ return true; };
	RayIntersectionKernel rKernel = [&]( const ShapeProcessingInfo &spInfo , const RayShapeIntersectionInfo &_iInfo )
	{
		/////////////////////////////////////////////////////////
		// Create the computational kernel that gets the color //
		/////////////////////////////////////////////////////////
		WARN_ONCE( "method undefined" );
		color = Point3D( 0. , 1. , 0. );
		return true;
	};

	processFirstIntersection( ray , BoundingBox1D( Epsilon , Infinity ) , rFilter , rKernel , spInfo , tIdx );

	return color;
}

//////////////
// Material //
//////////////
void Material::drawOpenGL( GLSLProgram * glslProgram ) const
{
	GLfloat m_ambient[4]  = {(GLfloat) ambient[0] , (GLfloat)ambient[1]   , (GLfloat)ambient[2] , 1. };
	GLfloat m_diffuse[4]  = {(GLfloat) diffuse[0] , (GLfloat)diffuse[1]   , (GLfloat)diffuse[2] , 1. };
	GLfloat m_specular[4] = {(GLfloat) specular[0] ,(GLfloat) specular[1] , (GLfloat)specular[2] , 1. };
	GLfloat m_emission[4] = {(GLfloat) emissive[0] ,(GLfloat) emissive[1] , (GLfloat)emissive[2] , 1. };
	glMaterialfv( GL_FRONT_AND_BACK , GL_AMBIENT , m_ambient );
	glMaterialfv( GL_FRONT_AND_BACK , GL_DIFFUSE , m_diffuse );
	glMaterialfv( GL_FRONT_AND_BACK , GL_SPECULAR ,m_specular );
	glMaterialfv( GL_FRONT_AND_BACK , GL_EMISSION ,m_emission );
	glMaterialf( GL_FRONT_AND_BACK , GL_SHININESS , (GLfloat)specularFallOff);

	if (tex)
	{	
		glBindTexture(GL_TEXTURE_2D, tex->_openGLHandle);
	}
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

/////////////
// Texture //
/////////////
void Texture::initOpenGL( void )
{
	glGenTextures( 1 , &_openGLHandle );
	glBindTexture( GL_TEXTURE_2D , _openGLHandle );

	unsigned int width = _image.width();
	unsigned int height = _image.height();
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , width , height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , &_image( 0 , 0 ) );
	
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	glGenerateMipmap( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D , 0 );
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
