$input v_position, v_texcoord0

#include "common/common.sh"

uniform vec4 u_eyePosition;
uniform vec4 u_sunDirection;

#define u_fogAttenuation 0.00065
#define u_skyColor       vec3( 0.5, 0.6, 0.65 )
#define u_sunColor       vec3( 1.5, 1.35, 1.05 )
#define u_groundColor    vec3( 0.255, 0.275, 0.2 )

vec3 perturbNormal( vec3 position, float height ) {
	vec3 sigmaS = ddx( position );
	vec3 sigmaT = ddy( position );
	vec3 normal = cross( normalize( sigmaS ), normalize( sigmaT ) );

	vec3 r1 = cross( sigmaT, normal );
	vec3 r2 = cross( normal, sigmaS );

	float determinant = dot( sigmaS, r1 );

	float dbs = ddx( height );
	float dbt = ddy( height );

	vec3 gradient = sign( determinant ) * ( dbs * r1 + dbt * r2 );
	return normalize( abs( determinant ) * normal - gradient );
}

vec3 applyFog( vec3 rgb, float distance, float attenuation, vec3 eyeDir, vec3 sunDir ) {
    float sunAmount = max( dot( eyeDir, sunDir ), 0.0 );
    vec3  fogColor  = mix( u_skyColor, u_sunColor, pow( sunAmount, 8.0 ) );

    return mix( rgb, fogColor, 1.0 - exp( -distance * attenuation ) );
}

void main() {
	vec3 eyeDir = v_position - u_eyePosition;
	vec3 normal = perturbNormal( v_position, 0.0 );

	vec3 groundColor = u_groundColor * max( dot( normal, u_sunDirection ), 0.0 );

	gl_FragColor = vec4( applyFog( groundColor, length( eyeDir ), u_fogAttenuation, normalize( eyeDir ), u_sunDirection ), 1.0 );
}
