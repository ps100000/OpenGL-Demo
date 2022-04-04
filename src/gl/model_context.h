#ifndef _GL__MODEL_CONTEXT_H_
#define _GL__MODEL_CONTEXT_H_
#include <string>
#include <memory>

#include "state.h"
#include "common/singleton.h"
#include "common/context.h"
#include "common/gs_val.h"

namespace gl{
	
	class model_context: public context {
		protected:
			glm::mat4 previous_mat;
			void ctx_begin() override;
			void ctx_end() override;
		public:
			model_context(std::function<glm::mat4(glm::mat4)> change = [](glm::mat4 m){return m;});
			std::function<glm::mat4(glm::mat4)> change;

			glm::mat4 last_inner;
	};
	
	class rst_model_context: public model_context {
		private:
			template<typename T>
			constexpr typename gs_val<T>::setter_func setter();
			void update();
		public:
			gs_val<float> roll;
			gs_val<float> pitch;
			gs_val<float> yaw;
			
			gs_val<glm::vec3> scale;

			gs_val<glm::vec3> translation;
			rst_model_context();
			rst_model_context(rst_model_context& ref);
			rst_model_context(rst_model_context&& ref);
			rst_model_context& operator=(rst_model_context& ref);
			rst_model_context& operator=(rst_model_context&& ref);
	};

}

#endif
