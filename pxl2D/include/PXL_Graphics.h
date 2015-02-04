#ifndef _PXL_GRAPHICS_H
#define _PXL_GRAPHICS_H

#define GLEW_STATIC
#include "graphics/PXL_Batch.h"
#include "graphics/PXL_ShaderUtils.h"
#include "graphics/PXL_TextureSheet.h"
#include "graphics/PXL_FontUtils.h"
#include "graphics/PXL_Text.h"
#include "graphics/PXL_Lights.h"
#include "graphics/PXL_FrameBuffer.h"
#include <glew.h>

/** Initialises PXL graphics
**/
extern void PXL_graphics_init();
extern void PXL_glew_init();

extern void PXL_set_clear_colour(float r, float g, float b, float a);
extern void PXL_set_clear_depth(float d);
extern void PXL_clear();

#endif