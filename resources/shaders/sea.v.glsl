precision highp sampler3D;
in vec3 i_v_coord;
in vec2 i_v_tex;
in vec3 i_v_norm;

out vec2 c_v_tex;
out vec4 c_v_position;  // position of the vertex (and fragment) in world space
out vec3 c_v_surface_normal;  // surface normal vector in world space

uniform mat4 u_m_m;
uniform mat4 u_m_mvp;
uniform mat3 u_m_normal;
uniform float u_f_progress;
uniform bool u_b_heightmap;
uniform sampler3D u_s_heightmap;

float get_height(vec2 v_tex){
	if(!u_b_heightmap){
		return 0.0f;
	}
	float height = texture(u_s_heightmap, vec3(v_tex, u_f_progress)).r;
	height -= 0.25f;
	height *= 2.0f;
	return height;
}

void main(){
	vec2 v_tex = i_v_tex * 498.0f / 500.0f;
	c_v_tex = v_tex;

	v_tex += vec2(1.0f / 500.0f, 1.0f / 500.0f);
	float height = get_height(v_tex);
	vec4 v_coord = vec4(
		i_v_coord.x,
		height,
		i_v_coord.z,
		1.0f);
	c_v_position = u_m_m * v_coord;

	c_v_surface_normal = i_v_norm * u_m_normal;

	gl_Position = u_m_mvp * v_coord;
}
