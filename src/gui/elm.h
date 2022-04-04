#ifndef _GUI__ELM_H_
#define _GUI__ELM_H_

#include <string>

#include "glm/glm.hpp"

#include "gl/program.h"
#include "gl/mesh.h"
#include "gl/texture.h"
#include "gl/texture.h"
#include "common/gs_val.h"

namespace gui{
    class elm{
        private:
            static inline std::shared_ptr<program> prog = nullptr;
            static inline glm::mat3 inv_window_size_scale = glm::mat3{1.0f};
            
            gl::mesh square;
            glm::mat3 rst;
			
			template<typename T>
			constexpr typename gs_val<T>::setter_func setter();
			void update();
		public:
            std::string name;
			std::shared_ptr<gl::texture> texture = nullptr;
            uint16_t z_index = 0;
            bool visible = true;

			gs_val<float> rotation;
			//gs_val<float> width;
			//gs_val<float> height;
			//gs_val<glm::vec2> offset;
			glm::vec2 alignment;
			gs_val<glm::vec2> center;
			gs_val<glm::vec2> scale;        // todo dynamic?
			gs_val<glm::vec2> translation;  // todo dynamic?

            void draw();

            elm(std::string filename, std::string name = "unnamed");
            elm(std::shared_ptr<gl::texture> texture, std::string name = "unnamed");
			elm(elm& ref);
			elm(elm&& ref);
			elm& operator=(elm& ref);
			elm& operator=(elm&& ref);

            static void set_window_size(int w, int h);
    };
}

#endif
