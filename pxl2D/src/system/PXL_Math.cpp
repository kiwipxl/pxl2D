#include "PXL_Math.h"
#include <iostream>

double* cos_table;
double* sin_table;
const int table_size = 720;
const double table_size_pi = table_size / PXL_two_pi;

void PXL_math_init() {
	cos_table = new double[table_size];
	double p = 0;
	for (int c = 0; c < table_size; ++c) {
		cos_table[c] = cos(p + (c / table_size_pi));
	}
	sin_table = new double[table_size];
	p = 0;
	for (int s = 0; s < table_size; ++s) {
		sin_table[s] = sin(p + (s / table_size_pi));
	}
}

double PXL_fast_cos(double degrees) {
	return cos_table[int(degrees * table_size_pi) % table_size];
}

double PXL_fast_sin(double degrees) {
	return sin_table[int(degrees * table_size_pi) % table_size];
}