#include "elm.h"

#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "gl/capability.h"

using namespace gui;

template<typename T>
constexpr typename gs_val<T>::setter_func elm::setter(){
	return [this](T& var, T val){var = val; update();};
}

void elm::update(){
	glm::mat3 new_rst{1.0f};
	new_rst = glm::translate(new_rst, (glm::vec2)translation);
	new_rst = glm::rotate(new_rst, (float)rotation);
	new_rst = glm::translate(new_rst, -(glm::vec2)scale * (glm::vec2)center);
	new_rst = glm::scale(new_rst, (glm::vec2)scale * glm::vec2{texture->width, texture->height});
	rst = new_rst;
}

void elm::set_window_size(int w, int h){
	inv_window_size_scale = glm::scale(glm::mat3{1.0f}, glm::vec2{2.0f / w, 2.0f / h});
}

void elm::draw(){
    if(!visible){
        return;
    }
    static capability depth(GL_DEPTH_TEST, false);
    context::use({depth, *texture},[this](){square.draw();});
}

elm::elm(std::string filename, std::string name):
    elm(gl::texture::create(filename), name){}

elm::elm(std::shared_ptr<gl::texture> tex, std::string name):
    name(name),
    texture(tex),
	rotation(gs_val<float>::std_getter, setter<float>(), 0),
    alignment(0.0f, 0.0f),
	center(gs_val<glm::vec2>::std_getter, setter<glm::vec2>(), {0, 0}),
	scale(gs_val<glm::vec2>::std_getter, setter<glm::vec2>(), {1.0f, 1.0f}),
	translation(gs_val<glm::vec2>::std_getter, setter<glm::vec2>(), {0, 0}){

    if(!prog){
        prog = program::create("./resources/shaders/gui.v.glsl", "./resources/shaders/gui.f.glsl");
    }
    std::shared_ptr<gl_um_buffer<glm::vec2>> vertices_buf = gl_um_buffer<glm::vec2>::create(
        {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
        },
        GL_ARRAY_BUFFER
    );
    std::shared_ptr<gl::vertex_array> v_array = gl::vertex_array::create(
        {
            {
                "i_v_coord",
                2,
                GL_FLOAT
            }
        },
        prog,
        vertices_buf,
        nullptr
    );

    GLint u_mvp = prog->get_uniform("u_m_rst");
    GLint u_s_texture = prog->get_uniform("u_s_texture");

    square = gl::mesh{
        prog,
        v_array,
        GL_TRIANGLE_FAN,
        (std::function<void(std::shared_ptr<program>)>)
        [this, u_mvp, u_s_texture](std::shared_ptr<program>){
            glm::mat3 combined = inv_window_size_scale * rst;
            combined[2][0] += alignment[0];
            combined[2][1] += alignment[1];
            glUniformMatrix3fv(u_mvp, 1, false, glm::value_ptr(combined));
            glUniform1i(u_s_texture, (GLuint)*texture);
        }
    };

    update();
}

elm::elm(elm& ref): elm(ref.texture, ref.name){
	*this = ref;
}

elm::elm(elm&& ref): elm((elm&)ref){
}

elm& elm::operator=(elm& ref){
    name = ref.name;
    texture = ref.texture;
    z_index = ref.z_index;
    visible = ref.visible;
	rotation = (float)ref.rotation;
    alignment = ref.alignment;
	center = (glm::vec2)ref.center;
	scale = (glm::vec2)ref.scale;
	translation = (glm::vec2)ref.translation;
	
	return *this;
}

elm& elm::operator=(elm&& ref){
    return operator=((elm&)ref);
}

