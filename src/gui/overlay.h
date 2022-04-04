#ifndef _GUI__OVERLAY_H_
#define _GUI__OVERLAY_H_

#include "elm.h"

namespace gui{
    class overlay{
        public:
            std::vector<elm> elements;

            void draw();
            elm& operator[](std::string name);

            static overlay load_from_config(std::string filename);
    };
}

#endif
