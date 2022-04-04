#include "capability.h"
#include <iostream>

void capability::ctx_begin(){
	previous_active = glIsEnabled(cap);
	if(active != previous_active){
		if(active){
			glEnable(cap);
		}else{
			glDisable(cap);
		}
	}
}

void capability::ctx_end(){
	if(active != previous_active){
		if(previous_active){
			glEnable(cap);
		}else{
			glDisable(cap);
		}
	}
}

capability::capability(GLenum cap, GLboolean active):
	cap(cap),
	active(active){}
