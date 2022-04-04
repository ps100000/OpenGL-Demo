in vec2 i_v_coord;

out vec2 c_v_tex;

uniform mat3 u_m_rst;

void main(){
	c_v_tex = i_v_coord;

    vec3 v_coord = u_m_rst * vec3(i_v_coord, 1.0f);

	gl_Position = vec4(v_coord.x, v_coord.y, 0.0, 1.0);
}
