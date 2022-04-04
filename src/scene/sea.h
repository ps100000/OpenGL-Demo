#ifndef _SCENE__SEA_H_
#define _SCENE__SEA_H_

#include "body.h"
#include "gl/texture.h"

namespace scene{
    class sea: public body{
        private:
            static std::shared_ptr<program> get_prog();
		    static inline std::shared_ptr<gl::texture> heightmap = nullptr;
        public:
            sea();
            float progress = 0.0f;
            bool waves = false;

            void draw() override;

			static void bind_lights();
    };
}

#endif
