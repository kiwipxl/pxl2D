#ifndef _PXL_SPRITE_H
#define _PXL_SPRITE_H

#include <string>
#include "graphics/PXL_Batch.h"
#include "system/PXL_API.h"

class PXL_Origin;

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
        
        float x = 0;
        float y = 0;
		float width = 0; /*> The width boundaries of the sprite */
		float height = 0; /*> The height boundaries of the sprite */
		PXL_Rect src_rect; /*> The spriteure source rendering boundaries */
		float rotation = 0;
		int z_depth = 0;
		PXL_Colour colour;
		PXL_Vec2 scale; /*> The scale of the sprite */
		PXL_Origin rotation_origin;
		PXL_Origin scale_origin;

		/**
		\*brief: frees all data from the sprite
		**/
        virtual void render(PXL_Batch* batch);

        PXL_Sprite clone();

		/**
		\*brief: frees all data from the sprite
		**/
		virtual void free();

		void set_texture(const PXL_Texture& source);

	protected:
		bool texture_set = false; /*> Defines whether or not the sprite has been loaded or not */
		const PXL_Texture* texture_source = NULL;
		PXL_Rect rect; /*> The rendering boundaries */

		void init();

		float get_texture_width() { return texture_source->get_width(); }
		float get_texture_height() { return texture_source->get_height(); }
};

enum PXL_OriginType {
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

class PXL_Origin {

	public:
		PXL_Origin(PXL_Sprite* s) { parent = s; }

		void set(float x, float y) {
			origin_type = PXL_ORIGIN_CUSTOM;
			origin.x = x; origin.y = y;
		}

		void set(const PXL_OriginType origin_point) {
			origin_type = origin_point;

			switch (origin_point) {
				case PXL_ORIGIN_TOP_LEFT:
					origin.x = 0; origin.y = 0;
					break;
				case PXL_ORIGIN_TOP_CENTER:
					origin.x = parent->width / 2; origin.y = 0;
					break;
				case PXL_ORIGIN_TOP_RIGHT:
					origin.x = parent->width; origin.y = 0;
					break;
				case PXL_ORIGIN_MID_LEFT:
					origin.x = 0; origin.y = parent->height / 2;
					break;
				case PXL_ORIGIN_CENTER:
					origin.x = parent->width / 2; origin.y = parent->height / 2;
					break;
				case PXL_ORIGIN_MID_RIGHT:
					origin.x = parent->width; origin.y = parent->height / 2;
					break;
				case PXL_ORIGIN_BOT_LEFT:
					origin.x = 0; origin.y = parent->height;
					break;
				case PXL_ORIGIN_BOT_CENTER:
					origin.x = parent->width / 2; origin.y = parent->height;
					break;
				case PXL_ORIGIN_BOT_RIGHT:
					origin.x = parent->width; origin.y = parent->height;
					break;
			}
		}

		PXL_Vec2 get_vec() { return origin; }
		PXL_OriginType get_type() { return origin_type; }

	private:
		PXL_Sprite* parent;
		PXL_Vec2 origin; /*> The origin point of the sprite to perform rotation and scaling transformations */
		PXL_OriginType origin_type = PXL_ORIGIN_TOP_LEFT;
};

#endif