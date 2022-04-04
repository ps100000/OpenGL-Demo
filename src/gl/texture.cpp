#include "texture.h"

#include <iostream>
#include <cassert>

#include "lodepng/lodepng.h"

using namespace gl;

texture::texture(
	GLint internalformat,
	GLenum format,
	GLsizei width,
	GLsizei height,
	GLsizei depth,
	GLenum type,
	const void * data,
	bool interpolate,
	bool repeat
	): gl_object(),
	dimensions(1 + (height > 1) + (height > 1 && depth > 1)),
	width(width),
	height(height),
	target(dimensions == 1 ? GL_TEXTURE_1D : dimensions == 2 ? GL_TEXTURE_2D : GL_TEXTURE_3D) {
		GLint former_active_texture;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &former_active_texture);

        glGenTextures(1, &gl_id);
        glActiveTexture(GL_TEXTURE0 + (gl_id % max_units));
        glBindTexture(target, gl_id);
        
		switch(dimensions){
			case 3:
				glTexParameteri(target, GL_TEXTURE_WRAP_R, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
     			[[fallthrough]];
			case 2:
	        	glTexParameteri(target, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
     			[[fallthrough]];
			case 1:
        		glTexParameteri(target, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
     			[[fallthrough]];
			default:
				break;
		}

        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);

		
		switch(dimensions){
			case 1:
				glTexImage1D(target, 0, internalformat, width, 0, format, type, data);
				break;
			case 2:
       			glTexImage2D(target, 0, internalformat, width, height, 0, format, type, data);
				break;
			case 3:
				glTexImage3D(target, 0, internalformat, width, height, depth, 0, format, type, data);
				break;
			default:
				break;
		}

        glActiveTexture(former_active_texture);
}

void texture::ctx_begin(){
	glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_texture);
	glActiveTexture(GL_TEXTURE0 + (gl_id % max_units));
    glBindTexture(target, gl_id);
}

void texture::ctx_end(){
	glActiveTexture(previous_texture);
}

texture::~texture(){
	glDeleteTextures(1, &gl_id);
}

texture::operator GLuint(){
	return gl_id % max_units;
}

void texture::init(){
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_units);
        std::cout << "max_units" << max_units << std::endl;
}

std::shared_ptr<texture> texture::create(
	GLint internalformat,
	GLenum format,
	GLsizei width,
	GLsizei height,
	GLsizei depth,
	GLenum type,
	const void * data,
	bool interpolate,
	bool repeat){
	return std::shared_ptr<texture>(new texture(
		internalformat,
		format,
		width,
		height,
		depth,
		type,
		data,
		interpolate,
		repeat));
}


std::shared_ptr<texture> texture::create(
	std::string path,
	bool interpolate,
	bool repeat){
	
	return create(std::vector<std::string>{path}, interpolate, repeat);
}


std::shared_ptr<texture> texture::create(
	std::vector<std::string> paths,
	bool interpolate,
	bool repeat){
	assert(paths.size());

	std::vector<unsigned char> images;
	unsigned int width, height;

	unsigned int error = lodepng::decode(images, width, height, paths[0]);
	if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    for(auto path_iter = paths.begin() + 1; path_iter != paths.end(); path_iter++){
		unsigned int w, h;
		error = lodepng::decode(images, w, h, *path_iter);
		assert(width == w);
		assert(height == w);
		if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	return create(
		GL_RGBA,
		GL_RGBA,
		width,
		height,
		paths.size(),
		GL_UNSIGNED_BYTE,
		images.data(),
		interpolate,
		repeat);
}
