#ifndef _INPUT__KEY_LISTENER_H_
#define _INPUT__KEY_LISTENER_H_

#include <map>
#include <vector>
#include <functional>
#include <cstdint>

namespace input::key{
    class listener{
        private:
            static inline std::map<uint64_t, std::vector<listener*>> listeners = {};

            static inline uint64_t id_counter = 0;
            uint64_t id = id_counter++;
        protected:
            listener(unsigned char key, int modifier_keys = 0);

            virtual void down();
            virtual void up();
        public:
            virtual ~listener();

            const unsigned char key;
            const int modifier_keys;
            
            static void glut_down_receiver(unsigned char key, int x, int y);
            static void glut_up_receiver(unsigned char key, int x, int y);

    };

    class pull_listener: public listener{
        private:
            bool pressed = false;
            bool toggle;
        protected:
            void down() override;
            void up() override;
        public:
            pull_listener(unsigned char key, int modifier_keys = 0);
            pull_listener(unsigned char key, bool start, int modifier_keys = 0);
            operator bool();


    };

    class cb_listener: public listener{
        protected:
            void down() override;
            void up() override;
        public:
            cb_listener(unsigned char key, std::function<void(bool)> callback = [](bool){}, int modifier_keys = 0);
            std::function<void(bool)> callback;
    };
}
#endif
