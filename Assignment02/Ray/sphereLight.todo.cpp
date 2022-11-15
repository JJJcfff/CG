#include <cmath>
#include <random>
#include <Util/exceptions.h>
#include "scene.h"
#include "sphereLight.h"

using namespace Ray;
using namespace Util;

/////////////////
// SphereLight //
/////////////////
Point3D SphereLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit , unsigned int samples , unsigned int tIdx ) const
{
	// std::random_device rd;
	// std::default_random_engine gen(rd());
	// std::uniform_real_distribution<double> dis(-1, 1);
	// Point3D t(1.0, 1.0, 1.0);
	// for (int i = 0; i < samples; i++) {
	// 	Ray3D r;
	// 	double x = dis(gen);
	// 	double y = dis(gen);
	// 	double z = dis(gen);
	// 	if (x*x + y*y + z*z > 1) {
	// 		i--;
	// 		continue;
	// 	}
	// 	Point3D p = Point3D(x,y,z).unit()*_radius + _location;
	// 	r.position = iInfo.position;
	// 	r.direction = (p - r.position).unit();
	// 	Scene::ShapeProcessingInfo spInfo;
	// 	Scene::RayIntersectionFilter rFilter = [](double) { return true; };
	// 	Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
	// 	t *= spInfo.material->transparent;
	// 	if (t[0] < cLimit[0] || t[1] < cLimit[1] || t[2] < cLimit[2]) return false;
	// 	return true;
	// 	};
	// 	shape.processAllIntersections(r, BoundingBox1D(Epsilon, Infinity), rFilter, rKernel, spInfo, tIdx);
	// }
	// return t/(double)samples;
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_real_distribution<double> dis(-1, 1);
	Point3D sum_t(0.0, 0.0, 0.0);
	for (int i = 0; i < samples; i++) {
		Ray3D r;
		double x = dis(gen);
		double y = dis(gen);
		double z = dis(gen);
		if (x*x + y*y + z*z > 1) {
			i--;
			continue;
		}
		Point3D p = Point3D(x,y,z).unit()*_radius + _location;
		r.position = iInfo.position;
		r.direction = (p - iInfo.position).unit();
		Point3D t{ 1.0,1.0,1.0 };
		Scene::ShapeProcessingInfo spInfo;
		Scene::RayIntersectionFilter rFilter = [](double) { return true; };
		Scene::RayIntersectionKernel rKernel = [&](const Scene::ShapeProcessingInfo& spInfo, const RayShapeIntersectionInfo& _iInfo){
			t *= spInfo.material->transparent;
			if (t[0] < cLimit[0] || t[1] < cLimit[1] || t[2] < cLimit[2]) {
				return true;
			}
			return false;
		};
		shape.processAllIntersections(r, BoundingBox1D(Epsilon, (p - iInfo.position).length()), rFilter, rKernel, spInfo, tIdx);
		sum_t += t;
  }
  return sum_t / (double)samples;	
}