#include "logic.h"

#include <iostream>
#include <chrono>
#include <optional>

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"

#include "common/singleton.h"
#include "scene/state.h"
#include "scene/sea.h"
#include "input/key_listener.h"
#include "input/mouse_listener.h"
#include "gl/light_manager.h"
#include "gl/window_info.h"


#include <GL/freeglut.h>

namespace{
    gl::state& gl_state = singleton<gl::state>::value;
    scene::state& state = singleton<scene::state>::value;
    gl::light_manager& light_mgr = singleton<gl::light_manager>::value;

    void update_boat_light(){
        static input::key::pull_listener left('4');
        static input::key::pull_listener right('6');
        static input::key::pull_listener up('8');
        static input::key::pull_listener down('5');
        static input::key::pull_listener on('7', false);

        static scene::graph::node& lamp = state.root["boat"]["body"]["lamp"];
        static scene::graph::node& lamp_head = lamp["lamp_head"];
        static gl::light_manager::light& light = light_mgr
            .lights[lamp["lamp_head"]["light"].light_index];
        if(left){
            lamp.model_ctx.pitch += glm::radians(25.0f) * state.delta_t;
        }
        if(right){
            lamp.model_ctx.pitch -= glm::radians(25.0f) * state.delta_t;
        }
        if(up && lamp_head.model_ctx.yaw > glm::radians(-90.0f)){
            lamp_head.model_ctx.yaw -= glm::radians(25.0f) * state.delta_t;
        }
        if(down && lamp_head.model_ctx.yaw < glm::radians(30.0f)){
            lamp_head.model_ctx.yaw += glm::radians(25.0f) * state.delta_t;
        }
        light.active = on;

        static gui::elm& lamp_beam = state.main_gui["lamp_beam"];
        lamp_beam.visible = on;
    }

    void update_boat(){
        static input::key::pull_listener left('a');
        static input::key::pull_listener right('d');
        static input::key::pull_listener accel('w');
        static input::key::pull_listener decel('s');
        static float speed = 0;
        static float steer = 0;

        static scene::graph::node& boat = state.root["boat"]; // l/r/pos
        static scene::graph::node& body = boat["body"]; // angle
        static scene::graph::node& r_motor = body["r_motor"];
        static scene::graph::node& l_motor = body["l_motor"];
        static scene::graph::node& r_prop = r_motor["propeller"];
        static scene::graph::node& l_prop = l_motor["propeller"];
        if(left && steer < 1){
            steer += 3 * state.delta_t;
        }
        if(right && steer > -1){
            steer -= 3 * state.delta_t;
        }
        if(!(left || right)){
            steer -= steer * 0.8 * state.delta_t;
        }
        if(accel){
            speed += 1 * state.delta_t;
        }
        if(decel){
            speed -= 0.5 * state.delta_t;
        }

        glm::vec3 boat_translation = (glm::vec3)boat.model_ctx.translation
            - glm::rotateY(glm::vec3(20.833f, 0.0f, 0.0f), (float)boat.model_ctx.pitch) * speed * state.delta_t;
        boat_translation.x = std::min(std::max(-240.f, boat_translation.x), 240.0f);
        boat_translation.z = std::min(std::max(-240.f, boat_translation.z), 240.0f);
        float dist_island = glm::length(glm::vec2{boat_translation.x, boat_translation.z});
        if(dist_island < 52.5f){
        boat_translation.x *= 52.5f / dist_island;
        boat_translation.z *= 52.5f / dist_island;
        }
        boat.model_ctx.translation = boat_translation;
        boat.model_ctx.pitch += steer * speed * glm::radians(27.5f) * state.delta_t;

        speed -= std::max(std::abs(speed), 0.2f) * speed * 1.25 * state.delta_t; // incorrect drag but feels right (max 0.2 <-> only abs)

        body.model_ctx.yaw = std::max(speed, 0.0f) * glm::radians(-5.0f);
        body.model_ctx.roll = std::max(speed, 0.0f) * steer * glm::radians(5.0f);

        float motor_angle = steer * glm::radians(-25.0f);
        r_motor.model_ctx.pitch = motor_angle;
        l_motor.model_ctx.pitch = motor_angle;

        float prop_speed = (accel - 0.5 * decel + 0.05 * speed) * glm::radians(1500.0f) * state.delta_t;
        r_prop.drawable->roll += prop_speed;
        l_prop.drawable->roll += prop_speed;

        static gui::elm& needle = state.main_gui["speed_needle"];
        needle.rotation = speed * glm::radians(-105.0f);
    }

    void update_cams(){
        static uint8_t active = 1;
        static input::key::cb_listener one('1', [](bool pressed){if(pressed) active = 0;});
        static input::key::cb_listener two('2', [](bool pressed){if(pressed) active = 1;});
        static input::key::cb_listener three('3', [](bool pressed){if(pressed) active = 2;});

        static input::key::cb_listener flying('0', [](bool pressed){if(pressed) active = 3;});
        static input::key::pull_listener flying_fw('i');
        static input::key::pull_listener flying_bw('k');
        static input::key::pull_listener flying_l('j');
        static input::key::pull_listener flying_r('l');
        

        constexpr uint8_t NUMBER_OF_CAMS = 4;
        static std::array<std::reference_wrapper<scene::graph::node>, NUMBER_OF_CAMS> cams = {
            state.root["boat"]["body"]["seat"]["first_person"],
            state.root["boat"]["body"]["follower"],
            state.root["spectator"],
            state.root["flying"]
        };
        for(uint8_t i = 0; i < NUMBER_OF_CAMS; i++){
            ((scene::graph::node&)cams[i]).cam_active = i == active;
        }

        static window_info& window = singleton<window_info>::value;
        static scene::graph::node& boat = state.root["boat"];
        static int x_pos = 0;
        static int y_pos = 0;
        static input::mouse::move_listener mouse{
            [](int x, int y){
                x_pos = x;
                y_pos = y;
            }};
        switch(active){
            case 0:
                cams[0].get().model_ctx.pitch = glm::radians(-240.0f) * (float)x_pos / window.w
                    + glm::radians(120.0f + 90.0f);
                cams[0].get().model_ctx.roll = glm::radians(-180.0f) * (float)y_pos / window.h
                    + glm::radians(90.0f);
                break;
            case 2:{
                static gl::rst_model_context& model_ctx = cams[2].get().model_ctx;
                glm::vec4 boat_pos = boat.model_ctx.last_inner * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
                model_ctx.pitch = atan2(-boat_pos.x, -boat_pos.z);
                model_ctx.roll = atan(
                    sqrt(boat_pos.x*boat_pos.x + boat_pos.z*boat_pos.z)
                    / abs(boat_pos.y - ((glm::vec3)model_ctx.translation).y))
                    - glm::radians(90.0f);
                break;
            }
            case 3:{
                static gl::rst_model_context& model_ctx = cams[3].get().model_ctx;
                float pitch = glm::radians(-360.0f) * (float)x_pos / window.w
                    + glm::radians(180.0f);
                float roll = glm::radians(-180.0f) * (float)y_pos / window.h
                    + glm::radians(90.0f);
                model_ctx.pitch = pitch;
                model_ctx.roll = roll;

                glm::mat4 r{1.0f};
                r = glm::rotate(r, -(float)roll, {1.0f, 0, 0});
                r = glm::rotate(r, -(float)pitch, {0, 1.0f, 0});
                glm::vec4 movement{
                    flying_r - flying_l, 
                    0.0f,
                    flying_bw - flying_fw,
                    1.0f
                };
                movement = movement * r;
                movement *= 10.0f * state.delta_t;
                model_ctx.translation += glm::vec3{movement};
                break;}
            default:
                break;
        }
    }

    struct flare_t{
        
        scene::graph::node& node;
        gl::light_manager::light& light;
        uint8_t steps;
        float scale_step;
        float scale;
        float speed;
        glm::vec3 up;

        flare_t() = default;

        flare_t(scene::graph::node& node):
            node(node),
            light(light_mgr.lights[node["light"].light_index]),
            steps(0),
            scale_step(0),
            scale(1),
            speed(1){}
    };

    void update_flare(){
        static uint8_t fired = 0;
        constexpr uint8_t NUMBER_OF_FLARES = 4;
        static std::optional<flare_t> flares[NUMBER_OF_FLARES];

        static input::key::pull_listener fire('9');
        static float last_fire = 1;
        last_fire += state.delta_t;

        if(fire && last_fire > 1 && fired < NUMBER_OF_FLARES){
            flares[fired].emplace(state.root["flares"]["flare" + std::to_string(fired)]);
            scene::graph::node& structure = state.root["boat"]["body"]["structure"];
            glm::mat4 rst = structure.model_ctx.last_inner;
            glm::vec3 p1 = rst * glm::vec4{0.0f, 1.8f, -0.4f, 1.0f};
            glm::vec3 p2 = rst * glm::vec4{0.0f, 2.8f, -0.4f, 1.0f};
            glm::vec3 up = p2 - p1;
            up *= 50.0f / glm::length(up);
            flares[fired].value().node.model_ctx.translation = glm::vec3{p1};
            flares[fired].value().up = up;
            flares[fired].value().light.active = true;
            last_fire = 0;

            state.main_gui["flare_cartridge" + std::to_string(fired)].visible = false;

            fired++;
        }

        
        for(uint8_t i = 0; i < NUMBER_OF_FLARES; i++){
            if(!flares[i]){
                continue;
            }
            flare_t& flare = flares[i].value();

            if(flare.steps){
                flare.steps--;
                flare.scale += flare.scale_step;
                flare.node.model_ctx.scale = glm::vec3{flare.scale, flare.scale, flare.scale};

                flare.light.diffuse = glm::vec4{1.0, 0.1, 0.1, 1.0} * (flare.scale * 1.5f + 0.25f);
                flare.light.specular = glm::vec4{0.8, 0.4, 0.4, 1.0} * (flare.scale * 1.5f + 0.25f);
                
                flare.node.model_ctx.translation += flare.up * flare.speed * state.delta_t;
                flare.speed -= 0.1 * state.delta_t;
                flare.speed -= flare.speed * std::abs(flare.speed) * 1.25 * state.delta_t;
            }else{
                flare.steps = (rand() % 10); 
                float target = ((rand() % 20) / 80.0f + 0.25);
                flare.scale_step = (target - flare.scale) / flare.steps;
            }

            if(((glm::vec3)flare.node.model_ctx.translation).y < 0.0f){
                flares[i].reset();
                flare.node.model_ctx.scale = glm::vec3{0.0f, 0.0f, 0.0f};
                flare.light.active = false;
            }
        }
    }

    void update_sea(){
        scene::sea* sea = (scene::sea*)state.root["sea"].drawable.get();
        static input::key::pull_listener waves('q', false);

        sea->waves = waves;
        sea->progress += 0.1f * state.delta_t;

        if(sea->progress >= 0.925f){
            sea->progress = 0.075f;
        }
    }

    void update_help(){
        static input::key::pull_listener help_kb('h', false);
        static gui::elm& help = state.main_gui["help"];
        help.visible = help_kb;
    }
}

void tick(){
	static auto start = std::chrono::steady_clock::now();
    static auto end = std::chrono::steady_clock::now();
	end = std::chrono::steady_clock::now();
    state.delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    start = std::chrono::steady_clock::now();

    static input::key::cb_listener quit('\x1b', [](bool){glutLeaveMainLoop();});

    update_boat_light();
    update_boat();
    update_cams();
    update_flare();
    update_sea();
    update_help();

	state.root.draw();
    state.main_gui.draw();
	scene::body::bind_lights();
	scene::sea::bind_lights();
}