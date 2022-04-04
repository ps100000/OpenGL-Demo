#ifndef _GL__LIGHTS_H_
#define _GL__LIGHTS_H_

#include <memory>
#include <array>

#include "glm/glm.hpp"

#include "program.h"
#include "common/singleton.h"

namespace gl {
	class light_manager{
		public:
			static constexpr uint8_t NUMBER_OF_LIGHTS = 8;
			struct light{
					bool used; // only loader
					
					bool active;

					struct{
						glm::vec4 local;
						glm::vec4 global;
					}position;
					glm::vec4 diffuse;
					glm::vec4 specular;
					struct{
						float constant;
						float linear;
						float quadratic;
					}attenuation;
					struct{
						float cutoff;
						float exponent;
						struct{
							glm::vec3 local;
							glm::vec3 global;
						}direction;
					}spot;

					void update_global(glm::mat4 model);
			};
		private:
			light_manager() = default;
		public:
			friend singleton<light_manager>;
		
			std::array<light, NUMBER_OF_LIGHTS> lights;
		
			std::function<void()> bind(std::shared_ptr<program> prog);
			std::function<void()> bind(std::shared_ptr<program> prog, uint8_t index);
			
			uint8_t find_unused();
	};
}

#endif
