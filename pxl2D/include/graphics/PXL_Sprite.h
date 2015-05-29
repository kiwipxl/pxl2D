#ifndef _PXL_SPRITE_H
#define _PXL_SPRITE_H

#include <string>
#include "graphics/PXL_Batch.h"
#include "graphics/PXL_Origin.h"
#include "system/PXL_API.h"

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

		float get_texture_width() { return texture_source->get_width(); }
		float get_texture_height() { return texture_source->get_height(); }
};

#endif