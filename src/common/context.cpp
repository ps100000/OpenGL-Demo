#include "context.h"
#include <iostream>  
inline void context::ctx_begin(){}

inline void context::ctx_end(){}

void context::use(std::function<void()> lambda){
	ctx_begin();
	lambda();
	ctx_end();
}

void context::use(std::vector<std::reference_wrapper<context>> ctxs, std::function<void()> lambda){
	for (auto ctx = ctxs.begin(); ctx != ctxs.end(); ctx++){
		(*ctx).get().ctx_begin();
	}
	lambda();
	for (auto ctx = ctxs.rbegin(); ctx != ctxs.rend(); ctx++){
		(*ctx).get().ctx_end();
	}
}

/**************************************************************/

generic_context::generic_context(
	std::function<void()> begin,
	std::function<void()> end): begin_func(begin), end_func(end){}

inline void generic_context::ctx_begin(){
	begin_func();
}

inline void generic_context::ctx_end(){
	end_func();
}


void generic_context::set_func(
		std::function<void()> begin,
		std::function<void()> end){
	begin_func = begin;
	end_func = end;
}