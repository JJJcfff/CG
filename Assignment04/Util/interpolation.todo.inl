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

#include <math.h>
#include <Util/exceptions.h>

namespace Util
{
	///////////////////
	// Interpolation //
	///////////////////
	template< typename SampleType >
	SampleType Interpolation::Sample( const std::vector< SampleType > &samples , double t , int interpolationType )
	{
		switch( interpolationType )
		{
			case NEAREST:
			{
				t *= samples.size();
				int it1 = (int)floor(t);
				int it2 = ( it1 + 1 ) % samples.size();
				t -= it1;
				if( t<0.5 ) return samples[it1];
				else        return samples[it2];
				break;
			}
			case LINEAR:
			{
				t *= samples.size();
				int it1 = (int)floor(t);
				int it2 = ( it1 + 1 ) % samples.size();
				t -= it1;
				return (1-t)*samples[it1] + t*samples[it2];
				break;
			}
			case CATMULL_ROM:
			{	
				t *= samples.size();
				int it1 = (int)floor(t);
				int it0 = (it1 - 1) % samples.size();
				int it2 = (it1 + 1) % samples.size();
				int it3 = (it1 + 2) % samples.size();
				double u = t - it1;
				double u2 = u*u;
				double u3 = u2*u;
				double c0 = -0.5*u3 + u2 - 0.5*u;
				double c1 =  1.5*u3 - 2.5*u2 + 1;
				double c2 = -1.5*u3 + 2*u2 + 0.5*u;
				double c3 =  0.5*u3 - 0.5*u2;
				return c0 * samples[it0] + c1 * samples[it1] + c2 * samples[it2] + c3 * samples[it3];
				break;
			}
			case UNIFORM_CUBIC_B_SPLINE:
			{
				//todo; perform uniform cubic b-spline interpolations
				t *= samples.size();
				int it1 = (int)floor(t);
				int it0 = (it1 - 1) % samples.size();
				int it2 = (it1 + 1) % samples.size();
				int it3 = (it1 + 2) % samples.size();
				double u = t - it1;
				double u2 = u*u;
				double u3 = u2*u;
				double c0 = 1.0/6.0 * ( -u3 + 3*u2 - 3*u + 1 );
				double c1 = 1.0/6.0 * (  3*u3 - 6*u2 + 4 );
				double c2 = 1.0/6.0 * ( -3*u3 + 3*u2 + 3*u + 1 );
				double c3 = 1.0/6.0 * (  u3 );
				return c0 * samples[it0] + c1 * samples[it1] + c2 * samples[it2] + c3 * samples[it3];
				break;
			}
			default:
			{
				ERROR_OUT( "unrecognized interpolation type" );
				return samples[0];
			}
		}
	}
}