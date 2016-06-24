#ifndef _FONT_UTILS_H
#define _FONT_UTILS_H

#include "Font.h"

//define FT_Library struct
typedef struct FT_LibraryRec_* FT_Library;

namespace pxl { namespace graphics {

    extern FT_Library FT_lib;

    /**
    \*brief: initialises font, note: this should only ever be called by PXL
    **/
    extern void init_font();

}};

#endif