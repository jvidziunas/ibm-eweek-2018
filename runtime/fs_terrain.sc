$input v_position, v_texcoord0

#include "common/common.sh"

uniform vec4 u_eyePosition;

#define u_fogAttenuation 0.0065
#define u_skyColor       vec3( 0.5, 0.6, 0.65 )
#define u_sunColor       vec3( 1.0, 0.9, 0.7 )
#define u_groundColor    vec3( 0.255, 0.275, 0.2 )
#define u_sunDirection   normalize( vec3( 0.9, 0.8, 0.9 ) )

vec3 applyFog( vec3 rgb, float distance, vec3 eyeDir, vec3 sunDir ) {
    float fogAmount = 1.0 - exp( -distance * u_fogAttenuation );
    float sunAmount = max( dot( eyeDir, sunDir ), 0.0 );
    vec3  fogColor  = mix( u_skyColor, u_sunColor, pow( sunAmount, 8.0 ) );

    return mix( rgb, fogColor, fogAmount );
}

void main() {
	vec3 eyeDir = v_position - u_eyePosition;

	gl_FragColor = vec4( applyFog( u_groundColor, length( eyeDir ), normalize( eyeDir ), u_sunDirection ), 1.0 );
}
