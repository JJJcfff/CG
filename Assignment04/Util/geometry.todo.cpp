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
#include <cmath>
#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <Util/exceptions.h>
#include "geometry.h"

namespace Util
{
	////////////////////////////
	// EulerRotationParameter //
	////////////////////////////
	Matrix3D EulerRotationParameter::operator()(void) const
	{	
		/*
		The default representation of a rotation is by a 3x3 (not necessarily rotation) matrix. 
		Consequently, even if the samples are rotations, interpolation can introduce unwanted scaling (as in the runner's 
		legs in Data/act/test.ray). Modify the code to support parametrization of rotations using Euler angles.
		To do this, you will need to implement the operator EulerRotationParameter::operator() (in Util/geometry.todo.cpp) 
		which transforms the triplet of Euler angles into a rotation matrix.
		Recall that the three Euler angles define a rotation which is the product of a rotation about the x-axes, 
		multiplied on the left by a rotation about the y-axes, multiplied on the left by a rotation about the z-axes.
		*/
		double phi = parameter[0]*180/M_PI;
		double theta = parameter[1]*180/M_PI;
		double psi = parameter[2]*180/M_PI;
		Matrix3D x, y, z;
		x(0, 0) = 1;
		x(1, 1) = cos(phi);
		x(1, 2) = -sin(phi);
		x(2, 1) = sin(phi);
		x(2, 2) = cos(phi);

		y(0, 0) = cos(theta);
		y(0, 2) = sin(theta);
		y(1, 1) = 1;
		y(2, 0) = -sin(theta);
		y(2, 2) = cos(theta);

		z(0, 0) = cos(psi);
		z(0, 1) = -sin(psi);
		z(1, 0) = sin(psi);
		z(1, 1) = cos(psi);
		z(2, 2) = 1;
		return z * y * x;
	}
	/////////////////////////////////
	// QuaternionRotationParameter //
	/////////////////////////////////
	Matrix3D QuaternionRotationParameter::operator()(void) const
	{
		Quaternion q = parameter.unit();
		Matrix3D r;
		r(0, 0) = 1 - 2 * (q.imag[1] * q.imag[1] + q.imag[2] * q.imag[2]);
		r(0, 1) = 2 * (q.imag[0] * q.imag[1] - q.imag[2] * q.real);
		r(0, 2) = 2 * (q.imag[0] * q.imag[2] + q.imag[1] * q.real);
		r(1, 0) = 2 * (q.imag[0] * q.imag[1] + q.imag[2] * q.real);
		r(1, 1) = 1 - 2 * (q.imag[0] * q.imag[0] + q.imag[2] * q.imag[2]);
		r(1, 2) = 2 * (q.imag[1] * q.imag[2] - q.imag[0] * q.real);
		r(2, 0) = 2 * (q.imag[0] * q.imag[2] - q.imag[1] * q.real);
		r(2, 1) = 2 * (q.imag[1] * q.imag[2] + q.imag[0] * q.real);
		r(2, 2) = 1 - 2 * (q.imag[0] * q.imag[0] + q.imag[1] * q.imag[1]);
		return r;
	}
}
