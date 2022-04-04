#ifndef _GL_BUFFER_H_
#define _GL_BUFFER_H_
#include <memory>
#include <vector>
#include <iostream>

#include <GL/glew.h>

#include "common/context.h"
#include "gl_object.h"

class gl_buffer: public gl_object, public context{
	protected:
		gl_buffer(
			size_t size,
			size_t elm_size,
			bool mapped,
			GLuint bind_target,
			const void * data,
			GLenum usage);

		GLint previous_buffer; // only used for context
		void ctx_begin() override;
		void ctx_end() override;
	public:
		const size_t size;
		const bool mapped;
		const GLuint bind_target;

		virtual size_t get_elm_size();
		virtual ~gl_buffer();
};

template<typename T>
class gl_m_buffer: public gl_buffer{
	private:
		gl_m_buffer(
			size_t size,
			GLuint bind_target,
			const void * data,
			GLenum usage);
		const void * data;
	public:
		gl_m_buffer(const gl_m_buffer&) = delete;
		gl_m_buffer(gl_m_buffer&&) = delete;
		~gl_m_buffer();

		explicit operator T*(){
			return (T*)data;
		}

		static std::shared_ptr<gl_m_buffer<T>> create(
			size_t size,
			GLuint bind_target,
			const void * data = nullptr,
			GLenum usage = GL_STATIC_DRAW);

		static std::shared_ptr<gl_m_buffer<T>> create(
			const std::vector<T>& data,
			GLuint bind_target,
			GLenum usage = GL_STATIC_DRAW);
			
		size_t get_elm_size() override;
};

template<typename T>
class gl_um_buffer: public gl_buffer{
	private:
		gl_um_buffer(
			size_t size,
			GLuint bind_target,
			const void * data,
			GLenum usage);

	public:
		gl_um_buffer(const gl_um_buffer&) = delete;
		gl_um_buffer(gl_um_buffer&&) = delete;

		static std::shared_ptr<gl_um_buffer<T>> create(
			size_t size,
			GLuint bind_target,
			const void * data = nullptr,
			GLenum usage = GL_STATIC_DRAW);

		static std::shared_ptr<gl_um_buffer<T>> create(
			const std::vector<T>& data,
			GLuint bind_target,
			GLenum usage = GL_STATIC_DRAW);

		size_t get_elm_size() override;

		std::vector<T> get(size_t offset, size_t length);
		T get(size_t index);

		void set(size_t offset, std::vector<T>& data);
		void set(size_t offset, T* data, size_t size);
		void set(size_t index, T data);
};

#endif
