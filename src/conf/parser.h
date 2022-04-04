#pragma once

#include <string>
#include <map>
#include <stack>
#include <functional>
#include <stdexcept>

#include "elm.h"

namespace conf{
	class parser{
		private:
			bool is_header(const std::string& line, const size_t indentation);
			bool parse_header(const std::string& line, const size_t indentation, std::stack<elm*>& current_nesting);
			bool parse_key_value(const std::string& line, const size_t indentation, std::stack<elm*>& current_nesting);
			int16_t parse_value(const std::string& value, elm* elm);
			int16_t parse_string(const std::string& value, elm* elm);
			int16_t parse_bool(const std::string& value, elm* elm);
			int16_t parse_array(const std::string& value, elm* elm);
			int16_t parse_special(const std::string& value, elm* elm);
			int16_t parse_number(const std::string& value, elm* elm);
			std::pair<size_t, size_t> find_separator(const std::string& str, char separator, size_t start = 1);
			bool unescape_string(std::string& escaped);

			elm root;
		public:
			std::map<std::string, std::function<int16_t (const std::string& value, elm* elm)>> special_subparsers;
			bool parse_file(std::string file_path);
			elm& get_root();
	};
}
