#include <Util/exceptions.h>
#include <Util/threads.h>
#include "shapeList.h"
#include "triangle.h"

using namespace Ray;
using namespace Util;

////////////////
// Difference //
////////////////
void Difference::updateBoundingBox( void )
{
	_shape0->updateBoundingBox();
	_shape1->updateBoundingBox();
	_bBox = _shape0->boundingBox() + _shape1->boundingBox();
}

bool Difference::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{}

int Difference::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	//////////////////////////////////////////////////////////////////
	// Compute the intersection of the difference with the ray here //
	//////////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return 0;
}
bool Difference::isInside( Util::Point3D p ) const
{
	//////////////////////////////////////////////////////////
	// Determine if the point is inside the difference here //
	//////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

///////////////
// ShapeList //
///////////////
bool ShapeList::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	bool ret = false;
	RayIntersectionKernel rk;
	RayShapeIntersectionInfo info;
	info.t = Infinity;
	rk = [&,this](const ShapeProcessingInfo& _spInfo, const RayShapeIntersectionInfo& _iInfo){
		if (_iInfo < info && range.isInside(Point1D(_iInfo.t)) && rFilter(_iInfo.t)) {
			info = _iInfo;
			spInfo = _spInfo;
			ret = true;
		}
		return true;
	};

	std::vector<ShapeBoundingBoxHit> hits;
	for (const Shape* s : shapes) {
		ShapeBoundingBoxHit hit;
		BoundingBox1D intersectionRange = s->boundingBox().intersect(ray);
		if (intersectionRange.isEmpty() || range[1][0] < intersectionRange[0][0]) continue;
		hit.shape = s;
		hit.t = intersectionRange[0][0] > 0 ? intersectionRange[0][0] : intersectionRange[1][0];
		hits.push_back(hit);
	}
	std::sort(hits.begin(), hits.end(), ShapeBoundingBoxHit::Compare);
	for (ShapeBoundingBoxHit hit : hits) {
		hit.shape->processFirstIntersection(ray, range, rFilter, rk, spInfo, tIdx);
	}

	if (ret) {
		rKernel(spInfo, info);
	}
	return ret;
}

int ShapeList::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	int count = 0;
	bool terminate = false;
	RayIntersectionKernel rk;
	rk = [&,this](const ShapeProcessingInfo& _spInfo, const RayShapeIntersectionInfo& _iInfo){
		terminate = !rKernel(_spInfo, _iInfo);
		return true;
	};

	std::vector<ShapeBoundingBoxHit> hits;
	for (const Shape* s : shapes) {
		ShapeBoundingBoxHit hit;
		BoundingBox1D intersectionRange = s->boundingBox().intersect(ray);
		if (intersectionRange.isEmpty() || (range^intersectionRange).isEmpty()) continue;
		hit.shape = s;
		hit.t = intersectionRange[0][0];
		hits.push_back(hit);
	}
	std::sort(hits.begin(), hits.end(), ShapeBoundingBoxHit::Compare);
	for (ShapeBoundingBoxHit hit : hits) {
		count += hit.shape->processAllIntersections(ray, range, rFilter, rk, spInfo, tIdx);
		if (terminate) break;
	}
	return count;
}


bool ShapeList::isInside( Point3D p ) const
{
	for (const Shape* s : shapes) {
		if (s->isInside(p)) return true;
	}
	return false;
}

void ShapeList::init( const LocalSceneData &data )
{
	// Initialize the children
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->init( data );
	_primitiveNum = 0;
	for( int i=0 ; i<shapes.size() ; i++ ) _primitiveNum += shapes[i]->primitiveNum();

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void ShapeList::updateBoundingBox( void )
{
	for (Shape* s : shapes) {
		s->updateBoundingBox();
		_bBox += s->boundingBox();
	}
}

void ShapeList::initOpenGL( void )
{
	// Initialize the children
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->initOpenGL();

	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "remainder of method undefined" );
}

void ShapeList::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

/////////////////
// AffineShape //
/////////////////

bool AffineShape::isInside( Point3D p ) const
{
	///////////////////////////////////////////////////////////////////////
	// Determine if the point is inside the affinely deformed shape here //
	///////////////////////////////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return _shape->isInside( p );
}

void AffineShape::updateBoundingBox( void )
{
	_shape->updateBoundingBox();
	_bBox = getMatrix() * _shape->boundingBox();
}

void AffineShape::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );
	_shape->drawOpenGL( glslProgram );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

///////////////////////
// StaticAffineShape //
///////////////////////
void StaticAffineShape::init( const LocalSceneData &data )
{
	_inverseTransform = getMatrix().inverse();
	_normalTransform = getMatrix().transpose().inverse();

	_shape->init( data );
	_primitiveNum = _shape->primitiveNum();
}

//////////////////
// TriangleList //
//////////////////

void TriangleList::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void TriangleList::init( const LocalSceneData &data )
{
	// Set the vertex and material pointers
	_vertices = &data.vertices[0];
	_vNum = (unsigned int)data.vertices.size();
	if( _materialIndex>=data.materials.size() ) THROW( "shape specifies a material that is out of bounds: " , _materialIndex , " <= " , data.materials.size() );
	else if( _materialIndex<0 ) THROW( "negative material index: " , _materialIndex );
	else _material = &data.materials[ _materialIndex ];

	ShapeList::init( data );

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void TriangleList::initOpenGL( void )
{
	ShapeList::initOpenGL();

	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

///////////
// Union //
///////////
bool Union::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayIntersectionFilter rf = [&](double t) {
		Point3D i_p = ray(t);
		int count = 0;
		for (Shape* s : _shapeList.shapes) {
			if (s->isInside(i_p)) count++;
			if (count > 1) return false;
		}
		return rFilter(t);
	};
	return _shapeList.processFirstIntersection(ray, range, rf, rKernel, spInfo, tIdx);
}

int Union::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
		RayIntersectionFilter rf = [&](double t) {
		Point3D i_p = ray(t);
		for (Shape* s : _shapeList.shapes) {
			if (this != s && s->isInside(i_p))return false;
		}
		return rFilter(t);
	};
	return _shapeList.processAllIntersections(ray, range, rf, rKernel, spInfo, tIdx);
}
void Union::init( const LocalSceneData &data )
{
	_shapeList.init( data );
	_primitiveNum = _shapeList.primitiveNum();

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Union::updateBoundingBox( void )
{
	for (Shape* s : _shapeList.shapes) {
		s->updateBoundingBox();
		_bBox += s->boundingBox();
	}
}

bool Union::isInside( Point3D p ) const
{
	int count = 0;
	for (const Shape* s : _shapeList.shapes) {
		if (s->isInside(p)) count ++;
		if (count > 1) return false;
	}
	return count == 1;
}

//////////////////
// Intersection //
//////////////////
bool Intersection::processFirstIntersection( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayIntersectionFilter rf = [&](double t) {
		Point3D i_p = ray(t);
		for (Shape* s : _shapeList.shapes) {
			if (this != s && !s->isInside(i_p)) return false;
		}
		return rFilter(t);
	};
	return _shapeList.processFirstIntersection(ray, range, rf, rKernel, spInfo, tIdx);
}

int Intersection::processAllIntersections( const Ray3D &ray , const BoundingBox1D &range , const RayIntersectionFilter &rFilter , const RayIntersectionKernel &rKernel , ShapeProcessingInfo spInfo , unsigned int tIdx ) const
{
	RayIntersectionFilter rf = [&](double t) {
		Point3D i_p = ray(t);
		for (Shape* s : _shapeList.shapes) {
			if (this != s && !s->isInside(i_p)) return false;
		}
		return rFilter(t);
	};
	return _shapeList.processAllIntersections(ray, range, rf, rKernel, spInfo, tIdx);
}
void Intersection::init( const LocalSceneData &data )
{
	_shapeList.init( data );
	_primitiveNum = _shapeList.primitiveNum();

	/////////////////////////////////// 
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}

void Intersection::updateBoundingBox( void )
{
	for (Shape* s : _shapeList.shapes) {
		s->updateBoundingBox();
		_bBox = _bBox ^ s->boundingBox();
	}
}

bool Intersection::isInside( Point3D p ) const
{
	for (const Shape* s : _shapeList.shapes) {
		if (!s->isInside(p)) return false;
	}
	return true;
}