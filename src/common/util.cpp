#include "util.h"

#include <iostream>
#include <fstream>

#include<string>
#include<map>

#ifdef __EMSCRIPTEN__
extern const std::map<std::string, std::string> resource2c;
std::optional<std::string> file_read(std::string filename){
	return resource2c.at(filename);
}

#else
std::optional<std::string> file_read(std::string filename){
    if(std::ifstream is{filename, std::ios::binary | std::ios::ate}){
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        if(is.read(&str[0], size)){
            return str;
        }
    }
    return {};
}
#endif
