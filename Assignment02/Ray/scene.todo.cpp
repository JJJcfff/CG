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
	return (v - 2 * v.dot(n) * n).unit();
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	double n1 = 1.0, n2 = ir;
	double cosi = v.dot(n);
	if( cosi > 0){
		std::swap(n1, n2);
		n = -n;
	}
	else {
		cosi = -cosi;
	}
	double a = n1 / n2;
	double k = 1 - a * a * (1 - cosi * cosi);
	if( k < 0 ) return false;
	refract = (a * v + (a * cosi - sqrt(k)) * n).unit();
	return true;
}

Point3D Texture::getPixel(int x, int y)const { //adapted from assignment 1
	if (x < 0 || x >= _image.width() || y < 0 || y >= _image.height()) {
		return Point3D(0, 0, 0);
	}
	else {
		return Point3D(_image(x, y).r, _image(x, y).g, _image(x, y).b);
	}
}


Point3D Texture::bilinearSample(Point2D p)const { // adapted from assignment 1
	p[0] *= _image.width();
	p[1] *= _image.height();
	int x1 = floor(p[0]);
  int y1 = floor(p[1]);
  int x2 = x1+1;
  int y2 = y1+1;
  double x_weight = p[0] - x1;
  double y_weight = p[1] - y1;
  Point3D p1 = getPixel(x1,y1);
  Point3D p2 = getPixel(x2,y1);
  Point3D p3 = getPixel(x1,y2);
  Point3D p4 = getPixel(x2,y2);
  Point3D result;
  result[0] = ((1-x_weight)*(1-y_weight)*p1[0] + x_weight*(1-y_weight)*p2[0] + (1-x_weight)*y_weight*p3[0] + x_weight*y_weight*p4[0])/255.0;
  result[1] = ((1-x_weight)*(1-y_weight)*p1[1] + x_weight*(1-y_weight)*p2[1] + (1-x_weight)*y_weight*p3[1] + x_weight*y_weight*p4[1])/255.0;
  result[2] = ((1-x_weight)*(1-y_weight)*p1[2] + x_weight*(1-y_weight)*p2[2] + (1-x_weight)*y_weight*p3[2] + x_weight*y_weight*p4[2])/255.0;
  return result;
}



Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit , unsigned int lightSamples , unsigned int tIdx )
{
	Point3D color;
	Ray3D reflect, refract;
	if (!rDepth || (cLimit[0] > 1 && cLimit[1] > 1 && cLimit[2] > 1)) return color;
	RayTracingStats::IncrementRayNum();
	ShapeProcessingInfo spInfo;
	RayIntersectionFilter rFilter = []( double ){ return true; };
	RayIntersectionKernel rKernel = [&]( const ShapeProcessingInfo &spInfo , const RayShapeIntersectionInfo &_iInfo )
	{
		RayShapeIntersectionInfo t_iInfo = _iInfo;
		t_iInfo.position = spInfo.localToGlobal * _iInfo.position;
		t_iInfo.normal = (spInfo.normalLocalToGlobal * _iInfo.normal).unit();

		for (Light* light : (_globalData.lights)) {			
			if (spInfo.material->tex) {
				Point3D b = spInfo.material->tex->bilinearSample(t_iInfo.texture);
				color += light->getAmbient(ray, t_iInfo, *spInfo.material);
				// color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*(light->isInShadow(t_iInfo, *this, tIdx));
				// color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*(1-light->isInShadow(t_iInfo, *this, tIdx));
				color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*light->transparency(t_iInfo, *this, cLimit, lightSamples, tIdx);
				color += spInfo.material->emissive;
				color *= b;
			}else {
				color += light->getAmbient(ray, t_iInfo, *spInfo.material);
				// color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*(light->isInShadow(t_iInfo, *this, tIdx));
				// color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*(1-light->isInShadow(t_iInfo, *this, tIdx));
				color += (light->getSpecular(ray, t_iInfo, *spInfo.material)+light->getDiffuse(ray, t_iInfo, *spInfo.material))*light->transparency(t_iInfo, *this, cLimit, lightSamples, tIdx);
				color += spInfo.material->emissive;
			}
		}
		if (ray.direction.dot(t_iInfo.normal) < 0) {
			reflect.direction = Reflect(ray.direction, t_iInfo.normal);
			reflect.position = t_iInfo.position + reflect.direction * Epsilon;
			color += getColor(reflect, rDepth - 1, cLimit / (spInfo.material->specular), lightSamples, tIdx) * spInfo.material->specular;
		}
		if (Refract(ray.direction.unit(), t_iInfo.normal.unit(), spInfo.material->ir, refract.direction)) {
			refract.position = t_iInfo.position + refract.direction * Epsilon;
			color += getColor(refract, rDepth - 1, cLimit / (spInfo.material->transparent), lightSamples, tIdx) * (spInfo.material->transparent);
		}
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
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

/////////////
// Texture //
/////////////
void Texture::initOpenGL( void )
{
	///////////////////////////////////
	// Do OpenGL texture set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

