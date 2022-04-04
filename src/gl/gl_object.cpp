#include "gl_object.h"

gl_object::gl_object(): gl_id(0){}

gl_object::gl_object(GLuint gl_id): gl_id(gl_id){}

gl_object::operator GLuint(){
	return gl_id;
}
