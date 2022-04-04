#include "gl_buffer.h"

#include <iostream>
#include <cassert>

gl_buffer::gl_buffer(
	size_t size,
	size_t elm_size,
	bool mapped,
	GLuint bind_target,
	const void * data,
	GLenum usage):
		size(size),
		mapped(mapped),
		bind_target(bind_target) {
		glGenBuffers(1, &gl_id);
		glBindBuffer(bind_target, gl_id);
        glBufferData(bind_target, size * elm_size, data, usage);
	}


void gl_buffer::ctx_begin() {
	GLuint query = 0;
	switch(bind_target){
		case GL_ARRAY_BUFFER:
			query = GL_ARRAY_BUFFER_BINDING;
			break;
		case GL_ELEMENT_ARRAY_BUFFER:
			query = GL_ELEMENT_ARRAY_BUFFER_BINDING;
			break;
		default:
			std::cerr
				<< "glGet query for buffer binding target "
				<< bind_target
				<< " missing. please add it at "
				<< __FILE__
				<< ":"
				<< __LINE__ - 2
				<< std::endl;
			assert(0);
	}
	glGetIntegerv(query, &previous_buffer);
	glBindBuffer(bind_target, gl_id);
}

void gl_buffer::ctx_end() {
	glBindBuffer(bind_target, previous_buffer);
}

size_t gl_buffer::get_elm_size(){
	assert(0);
}

gl_buffer::~gl_buffer(){
	glDeleteBuffers(1, &gl_id);
}

