#ifndef _GL_BUFFER_IMPL_H_
#define _GL_BUFFER_IMPL_H_
#include <iostream>

#include "gl_buffer.h"

template<typename T>
gl_m_buffer<T>::gl_m_buffer(
	size_t size,
	GLuint bind_target,
	const void * data,
	GLenum usage): gl_buffer(size, sizeof(T), false, bind_target, data, usage){
	//https://www.bfilipek.com/2015/01/persistent-mapped-buffers-in-opengl.html
	throw std::exception();
	data = glMapBuffer(bind_target, GL_READ_WRITE);
}

template<typename T>
gl_m_buffer<T>::~gl_m_buffer(){
	glBindBuffer(bind_target, gl_id);
	glUnmapBuffer(bind_target);
}


template<typename T>
std::shared_ptr<gl_m_buffer<T>> gl_m_buffer<T>::create(
	size_t size,
	GLuint bind_target,
	const void * data,
	GLenum usage){
	return std::shared_ptr<gl_m_buffer<T>>(new gl_m_buffer<T>(
		size,
		bind_target,
		data,
		usage));
}

template<typename T>
std::shared_ptr<gl_m_buffer<T>> gl_m_buffer<T>::create(
	const std::vector<T>& data,
	GLuint bind_target,
	GLenum usage){
	return create(data.size(), bind_target, data.data(), usage);
}

template<typename T>
size_t gl_m_buffer<T>::get_elm_size(){
	return sizeof(T);
}

/**************************************************************/

template<typename T>
gl_um_buffer<T>::gl_um_buffer(
	size_t size,
	GLuint bind_target,
	const void * data,
	GLenum usage): gl_buffer(size, sizeof(T), false, bind_target, data, usage){}

template<typename T>
std::shared_ptr<gl_um_buffer<T>> gl_um_buffer<T>::create(
	size_t size,
	GLuint bind_target,
	const void * data,
	GLenum usage){
	return std::shared_ptr<gl_um_buffer<T>>(new gl_um_buffer<T>(
		size,
		bind_target,
		data,
		usage));
}

template<typename T>
std::shared_ptr<gl_um_buffer<T>> gl_um_buffer<T>::create(
	const std::vector<T>& data,
	GLuint bind_target,
	GLenum usage) {
	return create(data.size(), bind_target, data.data(), usage);
}

template<typename T>
size_t gl_um_buffer<T>::get_elm_size(){
	return sizeof(T);
}

template<typename T>
std::vector<T> gl_um_buffer<T>::get(size_t offset, size_t length){
	std::vector<T> data(length);
	ctx_begin();
	glGetBufferSubData(bind_target, offset * sizeof(T), length * sizeof(T), data.data());
	ctx_end();
	return data;
}

template<typename T>
T gl_um_buffer<T>::get(size_t index){
	T value;
	ctx_begin();
	glGetBufferSubData(bind_target, index * sizeof(T), 1 * sizeof(T), &value);
	ctx_end();
	return value;
}

template<typename T>
void gl_um_buffer<T>::set(size_t offset, std::vector<T>& data){
	ctx_begin();
	glBufferSubData(bind_target, offset * sizeof(T), data.size() * sizeof(T), data.data());
	ctx_end();
}

template<typename T>
void gl_um_buffer<T>::set(size_t offset, T* data, size_t size){
	ctx_begin();
	glBufferSubData(bind_target, offset * sizeof(T), size * sizeof(T), data);
	ctx_end();
}

template<typename T>
void gl_um_buffer<T>::set(size_t index, T data){
	ctx_begin();
	glBufferSubData(bind_target, index * sizeof(T), 1 * sizeof(T), &data);
	ctx_end();
}

#endif
