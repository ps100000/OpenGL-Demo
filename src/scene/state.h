#ifndef _SCENE__STATE_H_
#define _SCENE__STATE_H_

#include "gui/overlay.h"
#include "common/singleton.h"
#include "node.h"

namespace scene{
	class state{
		private:
			state() = default;
		public:
			friend singleton<state>;

            graph::node root;
            gui::overlay main_gui;
            float delta_t;
    };
}

#endif
