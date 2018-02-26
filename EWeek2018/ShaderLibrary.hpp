/*==================================================================*\
  ShaderLibrary.hpp
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

class ShaderLibrary {
public:
	ShaderLibrary();
	ShaderLibrary( const ShaderLibrary& ) = delete;

	~ShaderLibrary();

public:
	void	reload();

	void	destroy();

	void	updateUniforms( const float (&eyePosition)[3] );

public:
	bgfx::ProgramHandle	terrainProgram;
	bgfx::ProgramHandle atmosphereProgram;

	bgfx::UniformHandle eyePositionUnifom;
};