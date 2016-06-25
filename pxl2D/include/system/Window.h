#ifndef _WINDOW_H
#define _WINDOW_H

#include <string>
#include "system/Event.h"
#include "system/WindowBase.h"
#include "API.h"

namespace pxl { namespace sys {

    class Window {

	    public:
            Window(int window_width, int window_height, std::string title);
            ~Window();

            bool poll_event(Event& e) { return base->poll_event(e); }
            void display() { base->display(); }
            void free() { base->free(); }

            int get_width() { return base->get_width(); }
            int get_height() { return base->get_height(); }
            std::string get_title() { return base->get_title(); }

        private:
            WindowBase* base;
    };
}};

#endif