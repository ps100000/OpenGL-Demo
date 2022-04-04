#include "sea.h"

#include "gl/light_manager.h"

using namespace scene;

std::shared_ptr<program> sea::get_prog(){
    static std::shared_ptr<program> prog = program::create("./resources/shaders/sea.v.glsl", "./resources/shaders/sea.f.glsl");

    return prog;
}

sea::sea(): body(body::load_obj_file("./resources/models/sea/seaplane.obj"), get_prog()){
    if(!heightmap){
        heightmap = gl::texture::create(std::vector<std::string>{
            "./resources/models/sea/height0.png",
            "./resources/models/sea/height1.png",
            "./resources/models/sea/height3.png",
            "./resources/models/sea/height4.png",
            "./resources/models/sea/height5.png",
            "./resources/models/sea/height6.png",
            "./resources/models/sea/height7.png"
        });
    }
}

void sea::draw(){
    static std::shared_ptr<program> prog = get_prog();
    static GLint u_f_progress = prog->get_uniform("u_f_progress");
	static GLint u_b_heightmap = prog->get_uniform("u_b_heightmap");
	static GLint u_s_heightmap = prog->get_uniform("u_s_heightmap");

    prog->use([this](){
        glUniform1f(u_f_progress, progress);
        glUniform1i(u_b_heightmap, waves);
        glUniform1i(u_s_heightmap, (GLuint)*heightmap);
    });
    heightmap->use([this](){
        body::draw();
    });
}

void sea::bind_lights(){
    static std::shared_ptr<program> prog = get_prog();
	static std::function<void()> binder = singleton<gl::light_manager>::value.bind(prog); 
	prog->use([=](){
		binder();
	});
}