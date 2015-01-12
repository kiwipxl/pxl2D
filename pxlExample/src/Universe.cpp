#include "Universe.h"

WindowManager* Universe::win_manager;
GameLoop* Universe::game_loop;
Assets* Universe::assets;

void Universe::initiate() {
	srand(time(NULL));

	win_manager = new WindowManager();
	game_loop = new GameLoop();
	assets = new Assets();

	win_manager->initiate();
	assets->initiate();
	game_loop->start();
}