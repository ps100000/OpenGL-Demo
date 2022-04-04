#ifndef _GL_OBJECT_H_
#define _GL_OBJECT_H_
#include <GL/glew.h>

class gl_object{
	protected:
		GLuint gl_id;
	public:
		gl_object();
		gl_object(GLuint gl_id);

		operator GLuint();
};
#endif
