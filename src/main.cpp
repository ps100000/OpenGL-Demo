#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include <GL/glew.h>
#include <GL/glut.h>

#include "common/util.h"
#include "init.h"
#include "common/singleton.h"
#include "gl/texture.h"
#include "gl/program.h"
#include "gl/gl_util.h"
#include "gl/window_info.h"

#define GLM_FORCE_RADIANS


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void set_window_size(uint16_t w, uint16_t h){
    if(
        singleton<window_info>::value.w != w
        || singleton<window_info>::value.h != h){
        singleton<window_info>::value.w = w;
        singleton<window_info>::value.h = h;
        glViewport(0, 0, w, h);
    }
}

int init_resources(){
    gl::texture::init();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnable(GL_LINE_SMOOTH);
    set_window_size(1024, 600);
    
    return 1;
}

int main(int argc, char **argv){
    init(argc, argv);
    GLenum glew_status = glewInit();

    if(GLEW_OK != glew_status){
            std::cerr << "Error: " << glewGetErrorString(glew_status) << std::endl;
            return 1;
    }

    if(!GLEW_VERSION_3_2){
            std::cerr << "No support for OpenGL 3.2 found" << std::endl;
            return 1;
    }

    if(!init_resources())
            return EXIT_FAILURE;

    glutMainLoop();

    return EXIT_SUCCESS;
}
