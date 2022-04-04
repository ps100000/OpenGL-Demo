#include <optional>
#include <fstream>

#include "glm/gtc/type_ptr.hpp"

#include "body.h"
#include "gl/light_manager.h"
#include "common/util.h"

using namespace scene;

std::vector<body::vertex> body::load_obj_file(std::string filename){
	std::vector<glm::vec3> v_coords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	
	std::vector<vertex> vertices;
	
	std::ifstream obj = std::ifstream(filename);
    std::string line;
    
    while (std::getline(obj, line)) {
		if(line.size() < 3){
			continue;
		}
        switch(line[0]){
			case 'v':
				switch(line[1]){
					case ' ':
						glm::vec3 v;
						sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
						v_coords.push_back(v);
						break;
					case 'n':
						glm::vec3 n;
						sscanf(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
						normals.push_back(n);
						break;
					case 't':
						glm::vec2 t;
						sscanf(line.c_str(), "vt %f %f", &t.x, &t.y);
						tex_coords.push_back(t);
						break;
					default:
						assert(0);
				}
				
				break;
			case 'f':
					int32_t ptn_indices[9];
					sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
					ptn_indices + 0, ptn_indices + 1, ptn_indices + 2,
					ptn_indices + 3, ptn_indices + 4, ptn_indices + 5,
					ptn_indices + 6, ptn_indices + 7, ptn_indices + 8);
					
					vertices.insert(vertices.end(),{
						{v_coords[ptn_indices[0]-1], tex_coords[ptn_indices[1]-1], normals[ptn_indices[2]-1]},
						{v_coords[ptn_indices[3]-1], tex_coords[ptn_indices[4]-1], normals[ptn_indices[5]-1]},
						{v_coords[ptn_indices[6]-1], tex_coords[ptn_indices[7]-1], normals[ptn_indices[8]-1]}
					});
				break;
			case 'm':
				char f[128];
				sscanf(line.c_str(), "mtllib %s", f);
				base_material = {f};
				break;
			default:
				std::cout << line << std::endl;
				break;
		}
    }
	
	return vertices;
}

body::body(std::string filename): body(load_obj_file(filename)){};

body::body(const std::vector<vertex>& vertices, std::shared_ptr<program> prog_param){
	if(!prog_param){
		if(!default_prog){
			default_prog = program::create("./resources/shaders/object.v.glsl", "./resources/shaders/object.f.glsl");
		}
		prog_param = default_prog;
	}
	prog = prog_param;
	
	std::shared_ptr<gl_um_buffer<scene::body::vertex>> vertices_buf = gl_um_buffer<scene::body::vertex>::create(
    	vertices,
		GL_ARRAY_BUFFER
	);
	std::shared_ptr<gl::vertex_array> v_array = gl::vertex_array::create(
		{
			{
				"i_v_coord",
				3,
				GL_FLOAT
			},
			{
				"i_v_tex",
				2,
				GL_FLOAT
			},
			{
				"i_v_norm",
				3,
				GL_FLOAT
			}
		},
		prog,
		vertices_buf,
		nullptr
	);

	GLint u_mvp = prog->get_uniform("u_m_mvp");
	GLint u_v_inv = prog->get_uniform("u_m_v_inv");
	GLint u_m = prog->get_uniform("u_m_m");
	GLint u_normal = prog->get_uniform("u_m_normal");
	GLint u_s_texture = prog->get_uniform("u_s_texture");
	GLint u_b_textured = prog->get_uniform("u_b_textured");
	GLint u_s_normalmap = prog->get_uniform("u_s_normalmap");
	GLint u_b_normalmaped = prog->get_uniform("u_b_normalmaped");

	GLint u_material_ambient = prog->get_uniform("u_material.ambient");
	GLint u_material_diffuse = prog->get_uniform("u_material.diffuse");
	GLint u_material_specular = prog->get_uniform("u_material.specular");
	GLint u_material_shininess = prog->get_uniform("u_material.shininess");

	geometry = gl::mesh{
		prog,
		v_array,
		GL_TRIANGLES,
		(std::function<void(std::shared_ptr<program>)>)
		[this, u_mvp, u_v_inv, u_m, u_normal, u_s_texture, u_b_textured, u_s_normalmap, u_b_normalmaped,
		u_material_ambient, u_material_diffuse, u_material_specular, u_material_shininess](std::shared_ptr<program>){
			glm::mat4 mvp = singleton<gl::state>::value.get_mvp();
			glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));
			glm::mat4 v_inv = glm::inverse(singleton<gl::state>::value.view);
			glUniformMatrix4fv(u_v_inv, 1, false, glm::value_ptr(v_inv));
			glUniformMatrix4fv(u_m, 1, false, glm::value_ptr(singleton<gl::state>::value.model));
			glm::mat3 normal = singleton<gl::state>::value.get_normal_matrix();
			glUniformMatrix3fv(u_normal, 1, false, glm::value_ptr(normal));
			
			glUniform1i(u_b_textured, !!texture.get());
			if(texture.get()){
				glUniform1i(u_s_texture, (GLuint)*texture);
			}
			glUniform1i(u_b_normalmaped, !!normalmap.get());
			if(normalmap.get()){
				glUniform1i(u_s_normalmap, (GLuint)*normalmap);
			}

			glUniformMatrix4fv(u_v_inv, 1, false, glm::value_ptr(v_inv));

			glUniform4fv(u_material_ambient, 1, glm::value_ptr(base_material.ambient));
			glUniform4fv(u_material_diffuse, 1, glm::value_ptr(base_material.diffuse));
			glUniform4fv(u_material_specular, 1, glm::value_ptr(base_material.specular));
			glUniform1f(u_material_shininess, base_material.shininess);
		}
	};
}

void body::draw(){
	ctx_begin();
	if(texture)
		((context*)texture.get())->ctx_begin();
	geometry.draw();
	if(texture)
		((context*)texture.get())->ctx_end();
	ctx_end();
}

void body::bind_lights(){
	static std::function<void()> binder = singleton<gl::light_manager>::value.bind(default_prog); 
	default_prog->use([=](){
		binder();
	});
}