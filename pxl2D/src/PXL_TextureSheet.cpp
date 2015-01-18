#include "PXL_TextureSheet.h"
#include <iostream>

PXL_TextureSheet::PXL_TextureSheet() {
	texture_created = false;
	width = 0;
	height = 0;
}

void PXL_TextureSheet::create() {
	//if the texture is already created then delete the sheet texture
	if (texture_created) {
		glDeleteTextures(1, &id);
		texture_created = false;
	}

	//initialise pixel array for sheet based on sheet width/height multiplied by 4 (r, g, b, a)
	int arr_size = (width * height) * 4;
	pixels = new char[arr_size];
	//set each pixel to the background colour if background blending is true
	if (background_blending) {
		for (int n = 0; n < arr_size; n += 4) {
			pixels[n] = (char)bg_colour.r;
			pixels[n + 1] = (char)bg_colour.g;
			pixels[n + 2] = (char)bg_colour.b;
			pixels[n + 3] = (char)bg_colour.a;
		}
	}

	//loops through all pixel data and copy it into the pixel array
	for (PXL_PixelBuffer* buffer : pixel_data) {
		//start index based in pixels based on buffer x and y position
		int index = (buffer->x * 4) + (buffer->y * (width * 4));
		//calculate scale x and y
		float scale_x = buffer->max_width / float(buffer->width);
		float scale_y = buffer->max_height / float(buffer->height);
		float offset_y = 0;
		//loop through the height of the image
		for (int i = 0; i < buffer->height; ++i) {
			float offset_x = 0;
			offset_y += scale_y - 1;
			//loop through the width of the image multiplied by 4 (r, g, b, a) values
			for (int n = 0; n < buffer->width * 4; ++n) {
				//calculate buffer index in one dimensional buffer array that scales width/height
				int buffer_index = ((buffer->max_width * (i + int(offset_y))) * 4) + n + (int(offset_x / 4) * 4);
				if (buffer_index >= buffer->buffer_size) { break; }

				char value = buffer->buffer[buffer_index];

				//checks whether the current value is an alpha pixel
				if (n % 4 == 3) {
					//calculate pixel offset x based on scale x
					offset_x += (scale_x - 1) * 4;

					//if background blending is enabled, check whether the alpha is greater than 1 
					//and set the colour to the bg colour
					if (background_blending) {
						if (value >= 0) {
							pixels[index + n - 3] = bg_colour.r;
							pixels[index + n - 2] = bg_colour.g;
							pixels[index + n - 1] = bg_colour.b;
							value = bg_colour.a;
						}
					}
				}

				//set pixel value
				pixels[index + n] = value;
			}
			//move to a new line at the beginning of the image
			index += width * 4;
		}
	}

	//create texture from resulting pixels
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, NULL);

	set_filters();

	texture_created = true;
}

/**
\*brief: sets the background colour where no textures are when the sheet is created
\*param [r, g, b, a] colour values ranging from 0 to 255 that define the background colour
\*param [blending]: defines whether alpha images will be filled with the background colour
**/
void PXL_TextureSheet::set_background_colour(int r, int g, int b, int a, bool blending) {
	bg_colour.r = r; bg_colour.g = g; bg_colour.b = b; bg_colour.a = a;
	background_blending = blending;
}

/**
\*brief: sets the background colour where no textures are when the sheet is created
\*param: vector of r, g, b, a colours
\*param [blending]: defines whether alpha images will be filled with the background colour
**/
void PXL_TextureSheet::set_background_colour(PXL_RGBA colour, bool blending) {
	bg_colour = colour;
	background_blending = blending;
}

/**
\*brief: sets the background colour where no textures are when the sheet is created
\*param: vector of r, g, b, a colours
\*param [blending]: defines whether alpha images will be filled with the background colour
**/
void PXL_TextureSheet::set_background_colour(PXL_Vec4 colour, bool blending) {
	bg_colour.r = colour.x; bg_colour.g = colour.y; bg_colour.b = colour.z; bg_colour.a = colour.w;
	blending = background_blending;
}

void PXL_TextureSheet::add(PXL_Bitmap* bitmap, PXL_Rect* rect) {
	add(new PXL_PixelBuffer(rect->x, rect->y, rect->w, rect->h, bitmap->w, bitmap->h, 
							&bitmap->pixels[0], (bitmap->w * bitmap->h) * 4), rect);
}

void PXL_TextureSheet::add(PXL_Texture* texture, PXL_Rect* rect) {
	add(new PXL_PixelBuffer(rect->x, rect->y, rect->w, rect->h, texture->get_width(), texture->get_height(), 
							texture->get_pixels(), (texture->get_width() * texture->get_height()) * 4), rect);
}

void PXL_TextureSheet::add(PXL_PixelBuffer* buffer, PXL_Rect* rect) {
	int w = rect->x + rect->w;
	int h = rect->y + rect->h;
	if (w > width) { width = w; }
	if (h > height) { height = h; }

	pixel_data.push_back(buffer);
}

void PXL_TextureSheet::free() {
	if (texture_created) {
		glDeleteTextures(1, &id);
		texture_created = false;
	}
	pixel_data.clear();
}

PXL_TextureSheet::~PXL_TextureSheet() {
	free();
}