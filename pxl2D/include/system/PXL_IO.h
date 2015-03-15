#ifndef _PXL_IO_H
#define _PXL_IO_H

#include <string>

//loads file contents
//todo: can maybe have pxl_file which contains more information such as file name, contents, size, ect
std::string PXL_load_file_contents(std::string file_name);

#endif