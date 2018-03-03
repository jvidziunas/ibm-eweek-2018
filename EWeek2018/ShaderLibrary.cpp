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
#include <BgfxHelpers.hpp>
//------------------------------------------------------------------//
#include <bgfx_utils.h>
#include <utility>
//------------------------------------------------------------------//

ShaderLibrary::ShaderLibrary() : terrainProgram{ bgfx::kInvalidHandle }, atmosphereProgram{ bgfx::kInvalidHandle }, eyePositionUnifom{ bgfx::kInvalidHandle } {}

ShaderLibrary::~ShaderLibrary() {
	destroy();
}

void ShaderLibrary::regenerate() {
	if (!bgfx::isValid( eyePositionUnifom )) {
		eyePositionUnifom = bgfx::createUniform( "u_eyePosition", bgfx::UniformType::Vec4 );
	}

	bgfx::ProgramHandle program( loadProgram( "vs_terrain", "fs_terrain" ) );
	std::swap( terrainProgram, program );
	DestroyHandle( program );

	program = loadProgram( "vs_atmosphere", "fs_atmosphere" );
	std::swap( atmosphereProgram, program );
	DestroyHandle( program );
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