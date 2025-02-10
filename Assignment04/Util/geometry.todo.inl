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
		///////////////////////////////////////
		// Compute the matrix exponential here //
		///////////////////////////////////////
		/*Implement the parametrization of rotations by the 3x3 skew-symmetric matrices.
		To do this, you will need to implement the function Matrix::Exp (in Util/geometry.todo.inl) which computes the exponential of a matrix using the first terms terms of the Taylor approximation. Keep in mind that the Taylor series for the exponential function is:
		exp(X)=1+X+X*X/(2!)+X*X*X/(3!)+...+X**k/(k!)+...

		where X**k represents X raised to the k-th power, and k! is "k factorial" -- k*(k-1)*(k-2)*...*2*1.*/
		Matrix<Dim, Dim> result = Matrix<Dim, Dim>::Identity();
		Matrix<Dim, Dim> temp = Matrix<Dim, Dim>::Identity();
		for (int i = 1; i <= terms; i++)
		{
			temp = temp * m / i;
			result = result + temp;
		}
		return result;
		
	}

	template< unsigned int Dim >
	Matrix< Dim , Dim > Matrix< Dim , Dim >::closestRotation( void ) const
	{
		/*
		To do this, you will need to implement the code in Matrix::closestRotation (in Util/geometry.todo.inl) 
		which returns the rotation closest to the linear transformation described by the Matrix object. 
		(To help you on your way, the methods Matrix::SVD and Matrix::determinant have already been provided, 
		giving the SVD decomposition of an arbitrary matrix as the product of a rotation, a diagonal, 
		and another rotation matrix, and returning the determinant of a matrix. 
		The Singular Value Decomposition is performed in such a way that the diagonal entries are non-negative 
		and strictly non-increasing.)
		*/
		Matrix< Dim , Dim > r1, d, r2;
		SVD( r1 , d , r2 );
		if( r1.determinant() * r2.determinant() < 0 ) for( int i=0 ; i<Dim ; i++ ) r1( i , Dim-1 ) *= -1;
		return r1 * r2;
	}


	/////////////////
	// BoundingBox //
	/////////////////
	template< unsigned int Dim >
	BoundingBox< 1 > BoundingBox< Dim >::intersect( const Ray< Dim > &ray ) const
	{
		///////////////////////////////////////////////////////////////
		// Compute the intersection of a BoundingBox with a Ray here //
		///////////////////////////////////////////////////////////////
		WARN_ONCE( "method undefined" );
		return BoundingBox<1>();
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