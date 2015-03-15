#ifndef _PXL_IMAGE_IO_H
#define _PXL_IMAGE_IO_H

#include <string>
#include <iostream>
#include "graphics/PXL_Bitmap.h"

/**
\*brief: loads the contents of a png image into a PXL_Bitmap
\*param [file_name]: the path and file name of the png to load
\*param [bitmap]: The bitmap object to load the png data into. If this value is NULL, a new bitmap will be created with the new values
\*return Returns a newly created bitmap if the bitmap parameter is NULL, otherwise the specified bitmap will be returned. However, 
if an error occurs NULL will be returned.
**/
extern PXL_Bitmap* PXL_load_png(std::string file_name, PXL_Bitmap* bitmap = NULL);

#endif