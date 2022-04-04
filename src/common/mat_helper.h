#ifndef _MAT_HELPER_H_
#define _MAT_HELPER_H_

#include "glm/glm.hpp"

inline void decompose(glm::mat4 mat, glm::mat4 &transl, glm::mat4 &rot, glm::mat4 &scale){
    transl = glm::mat4{1.0f};
    transl[3] = mat[3];
    mat[3] = { 0,0,0,1 };

    scale = glm::mat4{1.0f};
    for (int i = 0; i < 3; ++i){
        float length = glm::length(mat[i]);
        scale[i] *= length;
        mat[i] /= length;
    }
    
    rot = mat;
}

#endif
