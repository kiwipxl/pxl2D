#include "StateManager.h"
#include <iostream>

#include "../ui/DebugUI.h"
#include "../input/InputManager.h"
#include "WindowManager.h"
#include "../ui/MessageBoxManager.h"
#include "Audio.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static DebugUI* debug_ui;
		static InputManager* input;
		static MessageBoxManager* messagebox;
		static Audio* audio;
};

/**
initiates the statemanager and creates the current state
**/
void StateManager::initiate() {
	create_current_state();
}

/**
removes the current state and creates the new state
**/
void StateManager::switch_state(State new_state) {
	remove_current_state();
	state = new_state;
	create_current_state();
}

/**
handles the creation of the new state
**/
void StateManager::create_current_state() {
	universe->debug_ui->create();
	//universe->messagebox->show(new MessageBox(0, 0, "title", "message\ntest"), OK);
	switch (state) {
		case TITLE_SCREEN:
			break;
	}
	std::cout << "created state " << state << "\n";
}

/**
handles the removing of the current state
**/
void StateManager::remove_current_state() {
	universe->debug_ui->remove();
	universe->messagebox->hide();
	switch (state) {
		case TITLE_SCREEN:
			break;
	}
	std::cout << "removed state " << state << "\n";
}

/**
handles updating of everything depending on the state
**/
void StateManager::update() {
	switch (state) {
		case TITLE_SCREEN:
			break;
	}
	//universe->debug_ui->update();
	universe->audio->update();
	universe->input->update();
	universe->messagebox->update();
}

/**
calls a resize function for parts in the state
**/
void StateManager::resize_current_state() {
	int w = universe->win_manager->screen_width;
	int h = universe->win_manager->screen_height;
	switch (state) {
		case TITLE_SCREEN:
			break;
	}
	universe->messagebox->resize_update(w, h);
}