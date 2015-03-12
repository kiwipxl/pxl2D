

LOCAL_PATH := $(call my-dir)


#####################################################################
# builds libpng into a static library
#####################################################################


ifndef APP_CLEAN
$(info ~~~~~~~~~~~Building libpng~~~~~~~~~~~)
else
$(info ~~~~~~~~~~~Cleaning libpng~~~~~~~~~~~)
endif


include $(CLEAR_VARS)

LOCAL_MODULE := libpng

LIBPNG_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/lib/libpng/src/*.c)
LIBPNG_SRC_FILES := $(LIBPNG_SRC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/lib/libpng/src/

LOCAL_SRC_FILES := $(LIBPNG_SRC_FILES)


include $(BUILD_STATIC_LIBRARY)
