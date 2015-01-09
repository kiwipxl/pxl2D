#include "GameLoop.h"
#include <iostream>
#include <Windows.h>
#include "managers/WindowManager.h"
#include "managers/Assets.h"
#include "input/MouseManager.h"
#include "ui/DebugUI.h"
#include "input/InputManager.h"
#include "tools/timer/TimerCallback.h"
#include "renderer/Renderer.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static Assets* assets;
		static MouseManager* mouse;
		static DebugUI* debug_ui;
		static InputManager* input;
		static TimerCallback* timer;
		static Renderer* renderer;
};

float t = 0;

void GameLoop::start() {
	set_fps(90);
	start_second_time = 0;
	frame_counter = 0;
	quit = false;

	SDL_JoystickEventState(SDL_ENABLE);

	while (!quit) {
		start_time = std::clock();

		universe->mouse->mouse_released = false;
		while (SDL_PollEvent(&e) != 0) {
			SDL_GetMouseState(&universe->mouse->mouse_pos.x, &universe->mouse->mouse_pos.y);

			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}else if (e.type == SDL_WINDOWEVENT) {
				universe->win_manager->update_resize();
			}
			universe->mouse->event_update(e);
			universe->input->event_update(e);
		}

		//reset render call variables
		universe->renderer->render_calls = 0;
		universe->renderer->transform_render_calls = 0;
		universe->renderer->vertices_uploaded = 0;
		universe->renderer->total_render_calls = 0;

		glClear(GL_COLOR_BUFFER_BIT);

		std::clock_t start_render = std::clock();
		universe->timer->update();

		//update render info
		universe->debug_ui->update_render_info();
		universe->debug_ui->update();

		SDL_Rect rect;
		rect.x = 400;
		rect.y = 300;
		rect.w = 500;
		rect.h = 355;
		SDL_Point origin;
		origin.x = rect.w / 2;
		origin.y = rect.h / 2;
		t += .5f;
		universe->renderer->render_transform(universe->assets->cat, NULL, &rect, t, &origin, SDL_RendererFlip::SDL_FLIP_NONE);

		//swaps back buffer to front buffer
		SDL_GL_SwapWindow(universe->win_manager->window);

		double ms = std::clock() - start_time;
		if (ms >= 0 && ms < ms_per_frame) { Sleep(floor(ms_per_frame - ms)); }

		++frame_counter;
		if (std::clock() - start_second_time >= 1000) {
			std::cout << "fps: " << frame_counter << "\n";
			universe->debug_ui->update_fps_text(frame_counter);
			frame_counter = 0;
			start_second_time = std::clock();
		}
	}
}