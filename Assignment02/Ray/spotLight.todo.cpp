#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "spotLight.h"

using namespace Ray;
using namespace Util;

///////////////
// SpotLight //
///////////////
Point3D SpotLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D direction = (_location - iInfo.position).unit();
	double distance = (_location - iInfo.position).length();
	Point3D attenuation = _ambient * _direction.dot(-direction) / (_constAtten + _linearAtten * distance + _quadAtten * distance * distance);
	return material.ambient * attenuation;
}

Point3D SpotLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D direction = (_location - iInfo.position).unit(); 
	double cosTheta = _direction.dot(-direction);
	if (cosTheta < cos(_cutOffAngle) || cosTheta < 0) return Point3D(0,0,0);
	double d = (iInfo.position - _location).length();
	Point3D attenuation = _diffuse * pow(cosTheta, _dropOffRate) / (_constAtten + _linearAtten * d + _quadAtten * d * d);
	return attenuation * material.diffuse * std::max(0.0, iInfo.normal.dot(direction));
}

Point3D SpotLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D direction = (_location - iInfo.position).unit();
	double cosTheta = _direction.dot(-direction);
	if (cosTheta < cos(_cutOffAngle) || cosTheta < 0) return Point3D(0,0,0);
	Point3D n = iInfo.normal;
	Point3D l = direction;
	Point3D v = (ray.position - iInfo.position).unit();
	Point3D r = (2 * n.dot(l) * n - l).unit();
	double d = (iInfo.position - _location).length();
	Point3D attenuation = _specular * pow(cosTheta, _dropOffRate) / (_constAtten + _linearAtten * d + _quadAtten * d * d);
	return attenuation * material.specular * std::pow(std::max(0.0, r.dot(v)), material.specularFallOff);
}

bool SpotLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape &shape , unsigned int tIdx ) const
{
	Ray3D shadowRay = Ray3D(iInfo.position, (_location - iInfo.position).unit());
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo)
	{
		return true;
	};
	return shape.processFirstIntersection(shadowRay, BoundingBox1D(Epsilon, (_location - iInfo.position).length()), rFilter, rKernel, spInfo, tIdx);
}

Point3D SpotLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , unsigned int samples , unsigned int tIdx ) const
{
	Ray3D r;
	Point3D t(1.0,1.0,1.0);
	r.position = iInfo.position;
	r.direction = (_location - iInfo.position).unit();
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
		t = spInfo.material->transparent;
		if (t[0] < cLimit[0] && t[1] < cLimit[1] && t[2] < cLimit[2]) return false;
		return true;
	};
	shape.processAllIntersections(r, BoundingBox1D(Epsilon,Infinity), rFilter, rKernel, spInfo, tIdx);
	return t;
}

void SpotLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
