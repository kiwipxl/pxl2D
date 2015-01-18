#ifndef TEXTURE_SHEET_H
#define TEXTURE_SHEET_H

#include <glew.h>
#include <vector>
#include "PXL_Structs.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_VertexPoint.h"

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
		bool background_blending = false;

		/**
		\*brief: creates the texture sheet from all added bitmaps and textures
		**/
		void create();

		/**
		\*brief: adds bitmap pixels to the texture sheet
		\*param [bitmap]: holds all pixel information for an image
		\*param [rect]: where in the sheet the rect will be placed
		**/
		void add(PXL_Bitmap* bitmap, PXL_Rect* rect);

		/**
		\*brief: adds bitmap pixels to the texture sheet
		\*param [bitmap]: holds all pixel information for an image
		\*param [rect]: where in the sheet the rect will be placed
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect);

		/**
		\*brief: adds a pixel buffer to the texture sheet
		\*param [buffer]: holds all pixel information for an image
		\*param [rect]: where in the sheet the rect will be placed
		**/
		void add(PXL_PixelBuffer* buffer, PXL_Rect* rect);

		/**
		\*brief: sets the background colour where no textures are when the sheet is created
		\*param [r, g, b, a]: colour values ranging from 0 to 255 that define the background colour
		\*param [blending]: defines whether alpha images will be filled with the background colour
		**/
		void set_background_colour(int r, int g, int b, int a, bool blending = false);
		/**
		\*brief: sets the background colour where no textures are when the sheet is created
		\*param: vector of r, g, b, a colours
		\*param [blending]: defines whether alpha images will be filled with the background colour
		**/
		void set_background_colour(PXL_RGBA colour, bool blending = false);

		/**
		\*brief: sets the background colour where no textures are when the sheet is created
		\*param: vector of r, g, b, a colours
		\*param [blending]: defines whether alpha images will be filled with the background colour
		**/
		void set_background_colour(PXL_Vec4 colour, bool blending = false);

		/**
		\*brief: deletes all texture sheet information
		**/
		void free();

	private:
		//sheet info
		char* pixels;
		vector<PXL_PixelBuffer*> pixel_data;
		PXL_RGBA bg_colour;
};

#endif