$input a_position, a_texcoord0
$output v_texcoord0

#include "common/common.sh"

void main() {
//	Make this a permanent resident of the far plane.
	gl_Position = vec4(a_position.xy, 1.0, 1.0);
	v_texcoord0 = a_texcoord0;
}