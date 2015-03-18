#include "system/PXL_Math.h"
#include <iostream>

double* cos_table;
double* sin_table;
const int table_size = 5760;
const double table_size_pi = table_size / PXL_two_pi;
const int table_overflow = 127 + table_size;

void PXL_init_math() {
	cos_table = new double[table_size];
	for (int c = 0; c < table_size; ++c) {
		cos_table[c] = cos(c / table_size_pi);
	}
	sin_table = new double[table_size];
	for (int s = 0; s < table_size; ++s) {
		sin_table[s] = sin(s / table_size_pi);
	}
}

double PXL_fast_cos(double degrees) {
	return cos_table[int((int(degrees * table_size_pi) % table_size) + INT_MAX - table_overflow) % table_size];
}

double PXL_fast_sin(double degrees) {
	return sin_table[int((int(degrees * table_size_pi) % table_size) + INT_MAX - table_overflow) % table_size];
}