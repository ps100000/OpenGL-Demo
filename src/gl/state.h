#ifndef _GL__STATE_H_
#define _GL__STATE_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "common/singleton.h"

namespace gl {
	class state{
		private:
			state() = default;
		public:
			friend singleton<state>;
			glm::mat4 model = glm::mat4{1.0};
			glm::mat4 view = glm::mat4{1.0};
			glm::mat4 projection = glm::mat4{1.0};
			
			void set_view(glm::vec3 up, float bearing, float tilt);
			
			glm::mat4 get_mvp();
			glm::mat3 get_normal_matrix();
		};

}

#endif
