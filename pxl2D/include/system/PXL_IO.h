#ifndef _PXL_IO_H
#define _PXL_IO_H

#include <string>

//loads file contents
//todo: can maybe have pxl_file which contains more information such as file name, contents, size, ect
std::string PXL_read_file_contents(std::string file_name);

//template<typename... Args>
extern char* PXL_append_char(const char* c1, const char* c2);

#endif