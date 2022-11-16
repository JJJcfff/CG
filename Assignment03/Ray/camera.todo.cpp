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
	gluLookAt(position[0], position[1], position[2], position[0] + forward[0], position[1] + forward[1], position[2] + forward[2], up[0], up[1], up[2]);
	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void Camera::rotateUp( Point3D center , float angle )
{
	//rotate the eye about the center by angle in the up direction

	
		WARN_ONCE( "method undefined" );



}

void Camera::rotateRight( Point3D center , float angle )
{
	//////////////////////////////////////////////////////
	// Rotate the camera about the right direction here //
	//////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Camera::moveForward( float dist )
{
		WARN_ONCE( "method undefined" );
gluLookAt(position[0]+dist, position[1], position[2], position[0] + forward[0] + dist, position[1] + forward[1], position[2] + forward[2], up[0], up[1], up[2]);
}

void Camera::moveRight( float dist )
{
		WARN_ONCE( "method undefined" );
gluLookAt(position[0], position[1], position[2]+dist, position[0] + forward[0], position[1] + forward[1], position[2] + forward[2]+dist, up[0], up[1], up[2]);

}

void Camera::moveUp( float dist )
{
		WARN_ONCE( "method undefined" );
gluLookAt(position[0], position[1]+dist, position[2], position[0] + forward[0], position[1] + forward[1] + dist, position[2] + forward[2], up[0], up[1], up[2]);
}
