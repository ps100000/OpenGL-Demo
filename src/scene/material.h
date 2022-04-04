#ifndef _SCENE__MATERIAL_H_
#define _SCENE__MATERIAL_H_

#include <string>

#include "glm/glm.hpp"

namespace scene{
    class material{
        public:
            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;
            float shininess;

            material() = default;
            material(std::string filename);
    };
}

#endif
