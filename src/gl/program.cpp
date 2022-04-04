#include "program.h"

#include <iostream>
#include <optional>
#include <cstring>
#include <cassert>

#include "gl_util.h"
#include "shader.h"
#include "common/util.h"

using namespace gl;

program::program(GLuint program_id): gl_object(program_id){}

program::program(std::string vertexfile, std::string fragmentfile){
        GLuint program = glCreateProgram();
        shader current_shader;


        if(!vertexfile.empty()){
                current_shader = shader(vertexfile, GL_VERTEX_SHADER);
                if(!current_shader){
                    gl_id = 0;
                }
                glAttachShader(program, current_shader);
        }

        if(!fragmentfile.empty()){
                current_shader = shader(fragmentfile, GL_FRAGMENT_SHADER);
                if(!current_shader){
                    gl_id = 0;
                }
                glAttachShader(program, current_shader);
        }

        glLinkProgram(program);
        GLint link_ok = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
        if(!link_ok){
                std::cerr << "glLinkProgram:" << std::endl;
                print_log(program);
                glDeleteProgram(program);
                gl_id = 0;
                assert(0);
        }

        gl_id = program;
}

void program::ctx_begin(){
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    glUseProgram(gl_id);
}

void program::ctx_end(){
    glUseProgram(previous_program);
}

program::~program(){
    glDeleteProgram(gl_id);
}

GLint program::get_attrib(std::string name){
        GLint attribute = glGetAttribLocation(gl_id, name.c_str());
        if(attribute == -1){
            std::cerr << "Could not bind attribute " << name << std::endl;
            assert(0);
        }
        return attribute;
}

GLint program::get_uniform(std::string name){
        GLint uniform = glGetUniformLocation(gl_id, name.c_str());
        if(uniform == -1){
            std::cerr << "Could not bind uniform " << name << std::endl;
            assert(0);
        }
        return uniform;
}

std::shared_ptr<program> program::create(std::string vertexfile, std::string fragmentfile){
    return std::shared_ptr<program>(new program(vertexfile, fragmentfile));
}

program program::none(){
    return program(0);
}
