#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>

#include <GL/glew.h>

#include "gl_object.h"

class shader: public gl_object{
	public:
		shader();
		shader(GLuint shader_id);
		shader(const std::string filename, GLenum type);
};

#endif
