#include <iostream>
#include <cassert>
#include <vector>
#include <optional>
#include <fstream>
#include <memory>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "common/singleton.h"
#include "common/util.h"
#include "gl/gl_buffer_impl.h"
#include "gl/program.h"
#include "gl/vertex_array.h"
#include "gl/mesh.h"
#include "gl/window_info.h"
#include "gl/state.h"
#include "gl/capability.h"
#include "scene/state.h"
#include "scene/body.h"
#include "scene/sea.h"
#include "scene/node.h"
#include "input/key_listener.h"
#include "input/mouse_listener.h"
#include "init.h"
#include "logic.h"

namespace{
	enum class menu_id{
		MSAA,
		DEPTH_TEST,
		FRONT_FACE,
		BACKFACE_CULLING,
		BLENDING
	};

	void display_callback(){
		std::cout << "--------" << std::endl;
		glClearColor(0.2, 0.2, 0.4, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static input::key::pull_listener msaa{'y', true};
		static input::key::pull_listener depth_test{'x', true};
		static input::key::pull_listener backface_culling{'v', true};
		static input::key::pull_listener blending{'b', true};
		static input::key::pull_listener front_face{'c', true};

		capability msaa_cap{GL_MULTISAMPLE, msaa};
		capability depth_test_cap{GL_DEPTH_TEST, depth_test};
		capability backface_culling_cap{GL_CULL_FACE, backface_culling};
		capability blending_cap{GL_BLEND, blending};

		glFrontFace(front_face ? GL_CCW : GL_CW);

		context::use({msaa_cap, depth_test_cap, backface_culling_cap, blending_cap}, tick);
		
		glutSwapBuffers();
	}

	void resize_callback(int width, int height){
		singleton<gl::state>::value.projection = glm::perspective<double>(45, (double)width / (double)height, 1, 2000);
		glViewport ( 0, 0, width, height );
		singleton<window_info>::value.w = width;
		singleton<window_info>::value.h = height;
	}

	void idle_callback(){
		glutPostRedisplay();
	}

}

void init(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 800);
	glutInitWindowPosition(0, 0);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_ACCUM | GLUT_MULTISAMPLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Demo");
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Glew error: " << glewGetErrorString(err) << std::endl;
		assert(0);
	}

	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glDisable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    gl::texture::init();
	
	glutDisplayFunc(display_callback);
	glutReshapeFunc(resize_callback);
	glutMouseFunc(input::mouse::btn_listener::glut_btn_receiver);
	glutMotionFunc(input::mouse::move_listener::glut_move_receiver);
	glutPassiveMotionFunc(input::mouse::move_listener::glut_move_receiver);
	glutKeyboardFunc(input::key::listener::glut_down_receiver);
	glutKeyboardUpFunc(input::key::listener::glut_up_receiver);
	glutIdleFunc(idle_callback);
	glutIgnoreKeyRepeat(GL_TRUE);

	singleton<scene::state>::value.root = scene::graph::node::load_graph("./resources/scenegraph.conf");
	singleton<scene::state>::value.main_gui = gui::overlay::load_from_config("./resources/gui.conf");
	
	scene::graph::node sea_node{"sea"};
	sea_node.type = scene::graph::node::type_t::BODY;
	sea_node.drawable = std::shared_ptr<scene::body>(new scene::sea{});
	singleton<scene::state>::value.root.children.push_back(std::move(sea_node));
}
