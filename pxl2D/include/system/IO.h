#ifndef _IO_H
#define _IO_H

#include <string>

namespace pxl { namespace system {

    //loads file contents
    //todo: can maybe have pxl_file which contains more information such as file name, contents, size, ect
    std::string read_file_contents(std::string file_name);

    //template<typename... Args>
    extern char* append_char(const char* c1, const char* c2);
}};

#endif