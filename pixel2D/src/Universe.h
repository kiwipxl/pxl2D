#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "managers/WindowManager.h"
#include "managers/Assets.h"
#include "GameLoop.h"
#include "tools/Point2D.h"
#include "input/MouseManager.h"
#include "input/InputManager.h"
#include "ui/DebugUI.h"
#include "tools/timer/TimerCallback.h"
#include "renderer/Renderer.h"
#include "renderer/shader/ShaderManager.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static GameLoop* game_loop;
		static Assets* assets;

		static MouseManager* mouse;
		static InputManager* input;

		static Renderer* renderer;
		static ShaderManager* shader;

		static TimerCallback* timer;

		static DebugUI* debug_ui;

		void initiate();
};

#endif