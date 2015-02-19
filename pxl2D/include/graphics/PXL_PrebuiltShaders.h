#ifndef _PXL_PREBUILT_SHADERS_H
#define _PXL_PREBUILT_SHADERS_H

#include <string>
#include <iostream>

#define GLSL(src) "#version 150 core\n" #src

/**

	-------------- default vertex shader -------------

	author: Richman Stewart

	simple vertex shader that sets the position
	to the specified matrix and position while
	passing the vertex colour and tex coords
	to the fragment shader

**/
const char* PXL_basic_vertex_shader_str = GLSL(
	//[START_VERTEX]

	in vec2 a_position;
	in vec2 a_tex_coord;
	in vec4 a_colour;

	uniform mat4 matrix;

	out vec4 v_colour;
	out vec2 tex_coord;

	void main() {
		v_colour = a_colour;
		tex_coord = a_tex_coord;
		gl_Position = matrix * vec4(a_position, 0, 1);
	}

	//[END_VERTEX]
);

/**

	------------ default fragment shader ------------

	author: Richman Stewart

	simple default fragment shader that multiplies
	the vertex colour with a texel

**/
const char* PXL_default_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;

	void main() {
		//pixel = v_colour * texture(t0, tex_coord);
		pixel = vec4(1, 0, 0, 1);
	}

	//[END_FRAGMENT]
);

/**

	------------ one pass bloom shader ------------

	author: Richman Stewart

	applies a gaussian blur horizontally and vertically
	and applies it on top of the original texture

	------------------ use ------------------------

	outline_size - defines the spread x and y
	outline_intensity - bloom intensity

**/
const char* PXL_bloom_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform float outline_spread;
	uniform float outline_intensity;

	void main() {
		ivec2 size = textureSize(t0, 0);

		float uv_x = tex_coord.x * size.x;
		float uv_y = tex_coord.y * size.y;

		vec4 sum = vec4(0.0);
		for (int n = 0; n < 9; ++n) {
			uv_y = (tex_coord.y * size.y) + (outline_spread * float(n - 4));
			vec4 h_sum = vec4(0.0);
			h_sum += texelFetch(t0, ivec2(uv_x - (4.0 * outline_spread), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - (3.0 * outline_spread), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - (2.0 * outline_spread), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - outline_spread, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + outline_spread, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (2.0 * outline_spread), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (3.0 * outline_spread), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (4.0 * outline_spread), uv_y), 0);
			sum += h_sum / 9.0;
		}

		pixel = v_colour * (texture(t0, tex_coord) + ((sum / 9.0) * outline_intensity));
	}

	//[END_FRAGMENT]
);

/**

	------------ one pass blur shader ------------

		author: Richman Stewart

		applies a gaussian blur horizontally and vertically

	------------------ use ------------------------

		outline_size - blur spread amount

**/
const char* PXL_blur_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform vec2 outline_size;

	void main() {
		ivec2 size = textureSize(t0, 0);

		float uv_x = tex_coord.x * size.x;
		float uv_y = tex_coord.y * size.y;

		vec4 sum = vec4(0.0);
		for (int n = 0; n < 9; ++n) {
			uv_y = (tex_coord.y * size.y) + (outline_size.y * float(n - 4.5));
			vec4 h_sum = vec4(0.0);
			h_sum += texelFetch(t0, ivec2(uv_x - (4.0 * outline_size.x), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - (3.0 * outline_size.x), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - (2.0 * outline_size.x), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x - outline_size.x, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + outline_size.x, uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (2.0 * outline_size.x), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (3.0 * outline_size.x), uv_y), 0);
			h_sum += texelFetch(t0, ivec2(uv_x + (4.0 * outline_size.x), uv_y), 0);
			sum += h_sum / 9.0;
		}

		pixel = sum / 9.0;
	}

	//[END_FRAGMENT]
);

/**
	
	------------ grayscale fragment shader ------------

	author: Richman Stewart

	changes each colour to grayscale using NTSC
	conversion weights

**/
const char* PXL_grayscale_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;

	void main() {
		pixel = texture(t0, tex_coord);
		float g = dot(pixel.rgb, vec3(0.299, 0.587, 0.114));
		pixel = vec4(g, g, g, v_colour.a * pixel.a);
	}

	//[END_FRAGMENT]
);

/**
	
	------------ one pass glow shader ------------

	author: Richman Stewart

	applies a gaussian glow horizontally and vertically
	behind the original texture

	------------------ use ------------------------

	outline_size - defines the spread x and y
	outline_colour - the colour of the glow
	outline_intensity - glow intensity

**/
const char* PXL_glow_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform float outline_size;
	uniform vec3 outline_colour;
	uniform float outline_intensity;
	uniform float outline_threshold;

	void main() {
		pixel = texture(t0, tex_coord);
		if (pixel.a <= outline_threshold) {
			ivec2 size = textureSize(t0, 0);
	
			float uv_x = tex_coord.x * size.x;
			float uv_y = tex_coord.y * size.y;

			float sum = 0.0;
			for (int n = 0; n < 9; ++n) {
				uv_y = (tex_coord.y * size.y) + (outline_size * float(n - 4.5));
				float h_sum = 0.0;
				h_sum += texelFetch(t0, ivec2(uv_x - (4.0 * outline_size), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - (3.0 * outline_size), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - (2.0 * outline_size), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - outline_size, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + outline_size, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (2.0 * outline_size), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (3.0 * outline_size), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (4.0 * outline_size), uv_y), 0).a;
				sum += h_sum / 9.0;
			}

			pixel = vec4(outline_colour, (sum / 9.0) * outline_intensity);
		}
	}

	//[END_FRAGMENT]
);

/**
	
	------------ one pass outline shader ------------

	author: Richman Stewart

	applies a gaussian blur horizontally and vertically
	behind the original texture and makes it black

	------------------ use ------------------------

	outline_thickness - outline spread amount
	outline_colour - colour of the outline

**/
const char* PXL_outline_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coords;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform float outline_thickness = 1;
	uniform vec4 outline_colour = vec4(0, 0, 0, 1);
	uniform float outline_threshold = .5;

	void main() {
		pixel = texture(t0, tex_coords);

		if (pixel.a <= outline_threshold) {
			ivec2 size = textureSize(t0, 0);

			float uv_x = tex_coords.x * size.x;
			float uv_y = tex_coords.y * size.y;

			float sum = 0.0;
			for (int n = 0; n < 9; ++n) {
				uv_y = (tex_coords.y * size.y) + (outline_thickness * float(n - 4.5));
				float h_sum = 0.0;
				h_sum += texelFetch(t0, ivec2(uv_x - (4.0 * outline_thickness), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - (3.0 * outline_thickness), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - (2.0 * outline_thickness), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x - outline_thickness, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + outline_thickness, uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (2.0 * outline_thickness), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (3.0 * outline_thickness), uv_y), 0).a;
				h_sum += texelFetch(t0, ivec2(uv_x + (4.0 * outline_thickness), uv_y), 0).a;
				sum += h_sum / 9.0;
			}

			if (sum / 9.0 >= 0.0001) {
				pixel = outline_colour;
			}
		}
	}

	//[END_FRAGMENT]
);

/**

	------------ point light fragment shader ------------

	author: Richman Stewart

	repeats the original texture x, y amount of times

	---------------------- use -----------------------------

	repeat - the amount of times to repeat
	the texture horizontally and vertically

**/
const char* PXL_point_light_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform float points[504];
	uniform int points_length;
	uniform float max_alpha = 1;
	const int point_size = 7;

	void main() {
		pixel = vec4(0, 0, 0, 0);

		vec2 pos = tex_coord * textureSize(t0, 0);
		float size;
		float intensity;
		for (int n = 0; n < points_length; n += point_size) {
			size = points[n + 2];
			intensity = points[n + 3];
			float dist = sqrt(pow(pos.x - points[n], 2) + pow(pos.y - points[n + 1], 2));
			if (dist <= size) {
				float a = intensity - (dist / (size / intensity));
				pixel.r += a * points[n + 4];
				pixel.g += a * points[n + 5];
				pixel.b += a * points[n + 6];
				//pixel.a += a * pixel.rgb;
			}
		}
		pixel.a = clamp(pixel.a, 0, max_alpha);
	}

	//[END_FRAGMENT]
);

/**

	------------ texture repeat fragment shader ------------

	author: Richman Stewart

	repeats the original texture x, y amount of times

	---------------------- use -----------------------------

	repeat - the amount of times to repeat
	the texture horizontally and vertically

**/
const char* PXL_repeat_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform vec2 repeat = vec2(2.0, 2.0);

	void main() {
		ivec2 size = textureSize(t0, 0);
		pixel = v_colour * texelFetch(t0, ivec2(mod(tex_coord.xy * repeat.xy * size.xy, size.xy)), 0);
	}

	//[END_FRAGMENT]
);

/**

	------------ texture repeat fragment shader ------------

	author: Richman Stewart

	repeats the original texture x, y amount of times

	---------------------- use -----------------------------

	repeat - the amount of times to repeat
	the texture horizontally and vertically

**/
const char* PXL_text_shader_str = GLSL(
	//[START_FRAGMENT]

	in vec4 v_colour;
	in vec2 tex_coord;
	out vec4 pixel;

	uniform sampler2D t0;
	uniform vec4 glyph_rects[];

	void main() {
	  pixel = vec4(v_colour.rgb, v_colour.a * texture(t0, tex_coord).a);
	}

	//[END_FRAGMENT]
);

#endif