#pragma once

#include "elm.h"

namespace conf{
	template<typename T>
	elm::operator T() const{
		static_assert(std::is_convertible<T, special_base>::value, "special types have to have base class special_base!");
		type_check(elm_types::SPECIAL);
		if (spec_val == NULL){
			throw no_value_exception;
		}
		if (special == T::special_type && special != elm_special::NONE){
			return *(T*)spec_val;
		}
		throw wrong_type_exception;
	};

	template<typename T>
	elm& elm::operator=(const T val){
		static_assert(std::is_convertible<T, special_base>::value, "special types have to have base class special_base!");
		type_check_set(elm_types::SPECIAL);
		if(spec_val != NULL){
			delete spec_val;
		}
		special = val.special_type;
		spec_val = new T(val);
		return *this;
	};
}