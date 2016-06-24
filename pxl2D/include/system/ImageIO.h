#ifndef _IMAGE_IO_H
#define _IMAGE_IO_H

#include <string>
#include <iostream>
#include "graphics/Bitmap.h"

namespace pxl { namespace system {

    extern void init_assets();

    /**
    \*brief: loads the contents of a png image into a Bitmap
    \*param [file_name]: the path and file name of the png to load
    \*param [bitmap]: The bitmap object to load the png data into. If this value is NULL, a new bitmap will be created with the new values
    \*return Returns a newly created bitmap if the bitmap parameter is NULL, otherwise the specified bitmap will be returned. However, 
    if an error occurs NULL will be returned.
    **/
    extern graphics::Bitmap* load_png(std::string file_name, graphics::Bitmap* bitmap = NULL);
}};

#endif