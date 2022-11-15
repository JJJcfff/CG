#include <algorithm>
#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include <fstream>
#include <iostream>
#include <cmath>


using namespace Util;
using namespace Image;

//helper functions


unsigned char uchar( double x ) {
  return (unsigned char) std::min(255, std::max(0, int(x)));
}

Pixel32 get_pix (int x, int y, const Image32& img) {
  if (x < 0 || x >= img.width() || y < 0 || y >= img.height()) {
    return Pixel32();
  }
  return img(x,y);
}

/////////////
// Image32 //
/////////////
Image32 Image32::addRandomNoise( double noise ) const
{
	Image32 result(*this);
  Image32::iterator it = result.begin();
	for(; it != result.end(); ++it){
    int r_noise = ((rand()%512)-256)*noise;
		it._p->r = uchar(it._p->r + r_noise);
    int g_noise = ((rand()%512)-256)*noise;
    it._p->g = uchar(it._p->g + g_noise);
    int b_noise = ((rand()%512)-256)*noise;
    it._p->b = uchar(it._p->b + b_noise);
	}
	return result;
}

Image32 Image32::brighten( double brightness ) const
{
  Image32 result(*this);
  Image32::iterator it = result.begin();
  for(; it != result.end(); ++it){
    it._p->r = uchar(it._p->r*brightness);
    it._p->g = uchar(it._p->g*brightness);
    it._p->b = uchar(it._p->b*brightness);
  }
  return result;
}

Image32 Image32::luminance( void ) const
{
  Image32 result(*this);
  Image32::iterator it = result.begin();
  for(; it != result.end(); ++it){
    it._p->r = it._p->g = it._p->b = (it._p->r+it._p->g+it._p->b)/3;
  }
  return result;
}

Image32 Image32::contrast( double contrast ) const
{
  Image32 result(*this);
  int l_sum;
  for(Image32::iterator it = result.begin(); it != result.end(); ++it){
    l_sum += (int)(0.3*(it._p->r) + 0.59*(it._p->g) + 0.11*(it._p->b));
  }
  int l_avg = l_sum/(_width*_height);
  for(Image32::iterator it = result.begin(); it != result.end(); ++it){
    it._p->r = uchar((it._p->r - l_avg)*contrast + l_avg);
    it._p->g = uchar((it._p->g - l_avg)*contrast + l_avg);
    it._p->b = uchar((it._p->b - l_avg)*contrast + l_avg);
  }
  return result;
}


Image32 Image32::saturate( double saturation ) const
{
  Image32 result(*this);
  Image32::iterator it = result.begin();
  for(; it != result.end(); ++it){
    int l = (int)(0.3*(it._p->r) + 0.59*(it._p->g) + 0.11*(it._p->b));
    it._p->r = uchar((it._p->r - l)*saturation + l);
    it._p->g = uchar((it._p->g - l)*saturation + l);
    it._p->b = uchar((it._p->b - l)*saturation + l);
  }
  return result;
}

Image32 Image32::quantize( int bits ) const
{
  Image32 result{*this};
  int p = pow(2,bits);
  for (iterator it = result.begin(); it != result.end(); ++it) {
    it._p->r = std::min(255,int(floor(double(it._p->r/255.0)*p))*(255/(p-1)));
    it._p->g = std::min(255,int(floor(double(it._p->g/255.0)*p))*(255/(p-1)));
    it._p->b = std::min(255,int(floor(double(it._p->b/255.0)*p))*(255/(p-1)));
  }
  return result;
}

Image32 Image32::randomDither( int bits ) const
{
	Image32 noise(*this);
  double p = pow(2,bits);
  Image32 result(noise.addRandomNoise(1.0/p).quantize(bits));
  return result;
}

Image32 Image32::orderedDither2X2( int bits ) const
{
  Image32 result(*this);
  int p = pow(2,bits);
  int x,y,count=0;
  int dither[2][2] = {{1,3},{4,2}};
  for (iterator it = result.begin(); it != result.end(); ++it) {
    x = count % _width;
    y = count / _width;
    int i = x % 2;
    int j = y % 2;
    double c_r = double(it._p->r)/255.0 * (p - 1);
    double c_g = double(it._p->g)/255.0 * (p - 1);
    double c_b = double(it._p->b)/255.0 * (p - 1);
    double e_r = c_r - floor(c_r);
    double e_g = c_g - floor(c_g);
    double e_b = c_b - floor(c_b);
    it._p->r = (e_r > ((dither[i][j]) / 5.0)) ? ceil(c_r)*(255/(p-1)) : floor(c_r)*(255/(p-1));
    it._p->g = (e_g > ((dither[i][j]) / 5.0)) ? ceil(c_g)*(255/(p-1)) : floor(c_g)*(255/(p-1));
    it._p->b = (e_b > ((dither[i][j]) / 5.0)) ? ceil(c_b)*(255/(p-1)) : floor(c_b)*(255/(p-1));
    count++;
  }
  return result;
}

Image32 Image32::floydSteinbergDither( int bits ) const
{
  Image32 result(*this);
  int p = pow(2,bits);
  double a[2][3]= {{       0,        0, 7.0/16.0},
                   {3.0/16.0, 5.0/16.0, 1.0/16.0}};
  iterator it = result.begin();
  int x,y,count=0;
  for (; it != result.end(); ++it) {
		x = count % _width;
		y = count / _width;
		double e_r  = it._p->r - std::min(255,int(floor(double(it._p->r/255.0)*p))*(255/(p-1)));
		double e_g  = it._p->g - std::min(255,int(floor(double(it._p->g/255.0)*p))*(255/(p-1)));
		double e_b  = it._p->b - std::min(255,int(floor(double(it._p->b/255.0)*p))*(255/(p-1)));
		for(int i=0; i<2; i++){
			for (int j=-1; j<=1; j++){
				if (x+j>=0 && x+j<_width && y+i<_height){
          result(x+j,y+i).r = std::min(255,std::max(0,int(result(x+j,y+i).r + e_r*a[i][j+1])));
          result(x+j,y+i).g = std::min(255,std::max(0,int(result(x+j,y+i).g + e_g*a[i][j+1])));
          result(x+j,y+i).b = std::min(255,std::max(0,int(result(x+j,y+i).b + e_b*a[i][j+1])));
				}
			}
		}
    it._p->r = std::min(255,int(floor(double(it._p->r/255.0)*p))*(255/(p-1)));
    it._p->g = std::min(255,int(floor(double(it._p->g/255.0)*p))*(255/(p-1)));
    it._p->b = std::min(255,int(floor(double(it._p->b/255.0)*p))*(255/(p-1)));
		count++;	
  }
  return result;
}

Image32 Image32::blur3X3( void ) const
{
	Image32 result(*this);
	int x,y,count=0;
  for (iterator it = result.begin(); it != result.end(); ++it) {
	x = count % _width;
	y = count / _width;
	int r=0,g=0,b=0;
	double filter [3][3] = {{1.0,2.0,1.0},
                          {2.0,4.0,2.0},
                          {1.0,2.0,1.0}};
	bool accessed[3][3] = {{false,false,false},
                         {false,false,false},
                         {false,false,false}};
	for (int i = -1; i <= 1; i++) {
	  for (int j = -1; j <= 1; j++) {
			if (x+i >= 0 && x+i < _width && y+j >= 0 && y+j < _height) {
        r += (*this)(x+i,y+j).r * filter[i+1][j+1]/16;
        g += (*this)(x+i,y+j).g * filter[i+1][j+1]/16;
        b += (*this)(x+i,y+j).b * filter[i+1][j+1]/16;
				accessed[i+1][j+1] = true;
			} 
	  }
	}
	for (int k = -1; k <= 1; k++) {
		for (int l = -1; l <= 1; l++) {
			if (!accessed[k+1][l+1]) {
				r += int(get_pix(x,y,*this).r * filter[-k+1][-l+1]/16);
        g += int(get_pix(x,y,*this).g * filter[-k+1][-l+1]/16);
        b += int(get_pix(x,y,*this).b * filter[-k+1][-l+1]/16);
			}
		}
	}
	it._p->r = r;
	it._p->g = g;
	it._p->b = b;
	count++;
  }
	return result;
}

Image32 Image32::edgeDetect3X3( void ) const
{
	Image32 result(*this);
  Image32 temp(*this);
	int x,y,diff=0,count=0;
  double filter [3][3] = {{-1.0/8,-1.0/8,-1.0/8},
                          {-1.0/8,   1.0,-1.0/8},
                          {-1.0/8,-1.0/8,-1.0/8}};
  for (iterator it = result.begin(); it != result.end(); ++it) {
    x = count % _width;
    y = count / _width;
    double r=0,g=0,b=0;
    bool accessed[3][3] = {{false,false,false},
                           {false,false,false},
                           {false,false,false}};
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (x+i >= 0 && x+i < _width && y+j >= 0 && y+j < _height) {
          r += get_pix(x+i,y+j,*this).r * filter[i+1][j+1];
          g += get_pix(x+i,y+j,*this).g * filter[i+1][j+1];
          b += get_pix(x+i,y+j,*this).b * filter[i+1][j+1];
          accessed[i+1][j+1] = true;
        }
      }
    }
    for (int k = -1; k <= 1; k++) {
      for (int l = -1; l <= 1; l++) {
        if (!accessed[k+1][l+1]) {
            r += int(get_pix(x,y,*this).r * filter[-k+1][-l+1]);
            g += int(get_pix(x,y,*this).g * filter[-k+1][-l+1]);
            b += int(get_pix(x,y,*this).b * filter[-k+1][-l+1]);
        }
      }
    }
    int alpha = 40;
    it._p->r = uchar(r*(255-abs(r))/alpha);
    it._p->g = uchar(g*(255-abs(g))/alpha);
    it._p->b = uchar(b*(255-abs(b))/alpha);
    count++;
  }
	return result;
}

Image32 Image32::scaleNearest( double scaleFactor ) const
{
  Image32 result(*this);
  int new_width = int(_width*scaleFactor);
  int new_height = int(_height*scaleFactor);
  result.setSize(new_width,new_height);
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i/scaleFactor;
      double y = j/scaleFactor;
      Point2D p(x,y);
      result(i,j) = nearestSample(p);
    }
  }
  return result;
}

Image32 Image32::scaleBilinear( double scaleFactor ) const
{
  Image32 result(*this);
  int new_width = int(_width*scaleFactor);
  int new_height = int(_height*scaleFactor);
  result.setSize(new_width,new_height);
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i/scaleFactor;
      double y = j/scaleFactor;
      Point2D p(x,y);
      result(i,j) = bilinearSample(p);
    }
  }
  return result;
}

Image32 Image32::scaleGaussian( double scaleFactor ) const
{
	Image32 result(*this);
  int new_width = int(_width*scaleFactor);
  int new_height = int(_height*scaleFactor);
  result.setSize(new_width,new_height);
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i/scaleFactor;
      double y = j/scaleFactor;
      result(i,j) = gaussianSample(Point2D(x,y),0.5,5);
    }
  }
  return result;
}



Image32 Image32::rotateNearest( double angle ) const
{
  double rad = angle * M_PI / 180.0;
	Image32 result(*this);
  int new_width = int(_width*abs(cos(rad))+_height*abs(sin(rad)));
  int new_height = int(_width*abs(sin(rad))+_height*abs(cos(rad)));
  result.setSize(new_width,new_height);
  int center_x = _width/2;
  int center_y = _height/2;
  int new_center_x = new_width/2;
  int new_center_y = new_height/2;
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i-new_center_x;
      double y = j-new_center_y;
      double new_x = x*cos(rad)-y*sin(rad)+center_x;
      double new_y = x*sin(rad)+y*cos(rad)+center_y;
      Point2D p(new_x,new_y);
      result(i,j) = nearestSample(p);
    }
  }
	return result;
}


Image32 Image32::rotateBilinear( double angle ) const
{
	double rad = angle * M_PI / 180.0;
	Image32 result(*this);
  int new_width = int(_width*abs(cos(rad))+_height*abs(sin(rad)));
  int new_height = int(_width*abs(sin(rad))+_height*abs(cos(rad)));
  result.setSize(new_width,new_height);
  int center_x = _width/2;
  int center_y = _height/2;
  int new_center_x = new_width/2;
  int new_center_y = new_height/2;
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i-new_center_x;
      double y = j-new_center_y;
      double new_x = x*cos(rad)-y*sin(rad)+center_x;
      double new_y = x*sin(rad)+y*cos(rad)+center_y;
      Point2D p(new_x,new_y);
      result(i,j) = bilinearSample(p);
    }
  }
	return result;
}

Image32 Image32::rotateGaussian( double angle ) const
{
	double rad = angle * M_PI / 180.0;
	Image32 result(*this);
  int new_width = int(_width*abs(cos(rad))+_height*abs(sin(rad)));
  int new_height = int(_width*abs(sin(rad))+_height*abs(cos(rad)));
  result.setSize(new_width,new_height);
  int center_x = _width/2;
  int center_y = _height/2;
  int new_center_x = new_width/2;
  int new_center_y = new_height/2;
  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      double x = i-new_center_x;
      double y = j-new_center_y;
      double new_x = x*cos(rad)-y*sin(rad)+center_x;
      double new_y = x*sin(rad)+y*cos(rad)+center_y;
      Point2D p(new_x,new_y);
      result(i,j) = gaussianSample(p,0.5,6);
    }
  }
	return result;
}

void Image32::setAlpha( const Image32& matte ){}

Image32 Image32::composite( const Image32& overlay ) const{ return Image32(); }

Image32 Image32::CrossDissolve( const Image32& source , const Image32& destination , double blendWeight )
{
  Image32 result;
  result.setSize(source.width(),source.height());
  int x, y, count = 0;
  for (iterator it = result.begin(); it != result.end(); it++) {
    x = count % result.width();
    y = count / result.width();
    it._p->r = uchar(double(get_pix(x,y,source).r)*blendWeight + double(get_pix(x,y,destination).r)*(1-blendWeight));
    it._p->g = uchar(double(get_pix(x,y,source).g)*blendWeight + double(get_pix(x,y,destination).g)*(1-blendWeight));
    it._p->b = uchar(double(get_pix(x,y,source).b)*blendWeight + double(get_pix(x,y,destination).b)*(1-blendWeight));
    count++;
  }
  return result;
}

Image32 Image32::warp( const OrientedLineSegmentPairs& olsp ) const
{
  Image32 result;
  result.setSize(_width,_height);
  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      Point2D p(x,y);
      Point2D new_p = olsp.getSourcePosition(p);
      result(x,y) = bilinearSample(Point2D{new_p[0],new_p[1]});
    }
  }
  return result;
}

Image32 Image32::funFilter( void ) const
{
	Image32 result(*this);
  double center_x = _width/2, center_y = _height/2, r = _height<_width? _height/2: _width/2;
  for(int x = 0; x < _width; x++){
    for(int y = 0; y < _height; y++){
      double dx = double(x - center_x), dy = double(y - center_y), angle = atan2(dy, dx), dist = sqrt(pow(dx,2) + pow(dy,2));
      if(1.0 - (dist / r) > 0) {
        angle += 2.5 * (1.0 - (dist / r));
        dx = cos(angle) * dist;
        dy = sin(angle) * dist;
      }
      result(x,y) = gaussianSample(Point2D(dx+center_x,dy+center_y),0.5,5);
    }
  }
  return result;
}

Image32 Image32::crop( int x1 , int y1 , int x2 , int y2 ) const
{
	Image32 result(*this);
	Image32 old(*this);
	result.setSize(x2-x1,y2-y1);
	iterator i_r = result.begin();
	int count = 0,x,y;
	for (iterator it = old.begin(); it != old.end(); ++it) {
		x = count % old._width;
		y = count / old._width;
		if (x >= x1 && x < x2 && y >= y1 && y < y2) {
			i_r._p->r = it._p->r;
			i_r._p->g = it._p->g;
			i_r._p->b = it._p->b;
			++i_r;
		}		
		count++;
	}
	return result;
}




Pixel32 Image32::nearestSample( Point2D p ) const
{
  int x = int(p[0]+0.5);
  int y = int(p[1]+0.5);
	return get_pix(x,y,*this);
}

Pixel32 Image32::bilinearSample( Point2D p ) const
{
  int x1 = floor(p[0]);
  int y1 = floor(p[1]);
  int x2 = x1+1;
  int y2 = y1+1;
  double x_weight = p[0] - x1;
  double y_weight = p[1] - y1;
  Pixel32 p1 = get_pix(x1,y1,*this);
  Pixel32 p2 = get_pix(x2,y1,*this);
  Pixel32 p3 = get_pix(x1,y2,*this);
  Pixel32 p4 = get_pix(x2,y2,*this);
  Pixel32 result;
  result.r = (1-x_weight)*(1-y_weight)*p1.r + x_weight*(1-y_weight)*p2.r + (1-x_weight)*y_weight*p3.r + x_weight*y_weight*p4.r;
  result.g = (1-x_weight)*(1-y_weight)*p1.g + x_weight*(1-y_weight)*p2.g + (1-x_weight)*y_weight*p3.g + x_weight*y_weight*p4.g;
  result.b = (1-x_weight)*(1-y_weight)*p1.b + x_weight*(1-y_weight)*p2.b + (1-x_weight)*y_weight*p3.b + x_weight*y_weight*p4.b;
  return result;
}


double get_g(const double variance, const Point2D& center, const int x, const int y)  {
 return  (1.0 / (2.0 * M_PI * variance)) * exp( -(pow(x-center[0],2) + pow(y-center[1],2)) / (2 * variance) );
}

Pixel32 Image32::gaussianSample( Point2D p , double variance , double radius ) const
{
  double sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0, normal = 0;
	int left_bound = p[0] - ceil(radius);
  int right_bound = p[0] + ceil(radius);
  int top_bound = p[1] - ceil(radius);
  int bottom_bound = p[1] + ceil(radius);
  for (int j = top_bound; j <= bottom_bound; j++) {
    for (int i = left_bound; i <= right_bound; i++) {
      if (pow(i-p[0],2)+pow(j-p[1],2) <= pow(radius,2)) {
        double gaussian = get_g(variance, p, i, j);
        if (i >= 0 && i < _width && j >= 0 && j < _height){
          sum_r += gaussian * get_pix(i,j,*this).r;
          sum_g += gaussian * get_pix(i,j,*this).g;
          sum_b += gaussian * get_pix(i,j,*this).b;
          sum_a += gaussian * get_pix(i,j,*this).a;
        }
        normal += gaussian;
      }
    }
  }
  Pixel32 result;
  result.r = uchar(sum_r / normal);
  result.g = uchar(sum_g / normal);
  result.b = uchar(sum_b / normal);
  result.a = uchar(sum_a / normal);
  return result;
}


