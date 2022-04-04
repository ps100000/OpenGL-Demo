#include <iostream>

#include "glm/gtx/string_cast.hpp"

#include "model_context.h"

using namespace gl;

void model_context::ctx_begin(){
	previous_mat = singleton<state>::value.model;
	last_inner = change(previous_mat);
	singleton<state>::value.model = last_inner;
}

void model_context::ctx_end(){
	singleton<state>::value.model = previous_mat;
}

model_context::model_context(std::function<glm::mat4(glm::mat4)> change): change(change){}

/**************************************************************/

template<typename T>
constexpr typename gs_val<T>::setter_func rst_model_context::setter(){
	return [this](T& var, T val){var = val; update();};
}

void rst_model_context::update(){
	glm::mat4 rst{1.0f};
	rst = glm::translate(rst, (glm::vec3)translation);
	rst = glm::rotate(rst, (float)yaw,   {0, 0, 1.0f});
	rst = glm::rotate(rst, (float)pitch, {0, 1.0f, 0});
	rst = glm::rotate(rst, (float)roll,  {1.0f, 0, 0});
	rst = glm::scale(rst, (glm::vec3)scale);
	change = [rst](glm::mat4 m){return m * rst;};
}

rst_model_context::rst_model_context():
	model_context(),
	roll(gs_val<float>::std_getter, setter<float>(), 0),
	pitch(gs_val<float>::std_getter, setter<float>(), 0),
	yaw(gs_val<float>::std_getter, setter<float>(), 0),
	scale(gs_val<glm::vec3>::std_getter, setter<glm::vec3>(), {1.0f, 1.0f, 1.0f}),
	translation(gs_val<glm::vec3>::std_getter, setter<glm::vec3>(), {0, 0, 0}){
}

rst_model_context::rst_model_context(rst_model_context& ref): rst_model_context(){
	*this = ref;
}

rst_model_context::rst_model_context(rst_model_context&& ref): rst_model_context((rst_model_context&)ref){}

rst_model_context& rst_model_context::operator=(rst_model_context& ref){
	roll = (float)ref.roll;
	pitch = (float)ref.pitch;
	yaw = (float)ref.yaw;
	scale = (glm::vec3)ref.scale;
	translation = (glm::vec3)ref.translation;
	update();
	
	return *this;
}

rst_model_context& rst_model_context::operator=(rst_model_context&& ref){
	return operator=((rst_model_context&)ref);
}
