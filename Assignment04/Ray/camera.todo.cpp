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
	glTranslatef( center[0] , center[1] , center[2] );
	glRotatef( angle*180/Pi , up[0] , up[1] , up[2] );
	glTranslatef( -center[0] , -center[1] , -center[2] );
}

void Camera::rotateRight( Point3D center , float angle )
{
	glTranslatef( center[0] , center[1] , center[2] );
	glRotatef( angle*180/Pi , right[0] , right[1] , right[2] );
	glTranslatef( -center[0] , -center[1] , -center[2] );
}

void Camera::moveForward( float dist )
{
	position += (forward.unit()) * dist;
}

void Camera::moveRight( float dist )
{
	position += (right.unit()) * dist;
}

void Camera::moveUp( float dist )
{
	position += (up.unit()) * dist;
}
