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

ShaderLibrary::ShaderLibrary() : terrainProgram{ bgfx::kInvalidHandle } {}

void ShaderLibrary::reload() {
	bgfx::ProgramHandle program( loadProgram( "terrain.vs", "terrain.fs" ) );

	std::swap( terrainProgram, program );
}

void ShaderLibrary::destroy() {
	DestroyHandle( terrainProgram );

	terrainProgram.idx = bgfx::kInvalidHandle;
}