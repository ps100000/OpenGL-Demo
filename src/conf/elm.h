#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

#include "special.h"

namespace conf{
	struct wrong_type_exception : public std::exception{
		const char* what () const throw (){
			return "wrong type!";
		}
	};

	struct no_value_exception : public std::exception{
		const char* what () const throw (){
			return "no value set!";
		}
	};

	enum class elm_types{
		NONE,
		INT,
		FLOAT,
		BOOL,
		STRING,

		ARRAY,
		OBJ,
		SPECIAL
	};

	class elm{
		private:
			std::string key = "";
			elm_types type = elm_types::NONE;

			uint32_t val_int;
			float val_float;
			elm_special special = elm_special::NONE;
			special_base* spec_val = NULL;
			
			std::string val_string = "";
			std::vector<elm*> children;

			void type_check(const elm_types checked_type) const;
			void type_check_set(const elm_types checked_type);
		public:
			elm(elm&) = delete;
			elm(elm&&) = delete;
			elm& operator=(elm&) = delete;
			elm& operator=(elm&&) = delete;
			elm() = default;
			~elm();

			void set_type(elm_types type);
			elm_types get_type();
			elm_special get_special_type();

			template<typename T>
			explicit operator T() const;
			explicit operator int() const;
			explicit operator float() const;
			explicit operator bool() const;
			explicit operator std::string() const;

			template<typename T>
			elm& operator=(const T val);
			elm& operator=(int val);
			elm& operator=(float val);
			elm& operator=(bool val);
			elm& operator=(std::string val);

			template<typename T>
			void set_special(const T val);

			template<typename T>
			T get_special() const;

			uint16_t size();
			elm* add_array_entry();
			elm& operator[](uint16_t index) const;

			std::vector<std::string> get_keys();
			std::vector<std::string> get_children();
			bool has_key(std::string key);
			elm* add_child(std::string key);
			elm& operator[](std::string key) const;
	};
}
