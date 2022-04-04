#include <iostream>

#include "glm/gtx/string_cast.hpp"

#include "state.h"

using namespace gl;

void state::set_view(glm::vec3 up, float bearing, float tilt){
	// rotate around to a given bearing: yaw
	glm::mat4 camera = glm::rotate(glm::mat4(), bearing, up);
	// Define the 'look up' axis, should be orthogonal to the up axis
	glm::vec3 pitch_vector = glm::vec3(1, 0, 0);
	// rotate around to the required head tilt: pitch
	camera = glm::rotate<float>(camera, tilt, pitch_vector);

	// now get the view matrix by taking the camera inverse
	view = glm::inverse(camera);
};

glm::mat4 state::get_mvp(){
	//std::cout << "model " << glm::to_string(model) << std::endl;
	return projection * view * model;
}

glm::mat3 state::get_normal_matrix(){ // https://stackoverflow.com/a/17402277
	return (glm::inverse(glm::mat3(model)));
}

