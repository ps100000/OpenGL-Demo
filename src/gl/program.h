#ifndef _PROGRAMM_H_
#define _PROGRAMM_H_
#include <string>
#include <memory>

#include <GL/glew.h>

#include "gl_object.h"
#include "common/context.h"

class program: public gl_object, public context {
	private:
		program(GLuint program_id);
		program(std::string vertexfile, std::string fragmentfile);

		GLint previous_program; // only used for context
		void ctx_begin() override;
		void ctx_end() override;
	public:

		program(const program&) = delete;
		program(program&&) = delete;
		~program();

		GLint get_attrib(std::string name);
		GLint get_uniform(std::string name);
		
		static std::shared_ptr<program> create(std::string vertexfile, std::string fragmentfile = "");

		static program none();
};

#endif
