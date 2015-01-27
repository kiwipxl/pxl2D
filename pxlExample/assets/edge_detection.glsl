//[START_VERTEX]
#version 140

/**
-------------- texture coords vertex shader -------------

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
------------ texture repeat fragment shader ------------

    author: Richman Stewart

    repeats the original texture x, y amount of times

---------------------- use -----------------------------

    repeat - the amount of times to repeat
    the texture horizontally and vertically

**/

in vec4 v_colour;
in vec2 tex_coord;
out vec4 pixel;

uniform sampler2D t0;

void main() {
  //pixel = v_colour * texture(t0, tex_coord);
  vec4 s1 = textureOffset(t0, tex_coord, ivec2( -1, -1 ));
  vec4 s2 = textureOffset(t0, tex_coord, ivec2( +1, -1 ));
  vec4 s3 = textureOffset(t0, tex_coord, ivec2( -1, +1 ));
  vec4 s4 = textureOffset(t0, tex_coord, ivec2( +1, +1 ));
  vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
  vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
  vec4 sobel = sqrt(sx * sx + sy * sy);
  pixel = sobel;
}

//[END_FRAGMENT]