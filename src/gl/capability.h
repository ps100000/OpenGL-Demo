#ifndef _CAPABILITY_H_
#define _CAPABILITY_H_

#include <GL/glew.h>

#include "common/context.h"

class capability: public context{
	private:
		GLboolean previous_active;
		void ctx_begin() override;
		void ctx_end() override;
	public:
		const GLenum cap;
		const GLboolean active;

		capability(GLenum cap, GLboolean active);
};

#endif
