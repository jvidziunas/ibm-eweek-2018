/*==================================================================*\
  Terrain.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <PerlinGenerator.hpp>
#include <bgfx/bgfx.h>
#include <memory>
//------------------------------------------------------------------//

class ShaderLibrary;

class Terrain {
public:
	struct Vertex {
		static bgfx::VertexDecl Declaration;
		
		float	position[3];
		float	texcoords[2];
	};

public:
	Terrain( const Terrain& ) = delete;
	Terrain();

	~Terrain();

public:
	float	sampleAltitude( float x, float y ) const;

	void	regenerate( float altitude, uint32_t sideResolution );

	void	submit( bgfx::ViewId view, const ShaderLibrary& shaders, int32_t sort = 0 ) const;

	void	destroy();

public:
	static void Initialize( uint32_t seed, float mu );

private:
	static PerlinGenerator<>  Noise;

	std::unique_ptr<Vertex[]> _vertices;
	uint32_t                  _sideResolution;

	bgfx::VertexBufferHandle  _vertexBuffer;
	bgfx::IndexBufferHandle   _indexBuffer;

	float                     _localToWorld[4 * 4];
};