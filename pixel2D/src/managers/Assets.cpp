#include "Assets.h"
#include <iostream>
#include "WindowManager.h"

class Universe {

	public:
		static WindowManager* win_manager;
};

void Assets::initiate() {
	cat = load_texture("cat.png");
}

/**
loads an image in the assets folder and returns a texture from it
**/
Texture* Assets::load_texture(char* path, bool dispose_surface) {
	std::string texturepath = "assets/";
	texturepath += path;
	SDL_Surface* surface = IMG_Load(texturepath.c_str());
	Texture* texture = new Texture();
	texture->create_texture(surface);
	if (dispose_surface) {
		SDL_FreeSurface(surface);
	}else {
		texture->s = surface;
	}
	textures.push_back(texture);
	return texture;
}

/**
free memory from all textures loaded
**/
void Assets::free_textures() {
	for (Texture* &texture : textures) {
		delete texture;
	}
	textures.clear();
}