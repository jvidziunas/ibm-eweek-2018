/*==================================================================*\
  Terrain.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <ShaderLibrary.hpp>
#include <BgfxHelpers.hpp>
#include <Terrain.hpp>
//------------------------------------------------------------------//
#include <bgfx_utils.h>
#include <bx/bx.h>
//------------------------------------------------------------------//

bgfx::VertexDecl Terrain::Vertex::Declaration;
PerlinGenerator<> Terrain::Noise;

void Terrain::initialize( uint32_t seed, float mu ) {
	Noise.reseed( seed, mu );

	Vertex::Declaration.begin()
		.add( bgfx::Attrib::Position,  3, bgfx::AttribType::Float )
		.add( bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float )
	.end();
}

Terrain::Terrain() : _vertexBuffer{ bgfx::kInvalidHandle }, _indexBuffer{ bgfx::kInvalidHandle } {}

Terrain::~Terrain() {
	destroy();
}

float Terrain::sampleAltitude( float x, float y ) const {
	if (!_vertices) {
		return 0.0f;
	}

	x = bx::clamp( x, 0.0f, static_cast<float>(_sideResolution) );
	y = bx::clamp( y, 0.0f, static_cast<float>(_sideResolution) );

	int32_t index = static_cast<int32_t>(bx::trunc( x ) + _sideResolution * bx::trunc( y ));

//	Bicubic interpolation of height values.
	return bx::lerp(
		bx::lerp( _vertices[index                  ].position[1], _vertices[                  index + 1].position[1], bx::smoothStep( bx::fract( x ) ) ),
		bx::lerp( _vertices[_sideResolution + index].position[1], _vertices[_sideResolution + index + 1].position[1], bx::smoothStep( bx::fract( x ) ) ),
		bx::smoothStep( bx::fract( y ) )
	);
}

void Terrain::regenerate( float altitude, uint32_t resolution, uint32_t octaves ) {
	const uint32_t vertexCount = resolution * resolution;

//	Generate Perlin noise for terrain mesh.
	std::unique_ptr<Vertex[]> vertices( std::make_unique<Vertex[]>( vertexCount ) );
	for (uint32_t y = 0; y < resolution; ++y) {
		for (uint32_t x = 0; x < resolution; ++x) {
			Vertex&	vertex = vertices[y * resolution + x];

			vertex.position[0]  = static_cast<float>(x);
			vertex.position[1]  = Noise( static_cast<float>(x) / resolution, static_cast<float>(y) / resolution, octaves ) * altitude;
			vertex.position[2]  = static_cast<float>(y);
			vertex.texcoords[0] = (x + 0.5f) / resolution;
			vertex.texcoords[1] = (y + 0.5f) / resolution;
		}
	}

//	Generate index buffer for terrain mesh. BGFX will release this, we do not need to manage.
	const bgfx::Memory* indexMemory( bgfx::alloc( 6u * vertexCount * sizeof( uint32_t ) ) );
	for (uint32_t y = 0, indexCount = 0; y < (resolution - 1); y++) {
		for (uint32_t x = 0; x < (resolution - 1); x++, indexCount += 6) {
		/*  Ordering visualization for a terrain patch:
			<2  >-----<0|5>
			  | (1)   / |
			  |     /   |
			  |   /     |
			  | /   (2) |
			<1|4>-----<  3> */
			uint32_t* const quadIndices = reinterpret_cast<uint32_t*>( indexMemory->data ) + indexCount;
			const uint32_t  vertexId    = y * resolution + x;

			quadIndices[0] = vertexId + 1;
			quadIndices[1] = vertexId + resolution;
			quadIndices[2] = vertexId;
			quadIndices[3] = vertexId + resolution + 1;
			quadIndices[4] = vertexId + resolution;
			quadIndices[5] = vertexId + 1;
		}
	}

	bgfx::VertexBufferHandle vertexBuffer( bgfx::createVertexBuffer( bgfx::makeRef( vertices.get(), vertexCount * sizeof( Vertex ) ), Vertex::Declaration ));
	bgfx::IndexBufferHandle  indexBuffer( bgfx::createIndexBuffer( indexMemory, BGFX_BUFFER_INDEX32 ) );

	std::swap( _vertices,       vertices );
	std::swap( _sideResolution, resolution );
	std::swap( _vertexBuffer,   vertexBuffer );
	std::swap( _indexBuffer,    indexBuffer );

	DestroyHandle( vertexBuffer );
	DestroyHandle( indexBuffer );

	bx::mtxTranslate( _localToWorld, 0.0f, 0.0f, 0.0f );
}

void Terrain::submit( bgfx::ViewId view, const ShaderLibrary& shaders, int32_t sort ) const {
	bgfx::setTransform( _localToWorld );

	bgfx::setVertexBuffer( 0, _vertexBuffer );
	bgfx::setIndexBuffer( _indexBuffer );

	bgfx::submit( view, shaders.terrainProgram, sort );
}

void Terrain::destroy() {
	_vertices.reset();

	DestroyHandle( _vertexBuffer );
	DestroyHandle( _indexBuffer );

	_vertexBuffer.idx = bgfx::kInvalidHandle;
	_indexBuffer.idx  = bgfx::kInvalidHandle;
}