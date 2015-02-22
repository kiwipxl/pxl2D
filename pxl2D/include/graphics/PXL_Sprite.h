#ifndef _PXL_SPRITE_H
#define _PXL_SPRITE_H

#include <string>
#include "PXL_Batch.h"
#include "system/PXL_API.h"

enum PXL_Origin {
	PXL_ORIGIN_TOP_LEFT, 
	PXL_ORIGIN_TOP_CENTER, 
	PXL_ORIGIN_TOP_RIGHT, 
	PXL_ORIGIN_MID_LEFT, 
	PXL_ORIGIN_CENTER, 
	PXL_ORIGIN_MID_RIGHT, 
	PXL_ORIGIN_BOT_LEFT, 
	PXL_ORIGIN_BOT_CENTER, 
	PXL_ORIGIN_BOT_RIGHT,  
	PXL_ORIGIN_CUSTOM, 
};

class PXL_Sprite {

	public:
		/**
		\*brief: sprite constructor
		**/
		PXL_Sprite();
		/**
		\*brief: sprite constructor
		**/
		PXL_Sprite(const PXL_Texture& source);
		/**
		\*brief: sprite deconstructor
		**/
		~PXL_Sprite();

		int x = 0;
		int y = 0;
		float width = 0; /*> The width boundaries of the sprite */
		float height = 0; /*> The height boundaries of the sprite */
		PXL_Rect src_rect; /*> The spriteure source rendering boundaries */
		float rotation = 0;
		int z_depth = 0;
		PXL_Colour colour;
		PXL_Vec2 scale; /*> The scale of the sprite */

		void set_texture(const PXL_Texture& source);

		void set_colour(float r, float g, float b, float a) {
			colour.r = r;
			colour.g = g;
			colour.b = b;
			colour.a = a;
		}

		void set_origin(float x = 0, float y = 0);
		void set_origin(const PXL_Origin origin_point = PXL_ORIGIN_TOP_LEFT);

		float get_texture_width() { return texture_source->get_width(); }
		float get_texture_height() { return texture_source->get_height(); }

		PXL_Vec2 get_origin() { return origin; }
		PXL_Origin get_origin_type() { return origin_type; }

		/**
		\*brief: frees all data from the sprite
		**/
		void render(PXL_Batch* batch);

		/**
		\*brief: frees all data from the sprite
		**/
		void free();

	private:
		bool texture_set = false; /*> Defines whether or not the sprite has been loaded or not */
		const PXL_Texture* texture_source = NULL;
		PXL_Rect rect; /*> The rendering boundaries */
		PXL_Vec2 origin; /*> The origin point of the sprite to perform rotation and scaling transformations */
		PXL_Origin origin_type = PXL_ORIGIN_TOP_LEFT;
};

#endif