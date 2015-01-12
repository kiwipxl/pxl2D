#include "PXL_PixelBuffer.h"
#include <iostream>

PXL_PixelBuffer::PXL_PixelBuffer() {

}

void PXL_PixelBuffer::free() {

}

PXL_PixelBuffer::~PXL_PixelBuffer() {
	free();
}