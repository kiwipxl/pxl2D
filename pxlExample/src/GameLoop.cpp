#include "GameLoop.h"
#include <iostream>
#include <Windows.h>
#include "managers/WindowManager.h"
#include "managers/Assets.h"
#include <PXL.h>

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

	PXL_init(universe->win_manager->screen_width, universe->win_manager->screen_height);

	SDL_JoystickEventState(SDL_ENABLE);

	PXL_Batch batch = PXL_Batch(PXL_LARGE_BATCH);

	int amount = 25;
	int* pos = new int[amount * 2];
	for (int n = 0; n < amount * 2; n += 2) {
		pos[n] = int((rand() / float(RAND_MAX)) * 800);
		pos[n + 1] = int((rand() / float(RAND_MAX)) * 550);
	}

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

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 1, 1, 1);

		std::clock_t start_render = std::clock();

		//silly test code stuff
		PXL_use_default_shader(&batch);

		PXL_Rect rect;
		PXL_Vec2 origin;
		rect.x = 400;
		rect.y = 300;
		rect.w = 512 / 2;
		rect.h = 320 / 2;
		origin.x = rect.w / 2;
		origin.y = rect.h / 2;
		t += .5f;

		rect.w = (cos(t / 20.0f) + 1) * 512;
		rect.h = (cos(t / 20.0f) + 1) * 320;
		rect.x = 0;
		rect.y = 0;
		origin.x = rect.w / 2;
		origin.y = rect.h / 2;
		batch.add(universe->assets->cat_2, &rect, NULL, 0, 255, 255, 120, 0, &origin, PXL_FLIP_NONE);

		/**
		for (int n = 0; n < amount * 2; n += 2) {
			rect.x = pos[n];
			rect.y = pos[n + 1];
			if (n >= amount - 1 && n <= amount + 1) {
				batch.render_all();
				PXL_use_bloom_shader(&batch);
			}
			if (pos[n] >= 512) {
				batch.add(universe->assets->cute_cat, &rect, NULL, 0, 120, 200, 120, t, &origin, PXL_FLIP_NONE);
			}else {
				batch.add(universe->assets->cat_2, &rect, NULL, 120, 255, 0, 200, t, &origin, PXL_FLIP_NONE);
			}
		}
		**/

		batch.render_all();

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