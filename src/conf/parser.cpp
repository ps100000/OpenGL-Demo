#include <fstream>
#include <iostream>

#include "parser.h"


using namespace conf;

inline bool parser::is_header(const std::string& line, const size_t indentation){
	return line[indentation] == '[';
}

bool parser::parse_header(const std::string& line, const size_t indentation, std::stack<elm*>& current_nesting){
	if(line[indentation] != '[' || line.find_first_of(']') != line.size() - 1){
		std::cerr << "malformed new section start!" << std::endl;
		return false;
	}
	std::string header = line.substr(indentation + 1, line.size() - indentation - 2);
	elm* new_child = current_nesting.top()->add_child(header);
	if(!new_child){
		std::cerr << "value or header with that name already exists!" << std::endl;
		return false;
	}
	current_nesting.push(new_child);
	return true;
}

bool parser::parse_key_value(const std::string& line, const size_t indentation, std::stack<elm*>& current_nesting){
	std::pair<size_t, size_t> key_value_delimit_pos = find_separator(line, '=');
	if(key_value_delimit_pos.first == std::string::npos){
		std::cerr << "couldn't find delimiter(=) between key and value!" << std::endl;
		return false;
	}

	std::string key = line.substr(indentation, key_value_delimit_pos.first - indentation);
	std::string value = line.substr(key_value_delimit_pos.second + 1, line.size() - key_value_delimit_pos.second - 1);
	if(!key.size()){
		std::cerr << "key has 0 length!" << std::endl;
		return false;
	}

	elm* new_child = current_nesting.top()->add_child(key);
	if(!new_child){
		std::cerr << "value or header with that name already exists!" << std::endl;
		return false;
	}

	if((size_t)parse_value(value, new_child) != value.size()){
		std::cerr << "value ends before end of line!" << std::endl;
		return false;
	}

	return true;
}

int16_t parser::parse_value(const std::string& value, elm* elm){
	if(!value.size()){
		std::cerr << "value has 0 length!" << std::endl;
		return -1;
	}
	switch(value[0]){
		case '"': // string
			return parse_string(value, elm);
		case 't':
		case 'f': // bool
			return parse_bool(value, elm);
		case '{': // array
			return parse_array(value, elm);
		case '\'': // special
			return parse_special(value, elm);
		default:  // int, float, or invalid
			return parse_number(value, elm);
	}
}

int16_t parser::parse_string(const std::string& value, elm* elm){
	while(1){
		size_t string_end = value.find_first_of('"', 1);
		if(string_end == std::string::npos){
			std::cerr << "no terminating \" for string!" << std::endl;
			return -1;
		}
		bool is_escaped = false;
		for(uint16_t i = string_end - 1; i >= 1; i--){
			if(value[i] == '\\'){
				is_escaped = !is_escaped;
			}else{
				break;
			}
		}
		if(!is_escaped){
			std::string data = value.substr(1, string_end - 1);
			if(!unescape_string(data)){
				return -1;
			}
			*elm = data;

			return string_end + 1;
		}
		string_end = value.find_first_of('"', string_end + 1);
	}

}

int16_t parser::parse_bool(const std::string& value, elm* elm){
	if(!value.compare(0, 4, "true")){
		*elm = true;
		return 4;
	}else if(!value.compare(0, 5, "false")){
		*elm = false;
		return 5;
	}else{
		std::cerr << "boolean value can only be 'true' or 'false'!" << std::endl;
		return -1;
	}

}

int16_t parser::parse_array(const std::string& value, elm* elm){
	size_t next_begin = 1;
	size_t value_length = value.size();
	elm->set_type(elm_types::ARRAY);
	if(value == "{}"){
		return 2;
	}else if(value == "{ }"){
		return 3;
	}
	while(1){
		int16_t used = parse_value(value.substr(next_begin), elm->add_array_entry());
		if(used == -1){
			std::cerr << "failed to parse value in array!" << std::endl;
			return -1;
		}
		next_begin += used;
		if(next_begin == value_length){
			break;
		}
		if(value[next_begin] == ' '){
			next_begin++;
			if(next_begin == value_length){
				break;
			}
		}
		if(value[next_begin] == ','){
			next_begin++;
			if(next_begin == value_length){
				break;
			}
			if(value[next_begin] == ' '){
				next_begin++;
			}
		}else if(value[next_begin] == '}'){
			return next_begin + 1;
		}else{
			std::cerr << "missing separator (,) or end (}) after array entry!" << std::endl;
			return -1;
		}
	}
	std::cerr << "missing } on end of array!" << std::endl;
	return -1;

}

int16_t parser::parse_special(const std::string& value, elm* elm){
	size_t special_end = value.find_first_of('\'', 1);
	if(special_end == std::string::npos){
		std::cerr << "no terminating ' for special type!" << std::endl;
		return -1;
	}

	std::string special = value.substr(1, special_end - 1);

	return special_end + 1 + special_subparsers.at(special)(value.substr(special_end + 1), elm);
}

int16_t parser::parse_number(const std::string& value, elm* elm){
	bool is_float = false;
	size_t num_end = 0;
	for(uint16_t i = value[0] == '-'; i < value.size(); i++){
		if(value[i] == '.'){
			if(!is_float){
				is_float = true;
			}else{
				num_end = i;
				break;
			}
			
		}else if(!(('0' <= value[i]) && (value[i] <= '9'))){
			num_end = i;
			break;
		}
	}
	if(num_end == 0){
		num_end = value.size();
	}

	if(is_float){
		*elm = std::stof(value);
	}else{
		*elm = (int)std::stol(value);
	}
	return num_end;
}


bool parser::unescape_string(std::string& escaped){
	size_t pos = escaped.find_first_of('\\');
	while(pos < escaped.size() - 1){

		switch(escaped[pos + 1]){
			case '"':
				escaped.replace(pos, 2, "\"");
				break;
			case '\\':
				escaped.replace(pos, 2, "\\");
				break;
			case 'n':
				escaped.replace(pos, 2, "\n");
				break;
			default:
				std::cerr << "invalid escaped char in string!" << std::endl;
				return false;
		}
		pos = escaped.find_first_of('\\', pos + 1);

	}
	return true;
}

std::pair<size_t, size_t> parser::find_separator(const std::string& str, char separator, size_t start){
	size_t token_begin = str.find_first_of(separator, start);
	size_t token_end = token_begin;
	if(token_begin == std::string::npos){
		return std::pair<size_t, size_t>(std::string::npos, std::string::npos);
	}

	if(token_begin > 0 && str[token_begin - 1] == ' '){
		token_begin--;
		if(token_begin == 0){
			return std::pair<size_t, size_t>(std::string::npos, std::string::npos);
		}
	}

	if(token_end < str.size() - 2 && str[token_end + 1] == ' '){
		token_end++;
		if(token_end == str.size() - 1){
			return std::pair<size_t, size_t>(std::string::npos, std::string::npos);
		}
	}

	return std::pair<size_t, size_t>(token_begin, token_end);
}


bool parser::parse_file(std::string file_path){
	std::ifstream input(file_path);
	size_t last_indent = 1;
	std::string line;
	bool last_was_header = false;
	std::stack<elm*> current_nesting;
	current_nesting.push(&root);
	current_nesting.push(NULL); // is poped instantly for the fake indent before the start of the file
	while(getline( input, line )){
		std::cout << line << std::endl;
		if(!line.size() || line[0] == '#'){
			continue;
		}
		size_t indentation = line.find_first_not_of('\t');

		if(line.size() < indentation + 3){
			std::cerr << "line after indentation to short!" << std::endl;
			return false;
		}

		if(indentation > last_indent + 1){
			std::cerr << "too far indented!" << std::endl;
			return false;
		}

		if(last_was_header && indentation <= last_indent){
			std::cerr << "indentation didn't increase after header!" << std::endl;
			return false;
		}

		if(indentation > last_indent && !last_was_header){
			std::cerr << "indentation increased without new header!" << std::endl;
			return false;
		}

		if(indentation < last_indent){
			/*if(!is_header(line, indentation)){
				std::cerr << "expected header after indentation decreased!" << std::endl;
				return false;
			}*/
			for(uint8_t i = 0; i < last_indent - indentation; i++){
				current_nesting.pop();
			}
		}

		if(is_header(line, indentation)){
			last_was_header = true;
			if(!parse_header(line, indentation, current_nesting)){
				std::cerr << "failed to parse header!" << std::endl;
				return false;
			}
		}else{
			last_was_header = false;
			if(!parse_key_value(line, indentation, current_nesting)){
				std::cerr << "failed to parse key-value pair!" << std::endl;
				return false;
			}
		}
		last_indent = indentation;
	}
	return true;
}

elm& parser::get_root(){
	return root;
}
