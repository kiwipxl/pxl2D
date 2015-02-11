#ifndef _PXL_FONT_UTILS_H
#define _PXL_FONT_UTILS_H

#include "PXL_Font.h"

//define FT_Library struct
typedef struct FT_LibraryRec_* FT_Library;
extern FT_Library PXL_FT_lib;

/**
\*brief: initialises font, note: this should only ever be called by PXL
**/
extern void PXL_font_init();

#endif