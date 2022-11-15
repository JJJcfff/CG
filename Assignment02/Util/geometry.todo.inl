/*
Copyright (c) 2019, Michael Kazhdan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

namespace Util
{
	////////////
	// Matrix //
	////////////
	template< unsigned int Dim >
	Matrix< Dim , Dim > Matrix< Dim , Dim >::Exp( const Matrix &m , int terms )
	{
		//////////////////////////////////////
		// Compute the matrix exponent here //
		//////////////////////////////////////
		WARN_ONCE( "method undefined" );
		return Matrix();
	}

	template< unsigned int Dim >
	Matrix< Dim , Dim > Matrix< Dim , Dim >::closestRotation( void ) const
	{
		///////////////////////////////////////
		// Compute the closest rotation here //
		///////////////////////////////////////
		WARN_ONCE( "method undefined" );
		return Matrix();
	}

	/////////////////
	// BoundingBox //
	/////////////////
	template< unsigned int Dim >
	BoundingBox< 1 > BoundingBox< Dim >::intersect( const Ray< Dim > &ray ) const{
	
		double tmin = -Infinity;
		double tmax = Infinity;
		for (int i = 0; i < Dim; i++) {
			double t1 = (_p[0][i] - ray.position[i]) / ray.direction[i];
			double t2 = (_p[1][i] - ray.position[i]) / ray.direction[i];
			if (t1 != t2){
				tmin = std::max(tmin, std::min(t1, t2));
				tmax = std::min(tmax, std::max(t1, t2));
			}
		}
		if (tmin < 0 && tmax < 0) return BoundingBox<1>();
		else if (tmin > tmax) return BoundingBox<1>();
		else return BoundingBox<1>(Point1D(tmin), Point1D(tmax));
	}

	/////////////
	// Quadric //
	/////////////
	template< unsigned int Dim >
	Quadric< 1 > Quadric< Dim >::intersect( const Ray< Dim > &ray ) const
	{
		WARN_ONCE( "method undefined" );
		return Quadric<1>();
	}
}