#include <cmath>
#include <Util/exceptions.h>
#include "directionalLight.h"
#include "scene.h"

using namespace Ray;
using namespace Util;

//////////////////////
// DirectionalLight //
//////////////////////

Point3D DirectionalLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	return _ambient * material.ambient;
}

Point3D DirectionalLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D l = -_direction;
	return _diffuse * material.diffuse * std::max(iInfo.normal.dot(l),0.0);
}

Point3D DirectionalLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo &iInfo , const Material &material ) const
{
	Point3D l = -_direction;
	if (iInfo.normal.dot(l) < 0) {
		return Point3D(0,0,0);
	}
	Point3D v = (ray.position - iInfo.position).unit();
	Point3D r = (2 * iInfo.normal.dot(l) * iInfo.normal - l).unit();
	return _specular * material.specular * std::pow(std::max(0.0, r.dot(v)), material.specularFallOff);

}

bool DirectionalLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape &shape , unsigned int tIdx ) const
{
	Ray3D ShadowRay;
	ShadowRay.position = iInfo.position;
	ShadowRay.direction = -_direction;
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
		return true;
	};
	return shape.processFirstIntersection(ShadowRay, BoundingBox1D(Epsilon, Infinity), rFilter, rKernel, spInfo, tIdx);
}

Point3D DirectionalLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , unsigned int samples , unsigned int tIdx ) const
{
	Ray3D r;
	Point3D t(1.0,1.0,1.0);
	r.position = iInfo.position;
	r.direction = -_direction;
	Scene::ShapeProcessingInfo spInfo;
	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
		t *= spInfo.material->transparent;
		if (t[0] < cLimit[0] && t[1] < cLimit[1] && t[2] < cLimit[2]) return false;
		return true;
	};
	shape.processAllIntersections(r, BoundingBox1D(Epsilon, Infinity), rFilter, rKernel, spInfo, tIdx);
	return t;
}

void DirectionalLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
