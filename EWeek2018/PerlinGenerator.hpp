/*==================================================================*\
  PerlinGenerator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <bx/math.h>
#include <random>
//------------------------------------------------------------------//

template <size_t B = 256u>
class PerlinGenerator {
public:
	PerlinGenerator( int seed ) {
		reseed( seed );
	}

	PerlinGenerator( const PerlinGenerator& ) = default;
	PerlinGenerator() = default;

	~PerlinGenerator() = default;

public:
	void reseed( int seed, float mu ) {
		std::uniform_real_distribution<float> floatDist( -1.0f, 1.0f );
		std::mt19937                          generator( seed );

		mu = bx::clamp( mu, 1.0f, 1.16f );

		for (float (&gradient)[2] : _gradients) {
			gradient[0] = floatDist( generator );
			gradient[1] = floatDist( generator );

			normalize( gradient );
		}

		for (int i = 0; i < B; ++i) {
			_permutations[i] = i;
		}

		for (size_t i = B - 1; i > 0; i--) {
			bx::xchg( _permutations[i], _permutations[generator() % (i + 1)] );
		}

		float s = 1.0; //current magnitude
		for (float& magnitude : _magnitudes) {
			magnitude = s; s /= mu;
		}
	}

	float operator()( float x, float y, uint32_t octaves ) {
		float scale = 0.5f;
		float sum   = 0.0f;

		for (uint32_t i = 0; i < octaves; ++i) {
			sum += scale * (*this)( x, y );
			scale *= 0.5f;
			x *= 2.0f; y *= 2.0f;
		}
		
		return bx::kSqrt2 * sum / ( 1.0f - scale );
	}
	
	float operator()( float x, float y ) {
		enum : int {
			TableMask = B - 1,
			N         = 0x1000
		};

	#define at2(rx, ry) (rx*q[0] + ry*q[1])
	#define setup(in,b0,b1,r0,r1)\
			  t  = in + N;\
			  b0 = ((int)t) & TableMask;\
			  b1 = (b0 + 1) & TableMask;\
			  r0 = t - (int)t;\
			  r1 = r0 - 1.0f;

		int bx0, bx1, by0, by1;
		float rx0, rx1, ry0, ry1, *q, t;

		setup( x, bx0, bx1, rx0, rx1 );
		setup( y, by0, by1, ry0, ry1 );

		int b00 = _permutations[(_permutations[bx0] + by0) & TableMask];
		int b10 = _permutations[(_permutations[bx1] + by0) & TableMask];
		int b01 = _permutations[(_permutations[bx0] + by1) & TableMask];
		int b11 = _permutations[(_permutations[bx1] + by1) & TableMask];

		float sx = bx::smoothStep( rx0 );

		float u, v;
		q = _gradients[b00]; u = _magnitudes[b00] * at2( rx0, ry0 );
		q = _gradients[b10]; v = _magnitudes[b10] * at2( rx1, ry0 );
		float a = bx::lerp( u, v, sx );

		q = _gradients[b01]; u = _magnitudes[b01] * at2( rx0, ry1 );
		q = _gradients[b11]; v = _magnitudes[b11] * at2( rx1, ry1 );
		float b = bx::lerp( u, v, sx );

		float sy = bx::smoothStep( ry0 );

		return bx::lerp( a, b, sy );
	}

private:
	static inline void normalize( float (&vector)[2] ) {
		const float rcpLength = bx::rsqrt( bx::square( vector[0] ) + bx::square( vector[1] ) );

		vector[0] *= rcpLength;
		vector[1] *= rcpLength;
	}

private:
	int   _permutations[B];
	float _gradients[B][2];
	float _magnitudes[B];
};