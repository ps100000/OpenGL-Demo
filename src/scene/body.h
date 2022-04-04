#ifndef _SCENE__BODY_H_
#define _SCENE__BODY_H_

#include <memory>

#include "gl/mesh.h"
#include "gl/texture.h"
#include "gl/program.h"
#include "gl/model_context.h"
#include "material.h"

namespace scene{
	
	class body: public gl::rst_model_context{
		public:
#pragma pack(push, 1)
			struct vertex{
				glm::vec3 pos;
				glm::vec2 tex;
				glm::vec3 norm;
			};
#pragma pack(pop)
		private:
			static inline std::shared_ptr<program> default_prog = nullptr;
		protected:
			std::shared_ptr<program> prog;
			gl::mesh geometry;
			
			std::vector<vertex> load_obj_file(std::string filename);
		public:
			material base_material;
			std::shared_ptr<gl::texture> texture = nullptr;
			std::shared_ptr<gl::texture> normalmap = nullptr;
			body(std::string filename);
			body(const std::vector<vertex>& vertices, std::shared_ptr<program> prog = nullptr /*mapped to body::default_prog*/);
			
			virtual void draw();
			
			static void bind_lights();
	};
};
#endif
