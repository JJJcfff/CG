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
				t *= samples.size() - 1;
				int i1 = (int)floor(t);
				int i2 = i1 + 1;
				double w = t - i1;
				return (1 - w) * samples[i1] + w * samples[i2];
				break;
			}
			case CATMULL_ROM:
			{	

				t *= samples.size() - 1;
				
				int i0 = (int)floor(t) - 1;
				if (i0 < 0) i0 = 0;
				int i1 = i0 + 1;
				int i2 = i1 + 1;
				int i3 = i2 + 1;
				if (i3 >= samples.size()) i3 = samples.size() - 1;
				
				double w = t - i1;
				
				return 0.5 * ((2 * samples[i1]) + 
											(-samples[i0] + samples[i2]) * w + 
											(2 * samples[i0] - 5 * samples[i1] + 4 * samples[i2] - samples[i3]) * w * w + 
											(-samples[i0] + 3 * samples[i1] - 3 * samples[i2] + samples[i3]) * w * w * w);
				break;
			}
			case UNIFORM_CUBIC_B_SPLINE:
			{
				t *= samples.size() - 1;
				
				int i0 = (int)floor(t) - 1;
				if (i0 < 0) i0 = 0;
				int i1 = i0 + 1;
				int i2 = i1 + 1;
				int i3 = i2 + 1;
				if (i3 >=	samples.size()) i3 = samples.size() - 1;
				double w = t - i1;

				return (1.0 / 6.0) * ((samples[i0] * (-1.0 + 3.0 * w - 3.0 * w * w + w * w * w)) +
				(samples[i1] * (3.0 - 6.0 * w * w + 3.0 * w * w * w)) +
				(samples[i2] * (3.0 * w + 3.0 * w * w - 3.0 * w * w * w)) +
				(samples[i3] * (w * w * w)));	
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