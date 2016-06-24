#ifndef _SPRITE_H
#define _SPRITE_H

#include <string>
#include "graphics/Batch.h"
#include "API.h"

namespace pxl { namespace graphics {

    class Sprite {

    public:
		    /**
		    \*brief: sprite constructor
		    **/
		    Sprite();
		    /**
		    \*brief: sprite constructor
		    **/
		    Sprite(const Texture& source);
		    /**
		    \*brief: sprite deconstructor
		    **/
            ~Sprite();
        
            float x = 0;
            float y = 0;
		    float width = 0; /*> The width boundaries of the sprite */
		    float height = 0; /*> The height boundaries of the sprite */
		    Rect src_rect; /*> The spriteure source rendering boundaries */
		    float rotation = 0;
		    int z_depth = 0;
		    Colour colour;
		    Vec2 scale; /*> The scale of the sprite */
		    Vec2 rotation_origin;
		    Vec2 scale_origin;

		    /**
		    \*brief: frees all data from the sprite
		    **/
            virtual void render(Batch* batch);

            Sprite clone();

		    /**
		    \*brief: frees all data from the sprite
		    **/
		    virtual void free();

		    const Texture* get_texture() { return texture_source; }
		    void set_texture(const Texture& source);

    private:
		    bool texture_set = false; /*> Defines whether or not the sprite has been loaded or not */
		    const Texture* texture_source = NULL;
		    Rect rect; /*> The rendering boundaries */

            void init();

		    float get_texture_width() { return texture_source->get_width(); }
		    float get_texture_height() { return texture_source->get_height(); }
    };
}};

#endif