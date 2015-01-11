#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <SDL.h>
#include <ctime>
#include <time.h>

class Universe;

class GameLoop {

	public:
		GameLoop() { }

		void start();

		void set_fps(int new_fps) {
			fps = new_fps;
			ms_per_frame = 1000 / fps;
		} 

		int get_fps() { return fps; }

	private:
		Universe* universe;
		SDL_Event e;
		bool quit;
		std::clock_t start_time;
		std::clock_t start_second_time;
		float fps;
		float ms_per_frame;
		int frame_counter;
};

#endif