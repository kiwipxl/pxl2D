#include "GameLoop.h"
#include <iostream>
#include <Windows.h>
#include "managers/WindowManager.h"
#include "managers/Assets.h"
#include "Renderer.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static Assets* assets;
};

float t = 0;

void GameLoop::start() {
	set_fps(90);
	start_second_time = 0;
	frame_counter = 0;
	quit = false;

	initiate(universe->win_manager->screen_width, universe->win_manager->screen_height);

	SDL_JoystickEventState(SDL_ENABLE);

	while (!quit) {
		start_time = std::clock();

		while (SDL_PollEvent(&e) != 0) {
			int mouse_x; int mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);

			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}else if (e.type == SDL_WINDOWEVENT) {
				universe->win_manager->update_resize();
			}
		}

		//reset render call variables
		//render_calls = 0;
		//transform_render_calls = 0;
		//vertices_uploaded = 0;
		//total_render_calls = 0;

		glClear(GL_COLOR_BUFFER_BIT);

		std::clock_t start_render = std::clock();

		SDL_Rect rect;
		rect.x = 400;
		rect.y = 300;
		rect.w = 500 / 2;
		rect.h = 355 / 2;
		SDL_Point origin;
		origin.x = rect.w / 2;
		origin.y = rect.h / 2;
		t += .5f;
		render_transform(universe->assets->cat, NULL, &rect, t, &origin, SDL_RendererFlip::SDL_FLIP_NONE);

		//swaps back buffer to front buffer
		SDL_GL_SwapWindow(universe->win_manager->window);

		double ms = std::clock() - start_time;
		if (ms >= 0 && ms < ms_per_frame) { Sleep(floor(ms_per_frame - ms)); }

		++frame_counter;
		if (std::clock() - start_second_time >= 1000) {
			std::cout << "fps: " << frame_counter << "\n";
			frame_counter = 0;
			start_second_time = std::clock();
		}
	}
}