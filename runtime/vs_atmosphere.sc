$input a_position, a_texcoord0
$output v_view

#include "common/common.sh"

void main() {
//	Make this a permanent resident of the far plane.
	vec4 position = vec4( a_position.xy, 1.0, 1.0 );

	gl_Position = position;
	v_view = mul( u_invViewProj, position );
}