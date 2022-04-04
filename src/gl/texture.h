#ifndef _GL__TEXTURE_H_
#define _GL__TEXTURE_H_
#include <string>
#include <memory>

#include <GL/glew.h>

#include "gl_object.h"
#include "common/context.h"

namespace gl{
	class texture: public gl_object, public context {
		private:
			texture(
				GLint internalformat,
				GLenum format,
				GLsizei width,
				GLsizei height,
				GLsizei depth,
				GLenum type,
				const void * data,
				bool interpolate,
				bool repeat);
			static inline GLint max_units = 0;

			GLint previous_texture; // only used for context
			void ctx_begin() override;
			void ctx_end() override;
		public:
			texture(const texture&) = delete;
			texture(texture&&) = delete;
			~texture();

			const uint8_t dimensions;
			const uint16_t width;
			const uint16_t height;
			GLenum target;

			operator GLuint();

			static void init();

			static std::shared_ptr<texture> create(
				GLint internalformat,
				GLenum format,
				GLsizei width,
				GLsizei height,
				GLsizei depth,
				GLenum type,
				const void * data,
				bool interpolate = true,
				bool repeat = false);
			static std::shared_ptr<texture> create(
				std::string path,
				bool interpolate = true,
				bool repeat = false);

			static std::shared_ptr<texture> create(
				std::vector<std::string> paths,
				bool interpolate = true,
				bool repeat = false);
	};
}
#endif
