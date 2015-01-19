#include "PXL_TextureSheet.h"
#include <iostream>

PXL_TextureSheet::PXL_TextureSheet() {
	//initiate sheet
	texture_created = false;
	width = 0;
	height = 0;
	set_background_colour(0, 0, 0, 0);
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
	//set each pixel to the background colour
	for (int n = 0; n < arr_size; n += 4) {
		pixels[n] = (char)bg_colour.r;
		pixels[n + 1] = (char)bg_colour.g;
		pixels[n + 2] = (char)bg_colour.b;
		pixels[n + 3] = (char)bg_colour.a;
	}

	//loops through all pixel data and copy it into the pixel array
	for (PXL_PixelBuffer* buffer : pixel_data) {
		//start index based in pixels based on buffer x and y position
		int index = (buffer->x * 4) + (buffer->y * (width * 4));
		//calculate scale x and y
		float scale_x = buffer->max_width / float(buffer->width);
		float src_scale_x = float(buffer->src_rect.w) / buffer->max_width;
		float scale_y = buffer->max_height / float(buffer->height);
		float src_scale_y = float(buffer->src_rect.h) / buffer->max_height;
		float offset_y = 0;
		float src_offset_y = 0;
		//loop through the height of the image
		for (int i = 0; i < buffer->height; ++i) {
			float offset_x = 0;
			float src_offset_x = 0;
			offset_y += scale_y - 1;
			src_offset_y += (src_scale_y - 1) * scale_y;
			//loop through the width of the image multiplied by 4 (r, g, b, a) values
			for (int n = 0; n < buffer->width * 4; ++n) {
				//calculate buffer index in one dimensional buffer array that scales width/height
				int buffer_index;
				//calculate y position
				buffer_index = ((buffer->max_width * (i + int(offset_y + (src_offset_y + buffer->src_rect.y)))) * 4);
				//calculate x position
				buffer_index += n + (int((offset_x + src_offset_x) / 4) * 4);
				buffer_index += buffer->src_rect.x * 4;
				if (buffer_index >= buffer->buffer_size) { break; }

				char value = buffer->buffer[buffer_index];

				//checks whether the current pixel value is red
				if (n % 4 == 0) {
					//calculate pixel offset x based on scale x
					offset_x += (scale_x - 1) * 4;
					src_offset_x += (src_scale_x - 1) * (4 * (scale_x));
					//if alpha blending is on, check whether the alpha value for the current pixel 
					//is greater than 0 and skip the current pixel as a previous pixel has already been placed
					if (alpha_blending && buffer->buffer[buffer_index + 3] >= 0) {
						n += 3;
						continue;
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
**/
void PXL_TextureSheet::set_background_colour(int r, int g, int b, int a) {
	bg_colour.r = r; bg_colour.g = g; bg_colour.b = b; bg_colour.a = a;
}

/**
\*brief: sets the background colour where no textures are when the sheet is created
\*param: vector of r, g, b, a colours
**/
void PXL_TextureSheet::set_background_colour(PXL_RGBA colour) {
	bg_colour = colour;
}

/**
\*brief: sets the background colour where no textures are when the sheet is created
\*param: vector of r, g, b, a colours
**/
void PXL_TextureSheet::set_background_colour(PXL_Vec4 colour) {
	bg_colour.r = colour.x; bg_colour.g = colour.y; bg_colour.b = colour.z; bg_colour.a = colour.w;
}

/**
\*brief: adds bitmap pixels to the texture sheet
\*param [bitmap]: the bitmap to be added to the sheet
\*param [rect]: where in the sheet the rect will be placed
\*param [src_rect]: the region of the bitmap which will be used
**/
void PXL_TextureSheet::add(PXL_Bitmap* bitmap, PXL_Rect* rect, PXL_Rect* src_rect) {
	add(new PXL_PixelBuffer(rect->x, rect->y, rect->w, rect->h, bitmap->w, bitmap->h, 
							&bitmap->pixels[0], (bitmap->w * bitmap->h) * 4), rect);
}

/**
\*brief: adds bitmap pixels to the texture sheet
\*param [texture]: the texture to be added to the sheet
\*param [rect]: where in the sheet the rect will be placed
\*param [src_rect]: the region of the texture which will be used
**/
void PXL_TextureSheet::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	add(new PXL_PixelBuffer(rect->x, rect->y, rect->w, rect->h, texture->get_width(), texture->get_height(), 
							texture->get_pixels(), (texture->get_width() * texture->get_height()) * 4), rect, src_rect);
}

/**
\*brief: adds a pixel buffer to the texture sheet
\*param [buffer]: holds all pixel information for an image
\*param [rect]: where in the sheet the rect will be placed
\*param [src_rect]: the region of the texture which will be used
**/
void PXL_TextureSheet::add(PXL_PixelBuffer* buffer, PXL_Rect* rect, PXL_Rect* src_rect) {
	int w = rect->x + rect->w;
	int h = rect->y + rect->h;
	if (w > width) { width = w; }
	if (h > height) { height = h; }

	if (src_rect == NULL) {
		buffer->src_rect.w = buffer->max_width;
		buffer->src_rect.h = buffer->max_height;
	}else {
		buffer->src_rect.x = src_rect->x;
		buffer->src_rect.y = src_rect->y;
		buffer->src_rect.w = src_rect->w;
		buffer->src_rect.h = src_rect->h;
		if (buffer->src_rect.x < 0) { buffer->src_rect.x = 0; }
		if (buffer->src_rect.y < 0) { buffer->src_rect.y = 0; }
		if (buffer->src_rect.x + buffer->src_rect.w >= buffer->max_width) {
			buffer->src_rect.w = buffer->max_width - buffer->src_rect.x;
		}
		if (buffer->src_rect.y + buffer->src_rect.h >= buffer->max_height) {
			buffer->src_rect.h = buffer->max_height - buffer->src_rect.y;
		}
	}

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