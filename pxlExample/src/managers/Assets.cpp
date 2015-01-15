#include "Assets.h"
#include <iostream>
#include "WindowManager.h"

class Universe {

	public:
		static WindowManager* win_manager;
};

void Assets::initiate() {
	cat = load_texture("cat.png");
	cat_2 = load_texture("cat2.png");
}

/**
loads an image in the assets folder and returns a texture from it
**/
PXL_Texture* Assets::load_texture(char* path, bool dispose_bitmap) {
	std::string texture_path = "assets/";
	texture_path += path;
	PXL_Bitmap* bitmap = new PXL_Bitmap(texture_path);
	PXL_Texture* texture = new PXL_Texture(bitmap);
	if (dispose_bitmap) {
		//SDL_FreeSurface(bitmap);
	}else {
		//texture->s = bitmap;
	}
	textures.push_back(texture);
	return texture;
}

/**
free memory from all textures loaded
**/
void Assets::free_textures() {
	for (PXL_Texture* &texture : textures) {
		delete texture;
	}
	textures.clear();
}