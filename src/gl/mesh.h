#ifndef _MESH_H_
#define _MESH_H_
#include <memory>
#include <functional>

#include <GL/glew.h>

#include "common/context.h"
#include "program.h"
#include "vertex_array.h"
#include "gl_buffer_impl.h"

namespace gl{

	class mesh: public context{
		private:
			void ctx_begin() override;
			void ctx_end() override;
		public:
			std::shared_ptr<gl_um_buffer<GLuint>> index_buffer;
			std::shared_ptr<program> prog;
			std::shared_ptr<gl::vertex_array> v_array;
			GLenum drawing_mode;

			std::function<void(std::shared_ptr<program> prog)> begin_func;
			std::function<void(std::shared_ptr<program> prog)> end_func;

			mesh() = default;
			mesh(
				std::shared_ptr<program> prog,
				std::shared_ptr<gl::vertex_array> v_array,
				GLenum drawing_mode = GL_TRIANGLES,
				std::function<void(std::shared_ptr<program> prog)> begin_func =
					(std::function<void(std::shared_ptr<program>)>)[](std::shared_ptr<program>){},
				std::function<void(std::shared_ptr<program> prog)> end_func =
					(std::function<void(std::shared_ptr<program>)>)[](std::shared_ptr<program>){});

			void draw();
			void draw(size_t offset, size_t count);
	};

}

#endif
