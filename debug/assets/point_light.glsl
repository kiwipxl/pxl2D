#START_VERTEX
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

#END_VERTEX

#START_FRAGMENT
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
uniform float points[6];

void main() {
	ivec2 size = textureSize(t0, 0);
  	pixel = texture(t0, tex_coord);
  	vec2 pos = tex_coord * size;
  	for (int n = 0; n < 2; n += 3) {
  		float dist = sqrt(pow(pos.x - points[n], 2) + pow(pos.y - points[n + 1], 2));
  		if (dist <= points[n + 2]) {
  			float intensity = .25;
  			pixel.r += intensity - (dist / (points[n + 2] / intensity));
  			pixel.a -= intensity - (dist / (points[n + 2] / intensity));
  		}
  	}
}

#END_FRAGMENT