#include "PXL.h"

int PXL_screen_width;
int PXL_screen_height;
int PXL_center_screen_x;
int PXL_center_screen_y;

void PXL_init(int screen_width, int screen_height) {
	PXL_screen_width = screen_width;
	PXL_screen_height = screen_height;
	PXL_center_screen_x = PXL_screen_width / 2;
	PXL_center_screen_y = PXL_screen_height / 2;

	PXL_shader_init();
}