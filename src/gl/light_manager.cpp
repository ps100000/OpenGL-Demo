#include "light_manager.h"

#include <stdexcept>
#include <iostream>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace gl;


void light_manager::light::update_global(glm::mat4 model){
	position.global = model * position.local;
	spot.direction.global = glm::mat3(model) * spot.direction.local;
}

std::function<void()> light_manager::bind(std::shared_ptr<program> prog){
	std::function<void()> binders[NUMBER_OF_LIGHTS];
	for(uint8_t i = 0; i < NUMBER_OF_LIGHTS; i++){
		binders[i] = bind(prog, i);
	}
	
	return [binders](){
		for(auto binder : binders){
			binder();
		}
	};
}

std::function<void()> light_manager::bind(std::shared_ptr<program> prog, uint8_t index){
	std::string entryname = std::string("u_lights[") + std::to_string(index) + "].";
	light& entry = lights[index];
	
	GLint active = prog->get_uniform(entryname + "on");

	GLint position = prog->get_uniform(entryname + "position");
	GLint diffuse = prog->get_uniform(entryname + "diffuse");
	GLint specular = prog->get_uniform(entryname + "specular");

	GLint attenuation_constant = prog->get_uniform(entryname + "attenuation_constant");
	GLint attenuation_linear = prog->get_uniform(entryname + "attenuation_linear");
	GLint attenuation_quadratic = prog->get_uniform(entryname + "attenuation_quadratic");

	GLint spot_cutoff = prog->get_uniform(entryname + "spot_cutoff");
	GLint spot_exponent = prog->get_uniform(entryname + "spot_exponent");
	GLint spot_direction = prog->get_uniform(entryname + "spot_direction");
	return [&entry, active, position, diffuse, specular, attenuation_constant,attenuation_linear,
		attenuation_quadratic, spot_cutoff, spot_exponent, spot_direction](){
			glUniform1i(active, entry.used && entry.active);
			glUniform4fv(position, 1, glm::value_ptr(entry.position.global));
			glUniform4fv(diffuse, 1, glm::value_ptr(entry.diffuse));
			glUniform4fv(specular, 1, glm::value_ptr(entry.specular));
			glUniform1f(attenuation_constant, entry.attenuation.constant);
			glUniform1f(attenuation_linear, entry.attenuation.linear);
			glUniform1f(attenuation_quadratic, entry.attenuation.quadratic);
			glUniform1f(spot_cutoff, entry.spot.cutoff);
			glUniform1f(spot_exponent, entry.spot.exponent);
			glUniform3fv(spot_direction, 1, glm::value_ptr(entry.spot.direction.global));
	};
}

uint8_t light_manager::find_unused(){
	for(uint8_t i = 0; i < NUMBER_OF_LIGHTS; i++){
		if(!lights[i].used){
			return i;
		}
	}
	throw std::out_of_range("no unused light could be found!");
}
