/*==================================================================*\
  Sky.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <ShaderLibrary.hpp>
#include <BgfxHelpers.hpp>
#include <Sky.hpp>
//------------------------------------------------------------------//
#include <bx/math.h>
#include <utility>
//------------------------------------------------------------------//

bgfx::VertexDecl Sky::Vertex::Declaration;

Sky::Sky() : _vertexBuffer{ bgfx::kInvalidHandle }, _sunDirection{ bgfx::kInvalidHandle } {}

Sky::~Sky() {
	destroy();
}

void Sky::initialize() {
	Vertex::Declaration.begin()
		.add( bgfx::Attrib::Position,  3, bgfx::AttribType::Float )
	.end();
}

void Sky::regenerate( bool bottomLeftOrigin ) {
/* Create a full-screen triangle with the following vertex ordering:
   <2>--------+-----<0>
    |         |     /
    [  Screen ]   /
	|         | /
	+---------*
	|       /
	|     /
	|   /
	| /
   <1> */
	static const Vertex   vertices[]    = {{ { 3.0f, 1.0f, 0.0f } }, { { -1.0f, -3.0f, 0.0f } }, { { -1.0f, 1.0f, 0.0f } }};
	static const Vertex   bloVertices[] = {{ { 3.0f, 1.0f, 0.0f } }, { {  0.0f, -1.0f, 0.0f } }, { {  0.0f, 1.0f, 0.0f } }};

	if (!bgfx::isValid( _sunDirection )) {
		_sunDirection = bgfx::createUniform( "u_sunDirection", bgfx::UniformType::Vec4 );
	}

	bgfx::VertexBufferHandle vertexBuffer = bgfx::createVertexBuffer( bgfx::makeRef( bottomLeftOrigin ? bloVertices : vertices, sizeof( vertices ) ), Vertex::Declaration );

	std::swap( _vertexBuffer, vertexBuffer );

	DestroyHandle( vertexBuffer );
}

void Sky::submit( bgfx::ViewId view, const ShaderLibrary& shaders, int32_t sort ) const {
	bgfx::setVertexBuffer( 0, _vertexBuffer );
//	Disable depth writes.
	bgfx::setState( BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_LEQUAL, 0 );

	bgfx::submit( view, shaders.atmosphereProgram, sort );
}

void Sky::destroy() {
	DestroyHandle( _vertexBuffer );
	DestroyHandle( _sunDirection );

	_vertexBuffer.idx = bgfx::kInvalidHandle;
	_sunDirection.idx = bgfx::kInvalidHandle;
}

void Sky::updateUniforms( float /*appTime*/ ) {
	float	direction[4] = { 0.9f, 0.9f, 0.8f, 1.0f };

	bx::vec3Norm( direction, direction );

	bgfx::setUniform( _sunDirection, direction );
}