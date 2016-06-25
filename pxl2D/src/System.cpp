#include "System.h"

namespace pxl {

    using namespace sys;

    void system_init() {
        math::init();
        init_assets();
    }
};