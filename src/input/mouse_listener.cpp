#include "mouse_listener.h"
#include <GL/freeglut.h>

using namespace input::mouse;

void btn_listener::down(){}

void btn_listener::up(){}

btn_listener::~btn_listener(){
    std::vector<btn_listener*>& entries = listeners[btn];
    for(auto l = entries.begin(); l != entries.end(); l++){
        if((*l)->id == id){
            entries.erase(l);
            return;
        }
    }
}

btn_listener::btn_listener(int btn):
    btn(btn){
    listeners[btn].push_back(this);
}

void btn_listener::glut_btn_receiver(int button, int state, int, int){
    for(auto* l : listeners[button]){
        if(state == GLUT_DOWN){
            l->down();
        }else{
            l->up();
        }
    }
}

/**************************************************************/


void pull_btn_listener::down(){
    pressed = true;
}

void pull_btn_listener::up(){
    pressed = false;
}

pull_btn_listener::pull_btn_listener(int btn):
    btn_listener(btn){}

pull_btn_listener::operator bool(){
    return pressed;
}


/**************************************************************/


void cb_btn_listener::down(){
    callback(true);
}

void cb_btn_listener::up(){
    callback(false);
}

cb_btn_listener::cb_btn_listener(int btn, std::function<void(bool)> callback):
    btn_listener(btn), callback(callback){}


/**************************************************************/



move_listener::move_listener(std::function<void(int,int)> callback):
    callback(callback){
    listeners.push_back(this);
}

move_listener::~move_listener(){
    for(auto l = listeners.begin(); l != listeners.end(); l++){
        if((*l)->id == id){
            listeners.erase(l);
            return;
        }
    }
    
}

void move_listener::glut_move_receiver(int x, int y){
    for(auto* l : listeners){
        l->callback(x, y);
    }
}
