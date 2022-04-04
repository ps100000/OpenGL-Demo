in vec3 i_v_coord;
in vec2 i_v_tex;
in vec3 i_v_norm;

out vec2 c_v_tex;
out vec4 c_v_position;  // position of the vertex (and fragment) in world space
out vec3 c_v_surface_normal;  // surface normal vector in world space

uniform mat4 u_m_m;
uniform mat4 u_m_mvp;
uniform mat3 u_m_normal;

void main(){
	c_v_tex = i_v_tex;
	c_v_position = u_m_m * vec4(i_v_coord, 1.0);
	c_v_surface_normal = normalize(i_v_norm * u_m_normal);

	gl_Position = u_m_mvp * vec4(i_v_coord, 1.0);
}
