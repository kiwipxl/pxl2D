#ifndef _PXL_H
#define _PXL_H

#include "PXLAPI.h"

namespace pxl {

    const int kInitPXLUnknownError = 0x00;
    const int kInitPXLSuccess = 0x01;
    const int kInitPXLSDLError = 0x02;
    const int kInitPXLGlewError = 0x03;

    extern int32 initPXL();
    extern void terminatePXL();
};

#endif