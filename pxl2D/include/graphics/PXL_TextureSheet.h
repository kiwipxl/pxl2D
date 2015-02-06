#ifndef _PXL_TEXTURE_SHEET_H
#define _PXL_TEXTURE_SHEET_H

#include <glew.h>
#include <vector>
#include "PXL_Structs.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"

class PXL_TextureSheet : public PXL_Texture {

	public:
		/**
		\*brief: default constructor
		**/
		PXL_TextureSheet();
		/**
		\*brief: batch deconstructor
		**/
		~PXL_TextureSheet();

		//sheet info
		bool alpha_blending = true;
		std::vector<PXL_PixelBuffer*> pixel_data;
		PXL_RGBA bg_colour;

		/**
		\*brief: creates the texture sheet from all added bitmaps and textures
		**/
		void create();

		/**
		\*brief: deletes all pixel buffers added to this sheet
		**/
		void delete_pixel_vec();

		/**
		\*brief: adds bitmap pixels to the texture sheet
		\*param [bitmap]: the bitmap to be added to the sheet
		\*param [rect]: where in the sheet the rect will be placed
		\*param [src_rect]: the region of the bitmap which will be used
		**/
		void add(PXL_Bitmap* bitmap, PXL_Rect* rect, PXL_Rect* src_rect = NULL);

		/**
		\*brief: adds bitmap pixels to the texture sheet
		\*param [texture]: the texture to be added to the sheet
		\*param [rect]: where in the sheet the rect will be placed
		\*param [src_rect]: the region of the texture which will be used
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect = NULL);

		/**
		\*brief: adds a pixel buffer to the texture sheet
		\*param [buffer]: holds all pixel information for an image
		\*param [rect]: where in the sheet the rect will be placed
		\*param [src_rect]: the region of the texture which will be used
		**/
		void add(PXL_PixelBuffer* buffer, PXL_Rect* rect, PXL_Rect* src_rect = NULL);

		/**
		\*brief: deletes all texture sheet information
		**/
		void free();
};

#endif