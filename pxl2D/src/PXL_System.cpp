#include "PXL_System.h"

void PXL_system_init() {
	PXL_math_init();

	timeBeginPeriod(1);
}

char* PXL_append_char(char* c1, char* c2) {
	char* buffer = new char[strlen(c1) + strlen(c2)];
	strcpy(buffer, c1);
	strcat(buffer, c2);
	return buffer;
}