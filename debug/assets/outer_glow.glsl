//[START_VERTEX]
#version 140

/**
-------------- glow vertex shader -------------

    author: Richman Stewart

    simple vertex shader that sets the position
    to the specified matrix and position while
    passing the vertex colour and tex coords
    to the fragment shader

**/

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

//[START_FRAGMENT]
#version 140

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