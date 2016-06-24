#include "system/Math.h"
#include <iostream>

namespace pxl { namespace math {

    double* cos_table;
    double* sin_table;
    const int table_size = 5760;
    const double table_size_pi = table_size / PXL_PI_2;
    const int table_overflow = 127 + table_size;

    void init() {
        cos_table = new double[table_size];
        for (int c = 0; c < table_size; ++c) {
            cos_table[c] = cos(c / table_size_pi);
        }
        sin_table = new double[table_size];
        for (int s = 0; s < table_size; ++s) {
            sin_table[s] = sin(s / table_size_pi);
        }
    }

    double fast_cos(double degrees) {
        return cos_table[int((int(degrees * table_size_pi) % table_size) + INT_MAX - table_overflow) % table_size];
    }

    double fast_sin(double degrees) {
        return sin_table[int((int(degrees * table_size_pi) % table_size) + INT_MAX - table_overflow) % table_size];
    }

    float clamp(float x, float min, float max) {
        return x < min ? min : x && x > max ? max : x;
    }

    float min(float x1, float x2) {
        return x1 < x2 ? x1 : x2;
    }

    float max(float x1, float x2) {
        return x1 > x2 ? x1 : x2;
    }
        
    int wrap(int x, int min, int max) {
        return int(((x + min) % max) + INT_MAX - (int(INT_MAX) % max)) % max;
    }
}};