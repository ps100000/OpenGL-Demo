#include "gl_util.h"

#include <iostream>
#include <cassert>

using namespace gl;

void gl::print_log(GLuint object){
    GLint log_length = 0;
    if(glIsShader(object)){
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }else if(glIsProgram(object)){
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }else {
        std::cerr << "printlog: Not a shader or a program" << std::endl;
        return;
    }

    std::string log(log_length, '\0');

    if(glIsShader(object)){
        glGetShaderInfoLog(object, log_length, NULL, &log[0]);
    }else if(glIsProgram(object)){
        glGetProgramInfoLog(object, log_length, NULL, &log[0]);
    }
    std::cerr << "log from object " << object << ":" << std::endl;
    std::cerr << log << std::endl;
}

size_t gl::size_from_type(GLenum type){
	switch(type){
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return 1;
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_HALF_FLOAT:
			return 2;
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_FIXED:
		case GL_FLOAT:
			return 4;
		case GL_DOUBLE: 
			return 8;
		default:
			assert(0);
		}
}
