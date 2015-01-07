#include "Universe.h"

WindowManager* Universe::win_manager;
GameLoop* Universe::game_loop;
Assets* Universe::assets;
MouseManager* Universe::mouse;
DebugUI* Universe::debug_ui;
InputManager* Universe::input;
TimerCallback* Universe::timer;
Renderer* Universe::renderer;
ShaderManager* Universe::shader;

void Universe::initiate() {
	srand(time(NULL));

	TTF_Init();

	win_manager = new WindowManager();
	game_loop = new GameLoop();
	assets = new Assets();
	mouse = new MouseManager();
	debug_ui = new DebugUI();
	input = new InputManager();
	timer = new TimerCallback();
	renderer = new Renderer();
	shader = new ShaderManager();

	debug_ui->create();

	win_manager->initiate();
	assets->initiate();
	input->initiate();
	renderer->initiate();
	game_loop->start();
}