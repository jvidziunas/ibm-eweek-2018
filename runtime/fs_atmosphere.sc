$input v_position, v_texcoord0

#include "common/common.sh"

#define u_skyColor vec3( 0.5, 0.6, 0.65 )

void main() {
	gl_FragColor = vec4( u_skyColor, 1.0 );
}