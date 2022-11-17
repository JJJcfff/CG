#include <cmath>
#include <Util/exceptions.h>
#include "camera.h"
#include "shape.h"

using namespace Ray;
using namespace Util;

////////////
// Camera //
////////////

Ray3D Camera::getRay( int i , int j , int width , int height ) const
{
	/////////////////////////////////////////////////
	// Get the ray through the (i,j)-th pixel here //
	/////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return Ray3D();
}

void Camera::drawOpenGL( void ) const
{
	Point3D lookAt = position + forward;
	gluLookAt( position[0] , position[1] , position[2] , lookAt[0] , lookAt[1] , lookAt[2] , up[0] , up[1] , up[2] );
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void Camera::rotateUp( Point3D center , float angle )
{
	//glLoadIdentity();
	WARN_ONCE( "method undefined" );

}

void Camera::rotateRight( Point3D center , float angle )
{

	WARN_ONCE( "method undefined" );

}

void Camera::moveForward( float dist )
{
	if (dist != 0){
		//glLoadIdentity();
		position += (forward.unit()) * dist;
		Point3D lookAt = position + forward;
		gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);
	}
}

void Camera::moveRight( float dist )
{
	if (dist != 0){
		//glLoadIdentity();
		Point3D right = Point3D::CrossProduct(forward,up);
		position += (right.unit()) * dist;
		Point3D lookAt = position + forward;
		gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);
	}
}

void Camera::moveUp( float dist )
{
	if (dist != 0)
	{
		position += (up.unit()) * dist;
		Point3D lookAt = position + forward;
		gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);
	}
	
}
