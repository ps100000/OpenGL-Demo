#include "elm.h"

using namespace conf;

elm::~elm(){
	if(type == elm_types::SPECIAL){
		delete (special_base*)spec_val;
	}
	for(elm* child : children){
		delete child;
	}
}

void elm::type_check(const elm_types checked_type) const{
	if(type != checked_type){
		throw wrong_type_exception();
	}
}

void elm::type_check_set(const elm_types checked_type){
	if((type != elm_types::NONE) && (type != checked_type)){
		switch(type){
			case elm_types::OBJ:
			case elm_types::ARRAY:
				for(elm* child : children){
					delete child;
				}
				break;
			case elm_types::SPECIAL:
				delete spec_val;
				spec_val = NULL;
				break;
			default:
				break;
		}
	}
	type = checked_type;
}

void elm::set_type(elm_types type){
	type_check_set(type);
}

elm_types elm::get_type(){
	return type;
}

elm_special elm::get_special_type(){
	return special;
}

elm::operator int() const{
	type_check(elm_types::INT);
	return val_int;
}

elm::operator float() const{
	type_check(elm_types::FLOAT);
	return val_float;
};

elm::operator bool() const{
	type_check(elm_types::BOOL);
	return val_int != 0;
};

elm::operator std::string() const{
	type_check(elm_types::STRING);
	return val_string;
};

elm& elm::operator=(int val){
	type_check_set(elm_types::INT);
	val_int = val;
	return *this;
}

elm& elm::operator=(float val){
	type_check_set(elm_types::FLOAT);
	val_float = val;
	return *this;
}

elm& elm::operator=(bool val){
	type_check_set(elm_types::BOOL);
	val_int = val;
	return *this;
}

elm& elm::operator=(std::string val){
	type_check_set(elm_types::STRING);
	val_string = val;
	return *this;
}

uint16_t elm::size(){
	return children.size();
}

elm* elm::add_array_entry(){
	type_check_set(elm_types::ARRAY);
	elm* new_entry = new elm;
	children.push_back(new_entry);
	return new_entry;
}


elm& elm::operator[](uint16_t index) const{
	type_check(elm_types::ARRAY);
	if (index >= children.size()){
		throw std::out_of_range("tried to access index "
									+ std::to_string(index)
									+ ". elm has "
									+ std::to_string(children.size())
									+ " children.");
	}
	return *children[index];
}

std::vector<std::string> elm::get_keys(){
	type_check(elm_types::OBJ);
	std::vector<std::string> keys;
	for(elm* child : children){
		keys.push_back(child->key);
	}
	return keys;
}

std::vector<std::string> elm::get_children(){
	type_check(elm_types::OBJ);
	std::vector<std::string> keys;
	for(elm* child : children){
		if(child->type == elm_types::OBJ){
			keys.push_back(child->key);
		}
	}
	return keys;
}

bool elm::has_key(std::string key){
	for(elm* child : children){
		if(child->key == key){
			return true;
		}
	}
	return false;
}

elm* elm::add_child(std::string key){
	type_check_set(elm_types::OBJ);
	if(has_key(key)){
		return NULL;
	}	
	elm* new_entry = new elm;
	new_entry->key = key;
	children.push_back(new_entry);
	return new_entry;
}

elm& elm::operator[](std::string key) const{
	type_check(elm_types::OBJ);
	for(elm* child : children){
		if(child->key == key){
			return *child;
		}
	}
	throw std::out_of_range("key " + key + "doesn't exist!");
}
