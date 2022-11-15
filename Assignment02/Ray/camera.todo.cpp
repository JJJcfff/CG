#include <cmath>
#include <Util/exceptions.h>
#include "camera.h"
#include "shape.h"

using namespace Ray;
using namespace Util;

////////////
// Camera //
////////////
Ray3D Camera::getRay(int i, int j, int width, int height) const //TODO: MODIFY THE CODE so that it also works for rotations about other axes!!!!!!!!!!!!!!!!!!!!!!!!!
{
	Ray3D ray;
	double ar = (double)height / width; 
	double widthAngle = atan(tan(heightAngle) / ar); 

	Point3D p1 = position + forward - right * tan(widthAngle / 2) - up * tan(heightAngle / 2);
	Point3D p2 = position + forward - right * tan(widthAngle / 2) + up * tan(heightAngle / 2);
	Point3D p3 = position + forward + right * tan(widthAngle / 2) + up * tan(heightAngle / 2);

	Point3D p;
	p[0] = (p2 + ((double)i + 0.5) / width * (p3 - p2))[0];
	p[1] = (p1 + ((double)j + 0.5) / height * (p2 - p1))[1];
	p[2] = (p1 + ((double)j + 0.5) / height * (p2 - p1))[2];
	
	ray.position = position;
	ray.direction = (p - position).unit();
	return ray;
}

void Camera::drawOpenGL( void ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void Camera::rotateUp( Point3D center , float angle )
{
	///////////////////////////////////////////////////
	// Rotate the camera about the up direction here //
	///////////////////////////////////////////////////
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
	//////////////////////////////////
	// Move the camera forward here //
	//////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Camera::moveRight( float dist )
{
	///////////////////////////////////////
	// Move the camera to the right here //
	///////////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Camera::moveUp( float dist )
{
	/////////////////////////////
	// Move the camera up here //
	/////////////////////////////
	WARN_ONCE( "method undefined" );
}
