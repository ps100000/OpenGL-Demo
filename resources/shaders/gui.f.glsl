in vec2 c_v_tex;

out vec4 color;

uniform sampler2D u_s_texture;


void main(){
    float y = 1.0 - c_v_tex.y;
    vec4 texture_color = texture(u_s_texture, vec2(c_v_tex.x, y));
	
	color = texture_color;
}
