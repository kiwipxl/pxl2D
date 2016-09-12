#include "PXL.h"

#include "PXLGraphics.h"
#include "PXLSystem.h"
#include "PXLPhysics.h"
#include "PXLInput.h"

namespace pxl {

    int32 initSystem() {
        using namespace sys;

        math::init();
        init_assets();

        return kInitPXLSuccess;
    }

    int32 initGraphics() {
        using namespace graphics;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            return kInitPXLSDLError;
        }

        {
            // create a dummy window, and then destroy it so we can initialise openGL, which
            // is required to initialise glew
            sys::Window* dummyWindow = sys::createWindow(Rect(0, 0, 1, 1), NULL);

            if (glewInit()) {
                return kInitPXLGlewError;
            }
        
            delete dummyWindow;
        }

        set_clear_colour(0, 0, 0, 1);
        set_clear_depth(1);
        clear();

        init_shader();
        init_font();

        return kInitPXLSuccess;
    }

    int32 initPhysics() {
        return kInitPXLSuccess;
    }

    int32 initInput() {
        return kInitPXLSuccess;
    }

    int32 initPXLFailed(int32 res) {
        sys::print << "PXL failed to init. Errcode " << res << "\n";
        return res;
    }

    extern int32 initPXL() {
	    sys::print << "PXL is initialising...\n";

        int32 res;
        if ((res = initSystem()) != kInitPXLSuccess) return initPXLFailed(res);
        if ((res = initGraphics()) != kInitPXLSuccess) return initPXLFailed(res);
        if ((res = initPhysics()) != kInitPXLSuccess) return initPXLFailed(res);
        if ((res = initInput()) != kInitPXLSuccess) return initPXLFailed(res);

        sys::print << "PXL initialised successfully!\n";

        return kInitPXLSuccess;
    }

    extern void terminatePXL() {
        SDL_Quit();
    }
};