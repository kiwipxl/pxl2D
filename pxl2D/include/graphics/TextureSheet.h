#ifndef _TEXTURE_SHEET_H
#define _TEXTURE_SHEET_H

#include <vector>
#include "graphics/Batch.h"
#include "system/Window.h"

namespace pxl { namespace graphics {

    class TextureSheet : public Texture {

	    public:
		    /**
		    \*brief: default constructor
		    **/
		    TextureSheet();
		    /**
		    \*brief: batch deconstructor
		    **/
		    ~TextureSheet();

		    //sheet info
		    Colour bg_colour;

		    /**
		    \*brief: creates the texture sheet from all added bitmaps and textures
		    **/
		    void create_sheet(Channel sheet_channel = CHANNEL_RGBA, bool dispose_batch = true, bool dispose_list = false, bool clear_list = true);

		    void clear();

		    /** Adds the specified texture to the batch render queue and transforms it with all specified parameters
		    @param texture The texture to add to the batch
		    @param rect Specifies where on the screen the texture will be rendered to
		    @param src_rect Specifies which part of the texture to use. Use NULL to use the whole texture
		    @param r, g, b, a colour ranges from 0 to 1 which set the texture colour
		    @param rotation the rotation transformation of the texture
		    @param origin the origin point of which the texture rotates around. Use NULL for top-left (0, 0)
		    @param flip defines the flip transformation for the texture
		    @param shader The shader to use when rendering this texture. Use NULL to use the default shader
		    **/
		    void add(Texture* texture, Rect* rect, Rect* src_rect = NULL, 
				     float rotation = 0, Vec2* rotation_origin = NULL, Vec2* scale_origin = NULL, 
				     int z_depth = 0, Colour colour = COLOUR_WHITE, 
				     ShaderProgram* shader = NULL, BlendMode blend_mode = BLEND);

		    void set_width(int new_width) { width = new_width; }
		    void set_height(int new_height) { height = new_height; }

		    /**
		    \*brief: deletes all texture sheet information
		    **/
		    void free();

	    private:
            Batch* batch = NULL;
		    std::vector<Texture*> texture_list;
    };
}};

#endif