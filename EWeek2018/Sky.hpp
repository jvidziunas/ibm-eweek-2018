/*==================================================================*\
  Sky.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <bgfx/bgfx.h>
//------------------------------------------------------------------//

class ShaderLibrary;

class Sky {
public:
	struct Vertex {
		static bgfx::VertexDecl Declaration;
		
		float	position[3];
	};

public:
	Sky( const Sky& ) = delete;
	Sky();

	~Sky();

public:
	void	regenerate( bool bottomLeftOrigin );

	void	submit( bgfx::ViewId view, const ShaderLibrary& shaders, int32_t sort = 1 ) const;

	void	destroy();

public:
	void	updateUniforms( float appTime );

public:
	static void initialize();

private:
	bgfx::VertexBufferHandle _vertexBuffer;
	bgfx::UniformHandle      _sunDirection;
};