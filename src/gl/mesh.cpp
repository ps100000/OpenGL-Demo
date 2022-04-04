#include "mesh.h"

using namespace gl;

void mesh::ctx_begin(){
	((context&)*v_array).ctx_begin();
	((context&)*prog).ctx_begin();
	begin_func(prog);
}

void mesh::ctx_end(){
	end_func(prog);
	((context&)*prog).ctx_end();
	((context&)*v_array).ctx_end();
}

mesh::mesh(
	std::shared_ptr<program> prog,
	std::shared_ptr<vertex_array> v_array,
	GLenum drawing_mode,
	std::function<void(std::shared_ptr<program> prog)> begin_func,
	std::function<void(std::shared_ptr<program> prog)> end_func):
		prog(prog),
		v_array(v_array),
		drawing_mode(drawing_mode),
		begin_func(begin_func),
		end_func(end_func){
}

void mesh::draw(){
	ctx_begin();
	if(v_array->elem_buffer){
    	glDrawElements(drawing_mode, v_array->elem_buffer->size, GL_UNSIGNED_INT, nullptr);
	}else{
		glDrawArrays(drawing_mode, 0, v_array->vertex_buffer->size);
	}
	ctx_end();
}

void mesh::draw(size_t offset, size_t count){
	ctx_begin();
	if(v_array->elem_buffer){
    	glDrawElements(
    		drawing_mode,
    		count,
    		GL_UNSIGNED_INT,
    		(void*)(offset * v_array->vertex_buffer->get_elm_size()));
	}else{
		glDrawArrays(drawing_mode, offset, count);
	}
	ctx_end();
}
