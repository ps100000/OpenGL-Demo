#include "shader.h"

#include <iostream>
#include <cstring>
#include <cassert>

#include "common/util.h"
#include "gl/gl_util.h"

using namespace gl;

shader::shader(): gl_object(){}

shader::shader(GLuint shader_id): gl_object(shader_id){}

shader::shader(const std::string filename, GLenum type){
    const std::optional<std::string> source = file_read(filename);
    if(!source){
        std::cerr << "Error opening " << filename << ": " << strerror(errno) << std::endl;
        gl_id = 0;
        assert(0);
    }
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = {
        // Define GLSL version
#ifdef __EMSCRIPTEN__
        "#version 300 es\n"  // OpenGL ES 2.0
#else
        "#version 320 es\n"  // OpenGL 2.1
#endif
        ,
        // GLES2 precision specifiers

        // Define default float precision for fragment shaders:
        (type == GL_FRAGMENT_SHADER) ?
        "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
        "precision highp float;           \n"
        "#else                            \n"
        "precision mediump float;         \n"
        "#endif                           \n"
        : ""
        // Note: OpenGL ES automatically defines this:
        // #define GL_ES

        ,
        &source.value()[0] };
    glShaderSource(res, 3, sources, NULL);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if(compile_ok == GL_FALSE){
        std::cerr << filename << ":" << std::endl;;
        print_log(res);
        glDeleteShader(res);
        gl_id = 0;
        assert(0);
    }

    gl_id = res;
}
