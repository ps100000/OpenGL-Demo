#include "key_listener.h"
#include <GL/freeglut.h>

using namespace input::key;

void listener::down(){}

void listener::up(){}

listener::~listener(){
    std::vector<listener*>& entries = listeners[key | (modifier_keys << 8)];
    for(auto l = entries.begin(); l != entries.end(); l++){
        if((*l)->id == id){
            entries.erase(l);
            return;
        }
    }
}

listener::listener(unsigned char key, int modifier_keys):
    key(key), modifier_keys(modifier_keys){
    listeners[key | (modifier_keys << 8)].push_back(this);
}

void listener::glut_down_receiver(unsigned char key, int, int){
    glutGetModifiers();
    for(auto* l : listeners[key | (glutGetModifiers() << 8)]){
        l->down();
    }
}

void listener::glut_up_receiver(unsigned char key, int, int){
    for(auto* l : listeners[key | (glutGetModifiers() << 8)]){
        l->up();
    }
}


/**************************************************************/


void pull_listener::down(){
    if(toggle){
        pressed = !pressed;
    }else{
        pressed = true;
    }
}

void pull_listener::up(){
    if(!toggle){
        pressed = false;
    }
}

pull_listener::pull_listener(unsigned char key, int modifier_keys):
    listener(key, modifier_keys){}

pull_listener::pull_listener(unsigned char key, bool start, int modifier_keys):
    listener(key, modifier_keys),
    pressed(start),
    toggle(true){}

pull_listener::operator bool(){
    return pressed;
}


/**************************************************************/


void cb_listener::down(){
    callback(true);
}

void cb_listener::up(){
    callback(false);
}

cb_listener::cb_listener(unsigned char key, std::function<void(bool)> callback, int modifier_keys):
    listener(key, modifier_keys), callback(callback){}


