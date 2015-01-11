#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "managers/WindowManager.h"
#include "managers/Assets.h"
#include "GameLoop.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static GameLoop* game_loop;
		static Assets* assets;

		void initiate();
};

#endif