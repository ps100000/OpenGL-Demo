#ifndef _GL_UTIL_H_
#define _GL_UTIL_H_
#include <GL/glew.h>

namespace gl {
	
	void print_log(GLuint object);
	size_t size_from_type(GLenum type);

}

#endif
