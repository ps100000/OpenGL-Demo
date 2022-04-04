#ifndef _SCENE__GRAPH__NODE_H_
#define _SCENE__GRAPH__NODE_H_

#include <vector>
#include <string>
#include <optional>
#include <memory>

#include "body.h"
#include "common/context.h"
#include "conf/elm.h"
#include "gl/model_context.h"

namespace scene::graph{
	class node{
		public:
			enum class type_t{
				NONE, BODY, LIGHT, CAMERA
			};
		private:
			static void load_body(conf::elm& conf_elm, node& body_node);
			static void load_light(conf::elm& conf_elm, node& light_node);
			static node load_from_config(conf::elm& conf_elm, std::string name);
		public:
			//std::shared_ptr<node> parent;
			std::string name;
			std::vector<node> children;
			
			gl::rst_model_context model_ctx;
			generic_context draw_ctx;

			type_t type = type_t::NONE;
			std::shared_ptr<body> drawable; // type_t::BODY
			uint8_t light_index; // type_t::LIGHT
			bool cam_active; // type_t::CAMERA

			void draw();
			
			node& operator[](std::string name);
			
			node(std::string name = "unnamed");
			
			static node load_graph(std::string filename);
	};
};


#endif
