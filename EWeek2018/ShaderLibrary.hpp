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
	ShaderLibrary( const ShaderLibrary& ) = delete;
	ShaderLibrary();

	~ShaderLibrary();

public:
	void	regenerate();

	void	destroy();

	void	updateUniforms( const float (&eyePosition)[3] );

public:
	bgfx::ProgramHandle	terrainProgram;
	bgfx::ProgramHandle atmosphereProgram;

	bgfx::UniformHandle eyePositionUnifom;
};