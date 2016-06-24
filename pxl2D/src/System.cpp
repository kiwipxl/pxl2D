#include "System.h"

namespace pxl {

    using namespace system;

    void system_init() {
        math::init();
        init_assets();
    }
};