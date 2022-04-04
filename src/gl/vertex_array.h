#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_
#include <vector>
#include <memory>
#include <string>

#include <GL/glew.h>

#include "gl_object.h"
#include "gl_buffer.h"
#include "program.h"
#include "common/context.h"

namespace gl{

	class vertex_array: public gl_object, public context{
		public:
			struct vertex_attribute{
				std::string name;
				GLint element_count;
				GLenum element_type;
			};
		private:
			static inline GLuint active_id = 0; // only used for context
			GLuint previous_active;  // only used for context
		
			vertex_array(
				const std::vector<vertex_attribute> attributes,
				const std::shared_ptr<program> prog,
				const std::shared_ptr<gl_buffer> vertex_buffer,
				const std::shared_ptr<gl_buffer> elem_buffer = nullptr);
			
			void ctx_begin() override;
			void ctx_end() override;
			
		public:
			~vertex_array();
			
			const std::shared_ptr<gl_buffer> elem_buffer = nullptr;
			const std::shared_ptr<gl_buffer> vertex_buffer = nullptr;

			static std::shared_ptr<vertex_array> create(
				const std::vector<vertex_attribute> attributes,
				const std::shared_ptr<program>,
				const std::shared_ptr<gl_buffer> vertex_buffer,
				const std::shared_ptr<gl_buffer> elem_buffer);
	};

}

#endif
