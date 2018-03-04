$input v_view

#include "common/common.sh"

uniform vec4 u_eyePosition;
uniform vec4 u_sunDirection;

#define u_skyColor vec3( 0.5, 0.6, 0.65 )
#define u_sunColor vec3( 1.5, 1.35, 1.05 )

vec3 getAtmosphereColor( vec3 eyeDir, vec3 sunDir ) {
	float sunAmount = max( dot( eyeDir, sunDir ), 0.0 );
	
    return mix( u_skyColor, u_sunColor, pow( sunAmount, 8.0 ) );
}

void main() {
	gl_FragColor = vec4( getAtmosphereColor( normalize( v_view ), u_sunDirection ), 1.0 );
//	gl_FragColor = vec4( u_skyColor, 1.0 );
}