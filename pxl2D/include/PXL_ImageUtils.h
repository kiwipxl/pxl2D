#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>
#include "PXL_PixelBuffer.h"

using namespace std;

extern PXL_PixelBuffer* PXL_load_png(string file_name);

#endif