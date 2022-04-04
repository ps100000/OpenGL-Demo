#include "overlay.h"

#include <algorithm>

#include "glm/gtx/string_cast.hpp"

#include "conf/parser.h"
#include "gl/window_info.h"
#include "common/singleton.h"

using namespace gui;

namespace{
	inline glm::vec2 vec2(conf::elm& array){
		return glm::vec2{
			(float)array[0],
			(float)array[1]
		};
	}
}

void overlay::draw(){
    elm::set_window_size(
        singleton<window_info>::value.w,
        singleton<window_info>::value.h);
	for(elm& elem : elements){
        std::cout << elem.name << " z:" << (int)elem.z_index << std::endl;
        elem.draw();
    }
}

elm& overlay::operator[](std::string name){
	for(auto& element : elements){
		if(element.name == name){
			return element;
		}
	}
	throw std::out_of_range("name " + name + "doesn't exist!");
}

overlay overlay::load_from_config(std::string filename){
    overlay new_ol;
    
	conf::parser parser;
	parser.parse_file(filename);

	
    conf::elm& root = parser.get_root();
	for(std::string child_name : root.get_children()){
        conf::elm& conf_elm = root[child_name];
        elm elem((std::string)conf_elm["file"], child_name);
        if(conf_elm.has_key("z_index")){
            elem.z_index = (int)conf_elm["z_index"];
        }
        if(conf_elm.has_key("visible")){
            elem.visible = (bool)conf_elm["visible"];
        }
        if(conf_elm.has_key("rotation")){
            elem.rotation = (float)conf_elm["rotation"];
        }
        if(conf_elm.has_key("alignment")){
            elem.alignment = vec2(conf_elm["alignment"]);
        }
        if(conf_elm.has_key("center")){
            elem.center = vec2(conf_elm["center"]);
        }
        if(conf_elm.has_key("scale")){
            elem.scale = vec2(conf_elm["scale"]);
        }
        if(conf_elm.has_key("translation")){
            elem.translation = vec2(conf_elm["translation"]);
        }
        
        new_ol.elements.push_back(std::move(elem));
    }
    std::sort(new_ol.elements.begin(), new_ol.elements.end(),[](elm& a, elm& b){
		return b.z_index > a.z_index;
	});
    
    return new_ol;
}
