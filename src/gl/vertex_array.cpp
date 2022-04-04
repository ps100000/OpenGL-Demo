#include "vertex_array.h"
#include "gl_util.h"

using namespace gl;

namespace {
	struct i_vertex_attribute{
		GLint index;
		GLint element_count;
		GLenum element_type;
	};
}

vertex_array::vertex_array(
				const std::vector<vertex_attribute> in_attributes,
				const std::shared_ptr<program> prog,
				const std::shared_ptr<gl_buffer> vertex_buffer,
				const std::shared_ptr<gl_buffer> elem_buffer):
					elem_buffer(elem_buffer),
					vertex_buffer(vertex_buffer){
    glGenVertexArrays(1, &gl_id);
    ctx_begin();
    if(elem_buffer){
		((context&)*elem_buffer).ctx_begin();
	}
	((context&)*vertex_buffer).ctx_begin();
    
	size_t total_attributes_size = 0;

	std::vector<i_vertex_attribute> vertex_attributes;
	for(auto& v_attribute : in_attributes){
		vertex_attributes.push_back({
			prog->get_attrib(v_attribute.name),
			v_attribute.element_count,
			v_attribute.element_type
		});
		total_attributes_size += size_from_type(v_attribute.element_type) * v_attribute.element_count;
	}
	
	std::sort(vertex_attributes.begin(), vertex_attributes.end(),[](i_vertex_attribute& a, i_vertex_attribute& b){
		return b.index > a.index;
	});
	
	size_t accumulated_size = 0;
	for(auto v_attribute : vertex_attributes){
        glVertexAttribPointer(
        	v_attribute.index,
        	v_attribute.element_count,
        	v_attribute.element_type,
        	GL_FALSE,
        	total_attributes_size,
        	(const void*)(accumulated_size));
        glEnableVertexAttribArray(v_attribute.index);
		accumulated_size += size_from_type(v_attribute.element_type) * v_attribute.element_count;
	}
	ctx_end();
}

void vertex_array::ctx_begin(){
    previous_active = active_id;
    glBindVertexArray(gl_id);
	active_id = gl_id;
}

void vertex_array::ctx_end(){
    active_id = previous_active;
    glBindVertexArray(previous_active);
}

vertex_array::~vertex_array(){
    glDeleteVertexArrays(1, &gl_id);
}

std::shared_ptr<vertex_array> vertex_array::create(
	std::vector<vertex_attribute> attributes,
	const std::shared_ptr<program> prog,
	const std::shared_ptr<gl_buffer> vertex_buffer,
	const std::shared_ptr<gl_buffer> elem_buffer){
	return std::shared_ptr<vertex_array>(new vertex_array(attributes, prog, vertex_buffer, elem_buffer));
}
