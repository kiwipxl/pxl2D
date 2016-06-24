#ifndef _STRUCTS_H
#define _STRUCTS_H

namespace pxl {

    struct Rect {

	    Rect() { }
	    Rect(float pos_x, float pos_y, float width, float height) : x(pos_x), y(pos_y), w(width), h(height) { }

	    float x = 0, y = 0, w = 0, h = 0;
    };

    struct Vec2 {

    public:
	    Vec2() { }
        Vec2(float pos_x, float pos_y) : x(pos_x), y(pos_y) { }

        float x = 0, y = 0;
    };

    struct Vec3 : Vec2 {

    public:
	    Vec3() { }
	    Vec3(float pos_x, float pos_y, float pos_z) : Vec2(pos_x, pos_y), z(pos_z) { }

        float z = 0;
    };

};

#endif