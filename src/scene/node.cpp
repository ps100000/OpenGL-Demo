#include "node.h"

#include "glm/gtx/string_cast.hpp"

#include "conf/parser.h"
#include "gl/light_manager.h"
#include "common/mat_helper.h"

using namespace scene::graph;
namespace{
	inline glm::vec3 vec3(conf::elm& array){
		return glm::vec3{
			(float)array[0],
			(float)array[1],
			(float)array[2]
		};
	}
	inline glm::vec4 vec4(conf::elm& array){
		return glm::vec4{
			(float)array[0],
			(float)array[1],
			(float)array[2],
			(float)array[3]
		};
	}
}

void node::load_body(conf::elm& conf_elm, node& body_node){
	body_node.type = type_t::BODY;
	std::shared_ptr<gl::texture> texture = nullptr;
	if(conf_elm.has_key("texture")){
		texture = gl::texture::create((std::string)conf_elm["texture"]);
	}
	std::shared_ptr<gl::texture> normalmap = nullptr;
	if(conf_elm.has_key("normalmap")){
		normalmap = gl::texture::create((std::string)conf_elm["normalmap"]);
	}
	body_node.drawable = std::shared_ptr<body>(new body{(std::string)conf_elm["obj"]});
	body_node.drawable->texture = texture;
	body_node.drawable->normalmap = normalmap;
}

void node::load_light(conf::elm& conf_elm, node& light_node){
	light_node.type = type_t::LIGHT;
	static gl::light_manager& light_mgr = singleton<gl::light_manager>::value;
	light_node.light_index = light_mgr.find_unused();
	gl::light_manager::light& light = light_mgr.lights[light_node.light_index];
	light.used = true;

	light.active = (bool)conf_elm["active"];

	light.position.local = vec4(conf_elm["position"]);
	light.diffuse = vec4(conf_elm["diffuse"]);
	light.specular = vec4(conf_elm["specular"]);

	conf::elm& attenuation_array = conf_elm["attenuation"];
	light.attenuation.constant = (float)attenuation_array[0];
	light.attenuation.linear = (float)attenuation_array[1];
	light.attenuation.quadratic = (float)attenuation_array[2];

	light.spot.cutoff = (float)conf_elm["spot_cutoff"];
	light.spot.exponent = (float)conf_elm["spot_exponent"];
	light.spot.direction.local = vec3(conf_elm["direction"]);
}

node node::load_from_config(conf::elm& conf_elm, std::string name){
	node n{name};
	if(conf_elm.has_key("rotation")){
		conf::elm& array = conf_elm["rotation"];
		n.model_ctx.roll  = (float)array[0];
		n.model_ctx.pitch = (float)array[1];
		n.model_ctx.yaw   = (float)array[2];
	}
	
	if(conf_elm.has_key("scale")){
		n.model_ctx.scale = vec3(conf_elm["scale"]);
	}
	
	if(conf_elm.has_key("translation")){
		n.model_ctx.translation = vec3(conf_elm["translation"]);
	}
	
	if(conf_elm.has_key("type")){
		const std::string type = (std::string)conf_elm["type"];
		if(type == "body"){
			load_body(conf_elm, n);
		}else if(type == "light"){
			load_light(conf_elm, n);
		}else if(type == "camera"){
			n.type = type_t::CAMERA;
			n.cam_active = (bool)conf_elm["active"];
		}
	}
	
	for(std::string child_name : conf_elm.get_children()){
		n.children.push_back(load_from_config(conf_elm[child_name], child_name));
	}
	
	return n;
}

void node::draw(){
	std::cout << name << std::endl;
	context::use({draw_ctx, model_ctx}, [&](){
		static gl::state& state = singleton<gl::state>::value;
		switch(type){
			case type_t::LIGHT:
				static gl::light_manager& light_mgr = singleton<gl::light_manager>::value;
				light_mgr.lights[light_index].update_global(state.model);
				break;
			case type_t::BODY:
				drawable->draw();
				break;
			case type_t::CAMERA:
				if(cam_active){
					glm::mat4 t, r, s;
					decompose(state.model, t, r, s);
					state.view = glm::inverse(t * r);
				}
				break;
			default:
				break;
		}
		for(auto& child : children){
			child.draw();
		}
	});
}

node node::load_graph(std::string filename){
	conf::parser parser;
	parser.parse_file(filename);

	return load_from_config(parser.get_root()["root"], "root");
}

node& node::operator[](std::string name){
	for(auto& child : children){
		if(child.name == name){
			return child;
		}
	}
	throw std::out_of_range("name " + name + "doesn't exist!");
}

node::node(std::string name):
	name(name){}
