/*==================================================================*\
  ShaderLibrary.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <ShaderLibrary.hpp>
//------------------------------------------------------------------//
#include <bgfx_utils.h>
#include <utility>
//------------------------------------------------------------------//

namespace {

	template <typename Handle>
	inline void DestroyHandle( Handle handle ) {
		if (bgfx::isValid( handle )) {
			bgfx::destroy( handle );
		}
	}

}	// anonymous namespace

ShaderLibrary::ShaderLibrary() : terrainProgram{ bgfx::kInvalidHandle }, atmosphereProgram{ bgfx::kInvalidHandle }, eyePositionUnifom{ bgfx::kInvalidHandle } {}

ShaderLibrary::~ShaderLibrary() {
	destroy();
}

void ShaderLibrary::reload() {
	bgfx::ProgramHandle program( loadProgram( "vs_terrain", "fs_terrain" ) );
	std::swap( terrainProgram, program );
	DestroyHandle( program );

	program = loadProgram( "vs_atmosphere", "fs_atmosphere" );
	std::swap( atmosphereProgram, program );
	DestroyHandle( program );

	if (!bgfx::isValid( eyePositionUnifom )) {
		eyePositionUnifom = bgfx::createUniform( "u_eyePosition", bgfx::UniformType::Vec4 );
	}
}

void ShaderLibrary::destroy() {
	DestroyHandle( terrainProgram );
	DestroyHandle( atmosphereProgram );
	DestroyHandle( eyePositionUnifom );

	terrainProgram.idx    = bgfx::kInvalidHandle;
	atmosphereProgram.idx = bgfx::kInvalidHandle;
	eyePositionUnifom.idx = bgfx::kInvalidHandle;
}

void ShaderLibrary::updateUniforms( const float (&eyePosition)[3] ) {
	bgfx::setUniform( eyePositionUnifom, eyePosition );
}