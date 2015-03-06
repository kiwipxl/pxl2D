#ifndef _PXL_TEXTURE_SHEET_H
#define _PXL_TEXTURE_SHEET_H

#include <vector>
#include "PXL_Batch.h"

class PXL_TextureSheet : public PXL_Texture {

	public:
		/**
		\*brief: default constructor
		**/
		PXL_TextureSheet(PXL_BatchSize max_vertices = PXL_BATCH_SMALL);
		/**
		\*brief: batch deconstructor
		**/
		~PXL_TextureSheet();

		//sheet info
		PXL_Colour bg_colour;

		/**
		\*brief: creates the texture sheet from all added bitmaps and textures
		**/
		void create_sheet(PXL_Channel sheet_channel = PXL_CHANNEL_RGBA, bool dispose_batch = true, bool dispose_list = false, bool clear_list = true);

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
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect = NULL, float rotation = 0, PXL_Vec2* origin = NULL,
				 PXL_Flip flip = PXL_FLIP_NONE, int z_depth = 0, PXL_Colour colour = PXL_COLOUR_WHITE, 
				 PXL_ShaderProgram* shader = NULL, PXL_BlendMode blend_mode = PXL_BLEND);

		void set_width(int new_width) { width = new_width; }
		void set_height(int new_height) { height = new_height; }

		/**
		\*brief: deletes all texture sheet information
		**/
		void free();

	private:
		PXL_Batch* batch;
		std::vector<PXL_Texture*> texture_list;
};

#endif