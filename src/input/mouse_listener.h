#ifndef _INPUT__MOUSE_LISTENER_H_
#define _INPUT__MOUSE_LISTENER_H_

#include <map>
#include <vector>
#include <functional>
#include <cstdint>

namespace input::mouse{
    class btn_listener{
        private:
            static inline std::map<int, std::vector<btn_listener*>> listeners = {};

            static inline uint64_t id_counter = 0;
            const uint64_t id = id_counter++;
        protected:
            btn_listener(int btn);

            virtual void down();
            virtual void up();
        public:
            virtual ~btn_listener();

            const int btn;
            
            static void glut_btn_receiver(int button, int state, int x, int y);

    };

    class pull_btn_listener: public btn_listener{
        private:
            bool pressed = false;
        protected:
            void down() override;
            void up() override;
        public:
            pull_btn_listener(int btn);
            operator bool();


    };

    class cb_btn_listener: public btn_listener{
        protected:
            void down() override;
            void up() override;
        public:
            cb_btn_listener(int btn, std::function<void(bool)> callback = [](bool){});
            std::function<void(bool)> callback;
    };


    class move_listener{
        private:
            static inline std::vector<move_listener*> listeners = {};

            static inline uint64_t id_counter = 0;
            const uint64_t id = id_counter++;
        public:
            move_listener(std::function<void(int,int)> callback = [](int,int){});
            virtual ~move_listener();

            std::function<void(int,int)> callback;
        
            static void glut_move_receiver(int x, int y);
    };
}
#endif
