#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "../renderer/Texture.h"

using namespace std;

class Universe;

class Assets {

	public:
		vector<Texture*> textures;

		Texture* cat;

		void initiate();

		/**
		loads an image in the assets folder and returns a texture from it
		**/
		Texture* load_texture(char* path, bool dispose_surface = true);

		/**
		free memory from all textures loaded
		**/
		void free_textures();

	private:
		Universe* universe;
};

#endif