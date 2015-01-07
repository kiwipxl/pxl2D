#ifndef DEBUG_UI_H
#define DEBUG_UI_H

#include "components/Text.h"

class Universe;

class DebugUI {

	public:
		void create();
		void update();
		void remove();

		bool debugging;

		void update_fps_text(int fps);
		void update_render_info();

	private:
		Universe* universe;

		Text* fps_text;
		Text* render_text;
		Text* transform_render_text;
		Text* vertices_uploaded_text;
		Text* total_renders_text;
		Text* particles_drawn_text;
		Text* particles_uploaded_text;
		Text* particle_chunks_text;
};

#endif