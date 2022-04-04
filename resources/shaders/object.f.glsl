in vec2 c_v_tex;
in vec4 c_v_position;  // position of the vertex (and fragment) in world space
in vec3 c_v_surface_normal;  // surface normal vector in world space

out vec4 color;

uniform mat4 u_m_v_inv;
uniform sampler2D u_s_texture;
uniform bool u_b_textured;
uniform sampler2D u_s_normalmap;
uniform bool u_b_normalmaped;
uniform mat3 u_m_normal;

struct light{
	bool on;
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float attenuation_constant, attenuation_linear, attenuation_quadratic;
	float spot_cutoff, spot_exponent;
	vec3 spot_direction;
};
const int number_of_lights = 8;
uniform light u_lights[number_of_lights];

struct material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform material u_material;

vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);
 

void main(){
	vec3 normal_direction;
	if(u_b_normalmaped){
		float y = 1.0 - c_v_tex.y;
		normal_direction = vec3(texture(u_s_normalmap, vec2(c_v_tex.x, y))) - vec3(0.5, 0.5, 0.5);
		normal_direction *= u_m_normal;
	}else{
		normal_direction = c_v_surface_normal;
	}
	normal_direction = normalize(normal_direction);
	vec3 view_direction = normalize(vec3(u_m_v_inv * vec4(0.0, 0.0, 0.0, 1.0) - c_v_position));
	vec3 light_direction;
	float attenuation;
	vec4 total_lighting = scene_ambient * u_material.ambient;
	
	for (int i = 0; i < number_of_lights; i++){ // for all light sources
		if(!u_lights[i].on){
			continue;
		}
		if(0.0 == u_lights[i].position.w){ // _directional light?
			attenuation = 1.0; // no attenuation
			light_direction = normalize(vec3(u_lights[i].position));
		}else{ // point light or spotlight (or other kind of light)
			vec3 position_to_light = vec3(u_lights[i].position - c_v_position);
			float distance = length(position_to_light);
			light_direction = normalize(position_to_light);
			attenuation = 1.0 / (u_lights[i].attenuation_constant
						 + u_lights[i].attenuation_linear * distance
						 + u_lights[i].attenuation_quadratic * distance * distance);
				
			if(u_lights[i].spot_cutoff <= 90.0){ // spotlight?
				float clamped_cosine = max(0.0, dot(-light_direction, u_lights[i].spot_direction));
				if(clamped_cosine < cos(radians(u_lights[i].spot_cutoff))){ // outside of spotlight cone?
					attenuation = 0.0;
				}else{
					attenuation = attenuation * pow(clamped_cosine, u_lights[i].spot_exponent);   
				}
			}
		}
		
		
		vec4 diffuse_reflection = attenuation 
			* u_lights[i].diffuse * u_material.diffuse
			* max(0.0, dot(normal_direction, light_direction));
		
		vec4 specular_reflection;
		if(dot(normal_direction, light_direction) < 0.0){ // light source on the wrong side?
			specular_reflection = vec4(0.0, 0.0, 0.0, 0.0); // no specular reflection
		}else{// light source on the right side
			specular_reflection = attenuation * u_lights[i].specular * u_material.specular 
				* pow(max(0.0, dot(reflect(-light_direction, normal_direction), view_direction)), u_material.shininess);
		}
		total_lighting = total_lighting + diffuse_reflection + specular_reflection;
	}
	
	vec4 texture_color = vec4(1.0, 1.0, 1.0, 1.0);
	if(u_b_textured){
		float y = 1.0 - c_v_tex.y;
		texture_color = texture(u_s_texture, vec2(c_v_tex.x, y));
	}
	
	color = total_lighting * texture_color;
}
