/*==================================================================*\
  Main.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------

\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <ShaderLibrary.hpp>
#include <Terrain.hpp>
//------------------------------------------------------------------//
#include <entry/input.h>
#include <bgfx_utils.h>
#include <bx/timer.h>
#include <camera.h>
//------------------------------------------------------------------//

namespace {

	enum : uint32_t {
		BgfxResetBits  = BGFX_RESET_VSYNC | BGFX_RESET_SRGB_BACKBUFFER,
		BgfxDebugBits  = BGFX_DEBUG_NONE,
		DefaultSeed    = 6690u,
		SideResolution = 1024u
	};

	enum : bgfx::ViewId { BgfxPrimaryView = 0 };

	constexpr float CameraHeight = 10.0f;
	constexpr float DefaultMu    = 1.01f;
	constexpr float NearDistance = 0.1f;
	constexpr float FarDistance  = 2000.0f;

	class EWeek2018Example : public entry::AppI {
	public:
		EWeek2018Example( const char* name, const char* description ) : entry::AppI( name, description ) {}
		EWeek2018Example( const EWeek2018Example& ) = delete;

		~EWeek2018Example() = default;

	public:
		void init( int32_t argc, const char* const* argv, uint32_t width, uint32_t height ) override {
		//	Parse BGFX-related options from the command line.
			Args args( argc, argv );

			_screen.width  = width;
			_screen.height = height;
			_terrainSeed   = DefaultSeed;

			if (!bgfx::init( bgfx::RendererType::Direct3D11, args.m_pciId )) {
				return;
			}

			bgfx::setDebug( (_debugBits = BgfxDebugBits) );
			bgfx::reset( width, height, (_resetBits = BgfxResetBits) );
			bgfx::setViewClear( BgfxPrimaryView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0 );

			Terrain::Initialize( _terrainSeed, DefaultMu );

			_terrain.regenerate( 800.0f, SideResolution );
			_shaderLibrary.reload();

			cameraCreate();

			const float cameraStart[3] = { 256.0f, _terrain.sampleAltitude( 256.0f, 256.0f ) + CameraHeight, 256.0f };
			cameraSetPosition( cameraStart );
			cameraSetVerticalAngle( -bx::kPi / 4.0f );
		}

		int shutdown() override {
			cameraDestroy();

			_terrain.destroy();
			_shaderLibrary.destroy();

		//	We need to advance the frame so BGFX knows we're no longer using any frame-specific resources.
			bgfx::frame();
			bgfx::shutdown();

			return 0;
		}

		bool update() override {
			if (entry::processEvents( _screen.width, _screen.height, _debugBits, _resetBits, &_mouseState )) {
				return false;
			}

			const int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;

			updateCamera( frameTime / static_cast<float>(bx::getHPFrequency()) );

			float view[16];
			cameraGetViewMtx( view );

			float projection[16];
			bx::mtxProj( projection, 60.0f, static_cast<float>(_screen.width) / static_cast<float>(_screen.height), NearDistance, FarDistance, false );

			bgfx::setViewRect( BgfxPrimaryView, 0, 0, static_cast<uint16_t>(_screen.width), static_cast<uint16_t>(_screen.height) );
			bgfx::setViewTransform( BgfxPrimaryView, view, projection );

			_terrain.submit( BgfxPrimaryView, _shaderLibrary );

			bgfx::frame();

			return true;
		}

		void updateCamera( float deltaTime ) {
			cameraUpdate( deltaTime, _mouseState );

			float position[3];
			cameraGetPosition( position );
		//  Disabled as it results in very jerky motion.
		//  position[1] = _terrain.sampleAltitude( position[0], position[2] ) + CameraHeight;
		//	cameraSetPosition( position );

			_shaderLibrary.updateUniforms( position );
		}

	private:
		struct {
			uint32_t width;
			uint32_t height;
		} _screen;

		uint32_t          _resetBits;
		uint32_t          _debugBits;

		uint32_t          _terrainSeed;

		ShaderLibrary     _shaderLibrary;
		Terrain           _terrain;
		entry::MouseState _mouseState;
	};

}	// anonymous namespace

ENTRY_IMPLEMENT_MAIN( EWeek2018Example, "EWeek2018", "IBM E-Week 2018 Example" );