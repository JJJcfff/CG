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
	double d = (iInfo.position - _location).length();
	Point3D attenuation = _ambient / ( _constAtten + _linearAtten * d + _quadAtten * d * d );
	return attenuation * material.ambient;
}

Point3D PointLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D n = iInfo.normal;
	Point3D l = (_location - iInfo.position).unit();
	double d = (iInfo.position - _location).length();
	Point3D attenuation = _diffuse / ( _constAtten + _linearAtten * d + _quadAtten * d * d );
	return attenuation * material.diffuse * std::max(0.0, n.dot(l));
}

Point3D PointLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D n = iInfo.normal;
	Point3D l = (_location - iInfo.position).unit();
	if (iInfo.normal.dot(l) < 0) {
		return Point3D(0,0,0);
	}
	Point3D v = (ray.position - iInfo.position).unit();
	Point3D r = (2 * n.dot(l) * n - l).unit();
	double d = (iInfo.position - _location).length();
	Point3D attenuation = _specular / ( _constAtten + _linearAtten * d + _quadAtten * d * d );
	return attenuation * material.specular * std::pow(std::max(0.0, r.dot(v)), material.specularFallOff);
}

bool PointLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape &shape , unsigned int tIdx ) const
{
	Point3D l = (_location - iInfo.position).unit();
	Ray3D shadowRay = Ray3D(iInfo.position, l);
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
		return true;
	};
	return shape.processFirstIntersection(shadowRay, BoundingBox1D(Epsilon, (_location - iInfo.position).length()), rFilter, rKernel, spInfo, tIdx);
}

Point3D PointLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , unsigned int samples , unsigned int tIdx ) const
{
	Ray3D shadowray;
	Point3D t(1.0,1.0,1.0);
	shadowray.position = iInfo.position;
	shadowray.direction = (_location - iInfo.position).unit();
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
		t = spInfo.material->transparent;
		if (t[0] < cLimit[0] && t[1] < cLimit[1] && t[2] < cLimit[2]) return false;
		return true;
	};
	shape.processAllIntersections(shadowray, BoundingBox1D(Epsilon, Infinity), rFilter, rKernel, spInfo, tIdx);
	return t;
}

void PointLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
