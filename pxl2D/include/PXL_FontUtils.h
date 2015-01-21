#ifndef FONT_UTILS_H
#define FONT_UTILS_H

#include <string>
#include "PXL_Structs.h"
#include "PXL_Font.h"

using namespace std;

class PXL_Batch;
typedef struct FT_LibraryRec_* FT_Library;

extern FT_Library PXL_FT_lib;

/**
\*brief: initialises font, note: this should only ever be called by PXL
**/
extern void PXL_font_init();

#endif