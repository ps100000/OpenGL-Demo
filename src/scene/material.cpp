#include "material.h"

#include <fstream>
#include <iostream>

using namespace scene;

material::material(std::string filename){
	std::ifstream obj = std::ifstream(filename);
    std::string line;
    float alpha = 1.0f;
    while (std::getline(obj, line)) {
		if(line.size() < 3){
			continue;
		}
		switch(line[0]){
			case 'K':
				switch(line[1]){
					case 'a':
						glm::vec3 a;
						sscanf(line.c_str(), "Ka %f %f %f", &a.r, &a.g, &a.b);
						ambient = glm::vec4(a, 1.0f);
						break;
					case 'd':
						glm::vec3 d;
						sscanf(line.c_str(), "Kd %f %f %f", &d.r, &d.g, &d.b);
						diffuse = glm::vec4(d, 1.0f);
						break;
					case 's':
						glm::vec3 s;
						sscanf(line.c_str(), "Ks %f %f %f", &s.r, &s.g, &s.b);
						specular = glm::vec4(s, 1.0f);
						break;
					default:
						break;
				}
				break;
			case 'N':
				switch(line[1]){
					case 's':
						float s;
						sscanf(line.c_str(), "Ns %f", &s);
						shininess = s;
						break;
					default:
						break;
				}
				break;
			case 'd':
				sscanf(line.c_str(), "d %f", &alpha);
				break;
			default:
				std::cout << line << std::endl;
				break;
		}
	}
	ambient.a = alpha;
	diffuse.a = alpha;
	specular.a = alpha;
}
